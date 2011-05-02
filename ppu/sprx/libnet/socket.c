#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __LINUX_ERRNO_EXTENSIONS__
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/net.h>
#include <net/netdb.h>
#include <net/socket.h>
#include <net/select.h>

#include <sys/lv2errno.h>

#define MAX_HOST_NAMES		32

#define LIBNET_INITIALZED	(__netMemory)
#define FD(socket)			((socket)&~SOCKET_FD_MASK)

int h_errno = 0;

static struct hostent host;
static char *hostaliases[MAX_HOST_NAMES];
static char *hostaddrlist[MAX_HOST_NAMES];

extern void *__netMemory;

const static int neterrno2errno[] = {
	[NET_EPERM]				= EPERM,
	[NET_ENOENT]			= ENOENT,
	[NET_ESRCH]				= ESRCH,
	[NET_EINTR]				= EINTR,
	[NET_EIO]				= EIO,
	[NET_ENXIO]				= ENXIO,
	[NET_E2BIG]				= E2BIG,
	[NET_ENOEXEC]			= ENOEXEC,
	[NET_EBADF]				= EBADF,
	[NET_ECHILD]			= ECHILD,
	[NET_EDEADLK]			= EDEADLK,
	[NET_ENOMEM]			= ENOMEM,
	[NET_EACCES]			= EACCES,
	[NET_EFAULT]			= EFAULT,
	[NET_ENOTBLK]			= ENOTBLK,
	[NET_EBUSY]				= EBUSY,
	[NET_EEXIST]			= EEXIST,
	[NET_EXDEV]				= EXDEV,
	[NET_ENODEV]			= ENODEV,
	[NET_ENOTDIR]			= ENOTDIR,
	[NET_EISDIR]			= EISDIR,
	[NET_EINVAL]			= EINVAL,
	[NET_ENFILE]			= ENFILE,
	[NET_EMFILE]			= EMFILE,
	[NET_ENOTTY]			= ENOTTY,
	[NET_ETXTBSY]			= ETXTBSY,
	[NET_EFBIG]				= EFBIG,
	[NET_ENOSPC]			= ENOSPC,
	[NET_ESPIPE]			= ESPIPE,
	[NET_EROFS]				= EROFS,
	[NET_EMLINK]			= EMLINK,
	[NET_EPIPE]				= EPIPE,
	[NET_EDOM]				= EDOM,
	[NET_ERANGE]			= ERANGE,
	[NET_EAGAIN]			= EAGAIN,
	[NET_EWOULDBLOCK]		= EWOULDBLOCK,
	[NET_EINPROGRESS]		= EINPROGRESS,
	[NET_EALREADY]			= EALREADY,
	[NET_ENOTSOCK]			= ENOTSOCK,
	[NET_EDESTADDRREQ]		= EDESTADDRREQ,
	[NET_EMSGSIZE]			= EMSGSIZE,
	[NET_EPROTOTYPE]		= EPROTOTYPE,
	[NET_ENOPROTOOPT]		= ENOPROTOOPT,
	[NET_EPROTONOSUPPORT]	= EPROTONOSUPPORT,
	[NET_ESOCKTNOSUPPORT]	= ESOCKTNOSUPPORT,
	[NET_EOPNOTSUPP]		= EOPNOTSUPP,
	[NET_EPFNOSUPPORT]		= EPFNOSUPPORT,
	[NET_EAFNOSUPPORT]		= EAFNOSUPPORT,
	[NET_EADDRINUSE]		= EADDRINUSE,
	[NET_EADDRNOTAVAIL]		= EADDRNOTAVAIL,
	[NET_ENETDOWN]			= ENETDOWN,
	[NET_ENETUNREACH]		= ENETUNREACH,
	[NET_ENETRESET]			= ENETRESET,
	[NET_ECONNABORTED]		= ECONNABORTED,
	[NET_ECONNRESET]		= ECONNRESET,
	[NET_ENOBUFS]			= ENOBUFS,
	[NET_EISCONN]			= EISCONN,
	[NET_ENOTCONN]			= ENOTCONN,
	[NET_ESHUTDOWN]			= ESHUTDOWN,
	[NET_ETOOMANYREFS]		= ETOOMANYREFS,
	[NET_ETIMEDOUT]			= ETIMEDOUT,
	[NET_ECONNREFUSED]		= ECONNREFUSED,
	[NET_ELOOP]				= ELOOP,
	[NET_ENAMETOOLONG]		= ENAMETOOLONG,
	[NET_EHOSTDOWN]			= EHOSTDOWN,
	[NET_EHOSTUNREACH]		= EHOSTUNREACH,
	[NET_ENOTEMPTY]			= ENOTEMPTY,
	[NET_EPROCLIM]			= EPROCLIM,
	[NET_EUSERS]			= EUSERS,
	[NET_EDQUOT]			= EDQUOT,
	[NET_ESTALE]			= ESTALE,
	[NET_EREMOTE]			= EREMOTE,
	[NET_EBADRPC]			= ENOTSUP, // no match
	[NET_ERPCMISMATCH]		= ENOTSUP, // no match
	[NET_EPROGUNAVAIL]		= ENOTSUP, // no match
	[NET_EPROGMISMATCH]		= ENOTSUP, // no match
	[NET_EPROCUNAVAIL]		= ENOTSUP, // no match
	[NET_ENOLCK]			= ENOLCK,
	[NET_ENOSYS]			= ENOSYS,
	[NET_EFTYPE]			= EFTYPE,
	[NET_EAUTH]				= EPERM, // no match
	[NET_ENEEDAUTH]			= EPERM, // no match
	[NET_EIDRM]				= EIDRM,
	[NET_ENOMSG]			= ENOMSG,
	[NET_EOVERFLOW]			= EOVERFLOW,
	[NET_EILSEQ]			= EILSEQ,
	[NET_ENOTSUP]			= ENOTSUP,
	[NET_ECANCELED]			= ECANCELED,
	[NET_EBADMSG]			= EBADMSG,
	[NET_ENODATA]			= ENODATA,
	[NET_ENOSR]				= ENOSR,
	[NET_ENOSTR]			= ENOSTR,
	[NET_ETIME]				= ETIME
};

