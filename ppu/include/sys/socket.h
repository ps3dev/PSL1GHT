/*! \file socket.h
 \brief Socket management functions.
*/
#ifndef __SYS_SOCKET_H__
#define __SYS_SOCKET_H__

typedef u32 socklen_t;
typedef u8 sa_family_t;

struct sockaddr
{
	u8 sa_len;
	sa_family_t sa_family;
	char sa_data[];
};

struct iovec
{
	void* iov_base;
	size_t iov_len;
};

struct msghdr
{
	void* msg_name;
	socklen_t msg_namelen;
	struct iovec* msg_iov;
	int msg_iovlen;
	void* msg_control;
	socklen_t msg_controllen;
	int msg_flags;
};

struct cmsghdr
{
	socklen_t cmsg_len;
	int cmsg_level;
	int cmsg_type;
};

struct linger
{
	int l_onoff;
	int l_linger;
};

#define SOCK_STREAM 0x0001
#define SOCK_DGRAM 0x0002
#define SOCK_RAW 0x0003

#define SOL_SOCKET 0xFFFF

#define SO_REUSEADDR 0x0004
#define SO_KEEPALIVE 0x0008
#define SO_BROADCAST 0x0020
#define SO_LINGER 0x0080
#define SO_OOBINLINE 0x0100
#define SO_REUSEPORT 0x0200
#define SO_SNDBUF 0x1001
#define SO_RCVBUF 0x1002
#define SO_SNDLOWAT 0x1003
#define SO_RCVLOWAT 0x1004
#define SO_SNDTIMEO 0x1005
#define SO_RCVTIMEO 0x1006
#define SO_ERROR 0x1007
#define SO_TYPE 0x1008
#define SO_NBIO 0x1100

#define MSG_OOB 0x0001
#define MSG_PEEK 0x0002
#define MSG_DONTROUTE 0x0004
#define MSG_EOR 0x0008
#define MSG_TRUNC 0x0010
#define MSG_CTRUNC 0x0020
#define MSG_WAITALL 0x0040
#define MSG_DONTWAIT 0x0080
#define MSG_BCAST 0x0100
#define MSG_MCAST 0x0200

#define AF_UNSPEC 0x0000
#define AF_UNIX 0x0001
#define AF_INET 0x0002
#define AF_INET6 0x0018
#define AF_MAX 0x0020

#define SHUT_RD 0x0000
#define SHUT_WR 0x0001
#define SHUT_RDWR 0x0002

#define PF_INET AF_INET
#define PF_INET6 AF_INET6

#define SOCKET_FD_MASK 0x40000000

#ifdef __cplusplus
extern "C" {
#endif

int accept(int socket, struct sockaddr* address, socklen_t* address_len);
int bind(int socket, const struct sockaddr* address, socklen_t address_len);
int connect(int socket, const struct sockaddr* address, socklen_t address_len);
int getpeername(int socket, struct sockaddr* address, socklen_t* address_len);
int getsockname(int socket, struct sockaddr* address, socklen_t* address_len);
int getsockopt(int socket, int level, int option_name, void* option_value,socklen_t* option_len);
int listen(int socket, int backlog);
ssize_t recv(int socket, void* buffer, size_t length, int flags);
ssize_t recvfrom(int socket, void* buffer, size_t length, int flags,struct sockaddr* from, socklen_t* fromlen);
ssize_t recvmsg(int socket, struct msghdr* message, int flags);
ssize_t send(int socket, const void* message, size_t length, int flags);
ssize_t sendto(int socket, const void* message, size_t length, int flags,const struct sockaddr* dest_addr, socklen_t dest_len);
ssize_t sendmsg(int socket, const struct msghdr* message, int flags);
int setsockopt(int socket, int level, int option_name, const void* option_value,socklen_t option_len);
int shutdown(int socket, int how);
int socket(int domain, int type, int protocol);
int socketpair(int domain, int type, int protocol, int socket_vector[2]);

#ifdef __cplusplus
	}
#endif

#endif
