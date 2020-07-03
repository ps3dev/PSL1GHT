#ifndef __SELECT_H__
#define __SELECT_H__

#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

typedef unsigned int _net_fd_mask;

typedef struct _net_fd_set {
	_net_fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} _net_fd_set;

#undef fd_set
#define fd_set _net_fd_set

#ifdef __cplusplus
extern "C" {
#endif

int select(int, fd_set*, fd_set*, fd_set*, struct timeval*);
int pselect(int, fd_set*, fd_set*, fd_set*, const struct timespec*, const sigset_t*);

#ifdef __cplusplus
	}
#endif

#endif
