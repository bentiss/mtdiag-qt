#ifndef	_LAUNCHER_MTDIAG_LAUNCHER_H
#define	_LAUNCHER_MTDIAG_LAUNCHER_H	1

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
int launcher_mtdiag_launch_open(int fd, const char *path, int flags);

#ifdef __cplusplus
}
#endif
#endif /* _LAUNCHER_MTDIAG_LAUNCHER_H */
