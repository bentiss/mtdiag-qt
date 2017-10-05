/*
 * Copyright © 2017 Benjamin Tissoires
 *
 * Duplicated from weston-launch.c:
 *
 * Copyright © 2012 Benjamin Franzke
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>

#include <error.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/signalfd.h>
#include <sys/sysmacros.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/major.h>

#include <pwd.h>
#include <grp.h>

#include "mtdiag-launch.h"

#ifndef EVIOCREVOKE
#define EVIOCREVOKE _IOW('E', 0x91, int)
#endif

#define MAX_ARGV_SIZE 256

struct mtdiag_launch {
	int sock[2];
	int last_input_fd;
	struct passwd *pw;

	int signalfd;

	pid_t child;
	int verbose;
	char *new_user;
};

union cmsg_data { unsigned char b[4]; int fd; };

static gid_t *
read_groups(void)
{
	int n;
	gid_t *groups;

	n = getgroups(0, NULL);

	if (n < 0) {
		fprintf(stderr, "Unable to retrieve groups: %m\n");
		return NULL;
	}

	groups = malloc(n * sizeof(gid_t));
	if (!groups)
		return NULL;

	if (getgroups(n, groups) < 0) {
		fprintf(stderr, "Unable to retrieve groups: %m\n");
		free(groups);
		return NULL;
	}
	return groups;
}

static bool
mtdiag_launch_allowed()
{
	struct group *gr;
	gid_t *groups;
	int i;

	if (getuid() == 0)
		return true;

    gr = getgrnam("mtdiag-launch");
    if (gr) {
		groups = read_groups();
		if (groups) {
			for (i = 0; groups[i]; ++i) {
				if (groups[i] == gr->gr_gid) {
					free(groups);
					return true;
				}
			}
			free(groups);
		}
	}

	return false;
}

static int
setup_launcher_socket(struct mtdiag_launch *ml)
{
	if (socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, ml->sock) < 0)
		error(1, errno, "socketpair failed");

	if (fcntl(ml->sock[0], F_SETFD, FD_CLOEXEC) < 0)
		error(1, errno, "fcntl failed");

	return 0;
}

static int
setup_signals(struct mtdiag_launch *ml)
{
	int ret;
	sigset_t mask;
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;
	ret = sigaction(SIGCHLD, &sa, NULL);
	assert(ret == 0);

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGHUP, &sa, NULL);

	ret = sigemptyset(&mask);
	assert(ret == 0);
	sigaddset(&mask, SIGCHLD);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);
	ret = sigprocmask(SIG_BLOCK, &mask, NULL);
	assert(ret == 0);

	ml->signalfd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
	if (ml->signalfd < 0)
		return -errno;

	return 0;
}

static void
setenv_fd(const char *env, int fd)
{
	char buf[32];

	snprintf(buf, sizeof buf, "%d", fd);
	setenv(env, buf, 1);
}

static int
handle_open(struct mtdiag_launch *ml, struct msghdr *msg, ssize_t len)
{
	int fd = -1, ret = -1;
	char control[CMSG_SPACE(sizeof(fd))];
	struct cmsghdr *cmsg;
	struct stat s;
	struct msghdr nmsg;
	struct iovec iov;
	struct mtdiag_launcher_open *message;
	union cmsg_data *data;

	message = msg->msg_iov->iov_base;
	if ((size_t)len < sizeof(*message))
		goto err0;

	/* Ensure path is null-terminated */
	((char *) message)[len-1] = '\0';

	fd = open(message->path, message->flags);
	if (fd < 0) {
		fprintf(stderr, "Error opening device %s: %m\n",
			message->path);
		goto err0;
	}

	if (fstat(fd, &s) < 0) {
		close(fd);
		fd = -1;
		fprintf(stderr, "Failed to stat %s\n", message->path);
		goto err0;
	}

	if (major(s.st_rdev) != INPUT_MAJOR) {
		close(fd);
		fd = -1;
		fprintf(stderr, "Device %s is not an input device\n",
			message->path);
		goto err0;
	}

err0:
	memset(&nmsg, 0, sizeof nmsg);
	nmsg.msg_iov = &iov;
	nmsg.msg_iovlen = 1;
	if (fd != -1) {
		nmsg.msg_control = control;
		nmsg.msg_controllen = sizeof control;
		cmsg = CMSG_FIRSTHDR(&nmsg);
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;
		cmsg->cmsg_len = CMSG_LEN(sizeof(fd));
		data = (union cmsg_data *) CMSG_DATA(cmsg);
		data->fd = fd;
		nmsg.msg_controllen = cmsg->cmsg_len;
		ret = 0;
	}
	iov.iov_base = &ret;
	iov.iov_len = sizeof ret;

	if (ml->verbose)
		fprintf(stderr, "mtdiag-launch: opened %s: ret: %d, fd: %d\n",
			message->path, ret, fd);
	do {
		len = sendmsg(ml->sock[0], &nmsg, 0);
	} while (len < 0 && errno == EINTR);

	if (len < 0)
		return -1;

	if (fd != -1 && major(s.st_rdev) == INPUT_MAJOR &&
	    ml->last_input_fd < fd)
		ml->last_input_fd = fd;

	return 0;
}

static int
handle_socket_msg(struct mtdiag_launch *ml)
{
	char control[CMSG_SPACE(sizeof(int))];
	char buf[BUFSIZ];
	struct msghdr msg;
	struct iovec iov;
	int ret = -1;
	ssize_t len;
	struct mtdiag_launcher_message *message;

	memset(&msg, 0, sizeof(msg));
	iov.iov_base = buf;
	iov.iov_len  = sizeof buf;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	msg.msg_controllen = sizeof control;

	do {
		len = recvmsg(ml->sock[0], &msg, 0);
	} while (len < 0 && errno == EINTR);

	if (len < 1)
		return -1;

	message = (void *) buf;
	switch (message->opcode) {
	case MTDIAG_LAUNCHER_OPEN:
		ret = handle_open(ml, &msg, len);
		break;
	}

	return ret;
}

