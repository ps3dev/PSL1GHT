#ifndef __POLL_H__
#define __POLL_H__

#define POLLIN		0x0001
#define POLLPRI		0x0002
#define POLLWRNORM	0x0004
#define POLLERR		0x0008
#define POLLHUP		0x0010
#define POLLNVAL	0x0020
#define POLLRDNORM	0x0040
#define POLLRDBAND	0x0080
#define POLLWRBAND	0x0100
#define POLLOUT		POLLWRNORM

typedef unsigned int nfds_t;

struct pollfd {
	int fd;
	short events;
	short revents;
};

#ifdef __cplusplus
extern "C" {
#endif

int poll(struct pollfd fds[],nfds_t nfds,int timeout);

#ifdef __cplusplus
	}
#endif

#endif
