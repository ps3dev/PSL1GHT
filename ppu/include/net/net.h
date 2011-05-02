#ifndef __LV2_NET_H__
#define __LV2_NET_H__

#include <net/socket.h>
#include <net/select.h>
#include <net/poll.h>

#define	NET_EPERM			1
#define	NET_ENOENT			2
#define	NET_ESRCH			3
#define	NET_EINTR			4
#define	NET_EIO				5
#define	NET_ENXIO			6
#define	NET_E2BIG			7
#define	NET_ENOEXEC			8
#define	NET_EBADF			9
#define	NET_ECHILD			10
#define	NET_EDEADLK			11
#define	NET_ENOMEM			12
#define	NET_EACCES			13
#define	NET_EFAULT			14
#define	NET_ENOTBLK			15
#define	NET_EBUSY			16
#define	NET_EEXIST			17
#define	NET_EXDEV			18
#define	NET_ENODEV			19
#define	NET_ENOTDIR			20
#define	NET_EISDIR			21
#define	NET_EINVAL			22
#define	NET_ENFILE			23
#define	NET_EMFILE			24
#define	NET_ENOTTY			25
#define	NET_ETXTBSY			26
#define	NET_EFBIG			27
#define	NET_ENOSPC			28
#define	NET_ESPIPE			29
#define	NET_EROFS			30
#define	NET_EMLINK			31
#define	NET_EPIPE			32
#define	NET_EDOM			33
#define	NET_ERANGE			34
#define	NET_EAGAIN			35
#define	NET_EWOULDBLOCK		NET_EAGAIN
#define	NET_EINPROGRESS		36
#define	NET_EALREADY		37
#define	NET_ENOTSOCK		38
#define	NET_EDESTADDRREQ	39
#define	NET_EMSGSIZE		40
#define	NET_EPROTOTYPE		41
#define	NET_ENOPROTOOPT		42
#define	NET_EPROTONOSUPPORT	43
#define	NET_ESOCKTNOSUPPORT	44
#define	NET_EOPNOTSUPP		45
#define	NET_EPFNOSUPPORT	46
#define	NET_EAFNOSUPPORT	47
#define	NET_EADDRINUSE		48
#define	NET_EADDRNOTAVAIL	49
#define	NET_ENETDOWN		50
#define	NET_ENETUNREACH		51
#define	NET_ENETRESET		52
#define	NET_ECONNABORTED	53
#define	NET_ECONNRESET		54
#define	NET_ENOBUFS			55
#define	NET_EISCONN			56
#define	NET_ENOTCONN		57
#define	NET_ESHUTDOWN		58
#define	NET_ETOOMANYREFS	59
#define	NET_ETIMEDOUT		60
#define	NET_ECONNREFUSED	61
#define	NET_ELOOP			62
#define	NET_ENAMETOOLONG	63
#define	NET_EHOSTDOWN		64
#define	NET_EHOSTUNREACH	65
#define	NET_ENOTEMPTY		66
#define	NET_EPROCLIM		67
#define	NET_EUSERS			68
#define	NET_EDQUOT			69
#define	NET_ESTALE			70
#define	NET_EREMOTE			71
#define	NET_EBADRPC			72
#define	NET_ERPCMISMATCH	73
#define	NET_EPROGUNAVAIL	74
#define	NET_EPROGMISMATCH	75
#define	NET_EPROCUNAVAIL	76
#define	NET_ENOLCK			77
#define	NET_ENOSYS			78
#define	NET_EFTYPE			79
#define	NET_EAUTH			80
#define	NET_ENEEDAUTH		81
#define	NET_EIDRM			82
#define	NET_ENOMSG			83
#define	NET_EOVERFLOW		84
#define	NET_EILSEQ			85
#define	NET_ENOTSUP			86
#define	NET_ECANCELED		87
#define	NET_EBADMSG			88
#define	NET_ENODATA			89
#define	NET_ENOSR			90
#define	NET_ENOSTR			91
#define	NET_ETIME			92
#define	NET_ELAST			92

#define net_errno			(*netErrnoLoc())
#define net_h_errno			(*netHErrnoLoc())

#ifdef __cplusplus
extern "C" {
#endif

struct net_msghdr
{
	u32 _pad0;
	u32 msg_name;
	socklen_t msg_namlen;
	u32 _pad1;
	u32 _pad2;
	u32 msg_iov;
	s32 msg_iovlen;
	u32 _pad3;
	u32 _pad4;
	u32 msg_control;
	socklen_t msg_controllen;
	s32 msg_flags;
};

struct net_hostent
{
	u32 h_name;
	u32 h_aliases;
	s32 h_addrtype;
	s32 h_length;
	u32 h_addr_list;
};

typedef struct _net_init_param
{
	u32 memory;
	u32 memory_size;
	s32 flags;
} netInitParam;

s32 netInitialize();
s32 netDeinitialize();

s32* netErrnoLoc();
s32* netHErrnoLoc();

s32 netInitializeNetworkEx(netInitParam* param);
s32 netFinalizeNetwork();
s32 netShowIfConfig();
s32 netShowNameServer();
s32 netShowRoute();

s32 netAccept(s32 socket,const struct sockaddr* address,socklen_t* address_len);
s32 netBind(s32 socket,const struct sockaddr* address,socklen_t address_len);
s32 netConnect(s32 socket,const struct sockaddr* address,socklen_t address_len);
s32 netListen(s32 socket,s32 backlog);
s32 netSocket(s32 domain,s32 type,s32 protocol);
s32 netClose(s32 socket);
s32 netShutdown(s32 socket,s32 how);

ssize_t netRecv(s32 socket,void *buffer,size_t len,s32 flags);
ssize_t netRecvFrom(s32 socket,void *buffer,size_t len,s32 flags,struct sockaddr* from,socklen_t* fromlen);
ssize_t netRecvMsg(s32 socket,struct net_msghdr *msg,s32 flags);
ssize_t netSend(s32 socket,const void *buffer,size_t len,s32 flags);
ssize_t netSendTo(s32 socket,const void *buffer,size_t len,s32 flags,const struct sockaddr* dest_addr,socklen_t dest_len);
ssize_t netSendMsg(s32 socket,const struct net_msghdr *msg,s32 flags);

s32 netPoll(struct pollfd *fds,nfds_t nfds,s32 timeout);
s32 netSelect(s32 nfds,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
s32 netGetSockName(s32 socket,struct sockaddr *address,socklen_t *address_len);
s32 netGetPeerName(s32 socket,struct sockaddr *address,socklen_t *address_len);

s32 netGetSockOpt(s32 socket,s32 level,s32 option_name,void *option_value,socklen_t *option_len);
s32 netSetSockOpt(s32 socket,s32 level,s32 option_name,const void *option_value,socklen_t option_len);

struct net_hostent* netGetHostByAddr(const char *addr,socklen_t len,s32 type);
struct net_hostent* netGetHostByName(const char *name);

#ifdef __cplusplus
	}
#endif

#endif