static void
quit(struct mtdiag_launch *ml, int status)
{
	close(ml->signalfd);
	close(ml->sock[0]);

	exit(status);
}

#if 0
static void
close_input_fds(struct mtdiag_launch *ml)
{
	struct stat s;
	int fd;

	for (fd = 3; fd <= ml->last_input_fd; fd++) {
		if (fstat(fd, &s) == 0 && major(s.st_rdev) == INPUT_MAJOR) {
			/* EVIOCREVOKE may fail if the kernel doesn't
			 * support it, but all we can do is ignore it. */
			ioctl(fd, EVIOCREVOKE, 0);
			close(fd);
		}
	}
}
#endif

static int
handle_signal(struct mtdiag_launch *ml)
{
	struct signalfd_siginfo sig;
	int pid, status, ret;

	if (read(ml->signalfd, &sig, sizeof sig) != sizeof sig) {
		error(0, errno, "reading signalfd failed");
		return -1;
	}

	switch (sig.ssi_signo) {
	case SIGCHLD:
		pid = waitpid(-1, &status, 0);
		if (pid == ml->child) {
			ml->child = 0;
			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				/*
				 * If mtdiag dies because of signal N, we
				 * return 10+N. This is distinct from
				 * mtdiag-launch dying because of a signal
				 * (128+N).
				 */
				ret = 10 + WTERMSIG(status);
			else
				ret = 0;
			quit(ml, ret);
		}
		break;
	case SIGTERM:
	case SIGINT:
		if (ml->child)
			kill(ml->child, sig.ssi_signo);
		break;
	default:
		return -1;
	}

	return 0;
}

static void
drop_privileges(struct mtdiag_launch *ml)
{
    /* we need to make sure we drop any group cabability first */
    setgroups(0, NULL);
	if (setgid(ml->pw->pw_gid) < 0 ||
#ifdef HAVE_INITGROUPS
	    initgroups(ml->pw->pw_name, ml->pw->pw_gid) < 0 ||
#endif
	    setuid(ml->pw->pw_uid) < 0)
		error(1, errno, "dropping privileges failed");
}

static void
launch_application(struct mtdiag_launch *ml, int argc, char *argv[])
{
	char *child_argv[MAX_ARGV_SIZE];
	sigset_t mask;
	int o, i;

	child_argv[0] = BINDIR "/mtdiag-qt";
	o = 1;

	if (ml->verbose)
		printf("mtdiag-launch: spawned mtdiag (%s) with pid: %d\n", child_argv[0], getpid());

	for (i = 0; i < argc; ++i)
		child_argv[o + i] = argv[i];
	child_argv[o + i] = NULL;

	if (geteuid() == 0)
		drop_privileges(ml);

	setenv_fd("MTDIAG_LAUNCHER_SOCK", ml->sock[1]);

	/* Do not give our signal mask to the new process. */
	sigemptyset(&mask);
	sigaddset(&mask, SIGTERM);
	sigaddset(&mask, SIGCHLD);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	execv(child_argv[0], child_argv);
	error(1, errno, "exec failed");
}

static void
help(const char *name)
{
	fprintf(stderr, "Usage: %s [args...] [-- [mtdiag args..]]\n", name);
	fprintf(stderr, "  -v, --verbose   Be verbose\n");
	fprintf(stderr, "  -h, --help      Display this help message\n");
}

int
main(int argc, char *argv[])
{
    struct mtdiag_launch ml;
	int i, c;
	struct option opts[] = {
		{ "verbose", no_argument,       NULL, 'v' },
		{ "help",    no_argument,       NULL, 'h' },
		{ 0,         0,                 NULL,  0  }
	};

	memset(&ml, 0, sizeof ml);

	while ((c = getopt_long(argc, argv, "u:t::vh", opts, &i)) != -1) {
		switch (c) {
		case 'v':
			ml.verbose = 1;
			break;
		case 'h':
			help("mtdiag-launch");
			exit(EXIT_FAILURE);
		default:
			exit(EXIT_FAILURE);
		}
	}

	if ((argc - optind) > (MAX_ARGV_SIZE - 6))
		error(1, E2BIG, "Too many arguments to pass to mtdiag");

	ml.pw = getpwuid(getuid());
	if (ml.pw == NULL)
		error(1, errno, "failed to get username");

	if (!mtdiag_launch_allowed())
		error(1, 0, "Permission denied. You should add yourself to the 'mtdiag-launch' group.");

	if (setup_launcher_socket(&ml) < 0)
		exit(EXIT_FAILURE);

	if (setup_signals(&ml) < 0)
		exit(EXIT_FAILURE);

	ml.child = fork();
	if (ml.child == -1)
		error(EXIT_FAILURE, errno, "fork failed");

	if (ml.child == 0)
		launch_application(&ml, argc - optind, argv + optind);

	close(ml.sock[1]);

	while (1) {
		struct pollfd fds[2];
		int n;

		fds[0].fd = ml.sock[0];
		fds[0].events = POLLIN;
		fds[1].fd = ml.signalfd;
		fds[1].events = POLLIN;

		n = poll(fds, 2, -1);
		if (n < 0)
			error(0, errno, "poll failed");
		if (fds[0].revents & POLLIN)
			handle_socket_msg(&ml);
		if (fds[1].revents)
			handle_signal(&ml);
	}

	return 0;
}
