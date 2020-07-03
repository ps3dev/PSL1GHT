#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <socket.h>
#include <select.h>
#include <sysutil/sysnet.h>
#include <sys/lv2errno.h>

#define FD(socket)			((socket)&~SOCKET_FD_MASK)

int __attribute__((weak)) accept(int socket, struct sockaddr* address, socklen_t* address_len)
{
	s32 ret = 0;
	socklen_t len;
	socklen_t *lenp = (address && address_len) ? &len : NULL;

	ret = sysNetAccept(FD(socket),address,lenp);
	if(ret<0) return lv2errno(ret);

	if(lenp)
		*address_len = len;

	return (ret | SOCKET_FD_MASK);
}

int __attribute__((weak)) bind(int socket, const struct sockaddr* address, socklen_t address_len)
{
	return lv2errno(sysNetBind(FD(socket),address,address_len));
}

int __attribute__((weak)) connect(int socket, const struct sockaddr* address, socklen_t address_len)
{
	return lv2errno(sysNetConnect(FD(socket),address,address_len));
}

int __attribute__((weak)) listen(int socket, int backlog)
{
	return lv2errno(sysNetListen(FD(socket),backlog));
}

int __attribute__((weak)) socket(int domain, int type, int protocol)
{
	s32 ret = sysNetSocket(domain,type,protocol);
	if(ret<0) return lv2errno(ret);

	return (ret | SOCKET_FD_MASK);
}

ssize_t __attribute__((weak)) send(int socket, const void* message, size_t length, int flags)
{
	return (ssize_t)lv2errno(sysNetSendto(FD(socket),message,length,flags,NULL,0));
}

ssize_t __attribute__((weak)) sendto(int socket, const void* message, size_t length, int flags,const struct sockaddr* dest_addr, socklen_t dest_len)
{
	return (ssize_t)lv2errno(sysNetSendto(FD(socket),message,length,flags,dest_addr,dest_len));
}

ssize_t __attribute__((weak)) recv(int socket, void* buffer, size_t length, int flags)
{
	return (ssize_t)lv2errno(sysNetRecvfrom(FD(socket),buffer,length,flags,NULL,NULL));
}

ssize_t __attribute__((weak)) recvfrom(int socket, void* buffer, size_t length, int flags,struct sockaddr* from, socklen_t* fromlen)
{
	s32 ret = 0;
	socklen_t len;
	socklen_t *lenp = (from && fromlen) ? &len : NULL;

	ret = sysNetRecvfrom(FD(socket),buffer,length,flags,from,lenp);
	if(ret<0) return (ssize_t)lv2errno(ret);

	if(lenp)
		*fromlen = len;

	return (ssize_t)ret;
}

int __attribute__((weak)) shutdown(int socket,int how)
{
	return lv2errno(sysNetShutdown(FD(socket),how));
}

int __attribute__((weak)) inet_aton(const char* cp, struct in_addr* inp)
{
	unsigned int num1;
	unsigned int num2;
	unsigned int num3;
	unsigned int num4;
	if (sscanf(cp, "%u.%u.%u.%u", &num1, &num2, &num3, &num4) != 4)
		return 0;
	if ((num1 | num2 | num3 | num4) & 0xFFFFFF00)
		return 0;

	inp->s_addr = htonl((num1 << 24) | (num2 << 16) | (num3 << 8) | num4);

	return 1;
}

int __attribute__((weak)) inet_pton(int af, const char* src, void* dst)
{
	switch (af) {
		case AF_INET:
			return inet_aton(src, (struct in_addr*)dst);
	case AF_INET6:
		break;
	}

	return 1;
}

int __attribute__((weak)) select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds, struct timeval* timeout)
{
	errno = ENOSYS;
	return -1;
}

int __attribute__((weak)) closesocket(int socket)
{
	return lv2errno(sysNetClose(FD(socket)));
}

