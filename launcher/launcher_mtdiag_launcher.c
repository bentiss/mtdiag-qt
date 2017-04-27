/*
 * Copyright © 2017 Benjamin Tissoires
 *
 * Duplicated from launcher-weston-launch.c:
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

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "launcher/launcher_mtdiag_launcher.h"
#include "launcher/mtdiag-launch.h"

union cmsg_data { unsigned char b[4]; int fd; };

int
launcher_mtdiag_launch_open(int fd, const char *path, int flags)
{
    int n, ret;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct iovec iov;
    union cmsg_data *data;
    char control[CMSG_SPACE(sizeof data->fd)];
    ssize_t len;
    struct mtdiag_launcher_open *message;

    n = sizeof(*message) + strlen(path) + 1;
    message = malloc(n);
    if (!message)
        return -1;

    message->header.opcode = MTDIAG_LAUNCHER_OPEN;
    message->flags = flags;
    strcpy(message->path, path);

    do {
        len = write(fd, message, n);
    } while (len < 0 && errno == EINTR);
    free(message);

    memset(&msg, 0, sizeof msg);
    iov.iov_base = &ret;
    iov.iov_len = sizeof ret;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = control;
    msg.msg_controllen = sizeof control;

    do {
        len = recvmsg(fd, &msg, MSG_CMSG_CLOEXEC);
    } while (len < 0 && errno == EINTR);

    if (len != sizeof ret ||
        ret < 0)
        return -1;

    cmsg = CMSG_FIRSTHDR(&msg);
    if (!cmsg ||
        cmsg->cmsg_level != SOL_SOCKET ||
        cmsg->cmsg_type != SCM_RIGHTS) {
        fprintf(stderr, "invalid control message\n");
        return -1;
    }

    data = (union cmsg_data *) CMSG_DATA(cmsg);
    if (data->fd == -1) {
        fprintf(stderr, "missing input fd in socket request\n");
        return -1;
    }

    return data->fd;
}