static int netErrno(int ret)
{
	if(ret>=0) return ret;

	if(net_errno<(sizeof(neterrno2errno)/sizeof(neterrno2errno[0])))
		errno = neterrno2errno[net_errno] ?: ENOTSUP;
	else
		errno = ENOTSUP;

	return -1;
}

static struct hostent* copyhost(struct net_hostent *nethost)
{
	s32 i;

	if(!nethost) return NULL;

	memset(&host,0,sizeof(struct hostent));

	host.h_name = (char*)((u64)nethost->h_name);
	host.h_addrtype = nethost->h_addrtype;
	host.h_length = nethost->h_length;
	host.h_aliases = hostaliases;
	host.h_addr_list = hostaddrlist;

	u32 *netaddrlist = (u32*)((u64)nethost->h_addr_list);
	u32 *netaliases = (u32*)((u64)nethost->h_aliases);
	for(i=0;i<MAX_HOST_NAMES;i++) {
		host.h_addr_list[i] = (char*)((u64)netaddrlist[i]);
		
		if(!netaddrlist[i]) break;
	}

	for(i=0;i<MAX_HOST_NAMES;i++) {
		host.h_aliases[i] = (char*)((u64)netaliases[i]);
		
		if(!netaliases[i]) break;
	}
	
	return &host;
}

int accept(int socket,struct sockaddr* address,socklen_t* address_len)
{
	s32 ret = 0;
	socklen_t len = address_len ? *address_len : 0;
	socklen_t *lenp = (address && address_len) ? &len : NULL;

	if(LIBNET_INITIALZED) {
		ret = netAccept(FD(socket),address,lenp);

		if(ret<0)
			return netErrno(ret);

		if(lenp)
			*address_len = len;

		return (ret | SOCKET_FD_MASK);
	}

	errno = ENOSYS;
	return -1;
}

int bind(int socket,const struct sockaddr* address,socklen_t address_len)
{
	if(LIBNET_INITIALZED)
		return netErrno(netBind(FD(socket),address,address_len));

	errno = ENOSYS;
	return -1;
}

int connect(int socket,const struct sockaddr* address,socklen_t address_len)
{
	if(LIBNET_INITIALZED)
		return netErrno(netConnect(FD(socket),address,address_len));

	errno = ENOSYS;
	return -1;
}

int listen(int socket,int backlog)
{
	if(LIBNET_INITIALZED)
		return netErrno(netListen(FD(socket),backlog));

	errno = ENOSYS;
	return -1;
}

int socket(int domain,int type,int protocol)
{
	s32 ret = 0;

	if(LIBNET_INITIALZED) {
		ret = netSocket(domain,type,protocol);

		if(ret<0) return netErrno(ret);

		return (ret | SOCKET_FD_MASK);
	}

	errno = ENOSYS;
	return -1;
}

ssize_t send(int socket,const void *buffer,size_t len,int flags)
{
	if(LIBNET_INITIALZED)
		return (ssize_t)netErrno(netSend(FD(socket),buffer,len,flags));

	errno = ENOSYS;
	return (ssize_t)-1;
}

