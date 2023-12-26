#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <net/socket.h>
#include <sys/lv2errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/tty.h>
#include <sys/file.h>

#include "../../configure.h"

#ifdef _STDIO_TO_NET
//optionally initialize directing stdio to ethernet
static int _stdio_sock;
static struct sockaddr_in _stdio_saddr;
__attribute__((constructor(1000)))
void init_stdio_to_net(void)
{
        // TODO: can netInitialize be run twice?
        // if not - could it be run implicitly by the crt?
        //netInitialize();

        // Connect the socket, hoping for the best.
        // We have little means of notifying the user if it fails.
        _stdio_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(&_stdio_saddr, 0, sizeof(_stdio_saddr));
        _stdio_saddr.sin_len = sizeof(_stdio_saddr);
        _stdio_saddr.sin_family = AF_INET;
        inet_pton(AF_INET, _STDIO_NET_IP, &_stdio_saddr.sin_addr);
        _stdio_saddr.sin_port = htons(_STDIO_NET_PORT);
        connect(_stdio_sock,(struct sockaddr*)&_stdio_saddr, \
                            sizeof(_stdio_saddr));
}
#endif


_ssize_t
_DEFUN(__librt_write_r,(r,fd,ptr,len),
	   struct _reent *r _AND
		   int fd _AND
		   const void *ptr _AND
		   size_t len)
{
	int ret = 0;

	if(fd&SOCKET_FD_MASK)
		return send(fd,ptr,len,0);

	if(fd==STDOUT_FILENO || fd==STDERR_FILENO) {
		u32 done = 0;
		
		#ifndef _STDIO_TO_NET
		ret = sysTtyWrite(fd,ptr,len,&done);
		if(ret) (_ssize_t)lv2errno_r(r,ret);
		#else
		done=write(_stdio_sock, ptr, len);
		if(ret==-1) (_ssize_t)lv2errno_r(r,ret);
		#endif
		return (_ssize_t)done;
	} else {
		u64 done = 0;

		ret = sysLv2FsWrite(fd,ptr,len,&done);
		if(ret) (_ssize_t)lv2errno_r(r,ret);

		return (_ssize_t)done;
	}

	return (_ssize_t)-1;
}
