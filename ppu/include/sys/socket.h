/*! \file socket.h
 \brief Socket management functions.
*/

#ifndef __SYS_SOCKET_H__
#define __SYS_SOCKET_H__

#include <ppu-lv2.h>
#include <net/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

LV2_SYSCALL sysNetAccept(int socket,const struct sockaddr *addr,socklen_t *addr_len)
{
	lv2syscall3(700,socket,(u64)addr,(u64)addr_len);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetBind(int socket,const struct sockaddr *addr,socklen_t addr_len)
{
	lv2syscall3(701,socket,(u64)addr,addr_len);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetConnect(int socket,const struct sockaddr *addr,socklen_t addr_len)
{
	lv2syscall3(702,socket,(u64)addr,addr_len);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetListen(int socket,int backlog)
{
	lv2syscall2(706,socket,backlog);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetRecvfrom(int socket,void *buffer,size_t len,int flags,const struct sockaddr *addr,socklen_t *addr_len)
{
	lv2syscall6(707,socket,(u64)buffer,len,flags,(u64)addr,(u64)addr_len);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetSendto(int socket,const void *message,size_t len,int flags,const struct sockaddr *addr,socklen_t addr_len)
{
	lv2syscall6(710,socket,(u64)message,len,flags,(u64)addr,addr_len);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetShutdown(int socket,int how)
{
	lv2syscall2(712,socket,how);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetSocket(int domain,int type,int protocol)
{
	lv2syscall3(713,domain,type,protocol);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysNetClose(int socket)
{
	lv2syscall1(714,socket);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