ssize_t sendto(int socket,const void *buffer,size_t len,int flags,const struct sockaddr* dest_addr,socklen_t dest_len)
{
	if(LIBNET_INITIALZED)
		return (ssize_t)netErrno(netSendTo(FD(socket),buffer,len,flags,dest_addr,dest_len));

	errno = ENOSYS;
	return (ssize_t)-1;
}

ssize_t recv(int socket,void *buffer,size_t len,int flags)
{
	if(LIBNET_INITIALZED)
		return (ssize_t)netErrno(netRecv(FD(socket),buffer,len,flags));

	errno = ENOSYS;
	return (ssize_t)-1;
}

ssize_t recvfrom(int socket,void *buffer,size_t len,int flags,struct sockaddr* from,socklen_t *fromlen)
{
	s32 ret = 0;
	socklen_t len_;
	socklen_t *lenp = (from && fromlen) ? &len_ : NULL;

	if(LIBNET_INITIALZED) {
		ret = netRecvFrom(FD(socket),buffer,len,flags,from,lenp);

		if(ret<0)
			return netErrno(ret);

		if(lenp)
			*fromlen = len_;

		return (ssize_t)ret;
	}

	errno = ENOSYS;
	return (ssize_t)-1;
}

int shutdown(int socket,int how)
{
	if(LIBNET_INITIALZED)
		return netErrno(netShutdown(FD(socket),how));

	errno = ENOSYS;
	return -1;
}

int closesocket(int socket)
{
	if(LIBNET_INITIALZED)
		return netErrno(netClose(FD(socket)));

	errno = ENOSYS;
	return -1;
}

int poll(struct pollfd fds[],nfds_t nfds,int timeout)
{
	if(LIBNET_INITIALZED)
		return netErrno(netPoll(fds,nfds,timeout));

	errno = ENOSYS;
	return -1;
}

int select(int nfds,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout)
{
	if(LIBNET_INITIALZED)
		return netErrno(netSelect(FD(nfds),readfds,writefds,errorfds,timeout));

	errno = ENOSYS;
	return -1;
}

int getsockname(int socket,struct sockaddr* address,socklen_t *address_len)
{
	s32 ret = 0;
	socklen_t len = address_len ? *address_len : 0;
	socklen_t *lenp = (address && address_len) ? &len : NULL;

	if(LIBNET_INITIALZED) {
		ret = netGetSockName(FD(socket),address,lenp);
		
		if(ret<0)
			return netErrno(ret);

		if(lenp)
			*address_len = len;

		return ret;
	}

	errno = ENOSYS;
	return -1;
}

int getpeername(int socket,struct sockaddr* address,socklen_t *address_len)
{
	s32 ret = 0;
	socklen_t len = address_len ? *address_len : 0;
	socklen_t *lenp = (address && address_len) ? &len : NULL;

	if(LIBNET_INITIALZED) {
		ret = netGetPeerName(FD(socket),address,lenp);
		
		if(ret<0)
			return netErrno(ret);

		if(lenp)
			*address_len = len;

		return ret;
	}

	errno = ENOSYS;
	return -1;
}

struct hostent* gethostbyaddr(const char *addr,socklen_t len,int type)
{
	if(!LIBNET_INITIALZED) {
		errno = ENOSYS;
		h_errno = TRY_AGAIN;
		return NULL;
	}

	struct net_hostent *ret = netGetHostByAddr(addr,len,type);
	if(!ret) h_errno = net_h_errno;

	return copyhost(ret);
}

struct hostent* gethostbyname(const char *name)
{
	if(!LIBNET_INITIALZED) {
		errno = ENOSYS;
		h_errno = TRY_AGAIN;
		return NULL;
	}

	struct net_hostent *ret = netGetHostByName(name);
	if(!ret) h_errno = net_h_errno;

	return copyhost(ret);
}

int getsockopt(int socket, int level, int option_name, void* option_value,socklen_t* option_len)
{
	if(!LIBNET_INITIALZED) {
		errno = ENOSYS;
		h_errno = TRY_AGAIN;
		return -1;
	}
	return netErrno(netGetSockOpt(FD(socket),level,option_name,option_value,option_len));
}

int setsockopt(int socket, int level, int option_name, const void* option_value,socklen_t option_len)
{
	if(!LIBNET_INITIALZED) {
		errno = ENOSYS;
		h_errno = TRY_AGAIN;
		return -1;
	}
	return netErrno(netSetSockOpt(FD(socket),level,option_name,option_value,option_len));
}
