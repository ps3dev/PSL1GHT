#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <socket.h>
#include <sys/lv2errno.h>

#include <sys/tty.h>
#include <sys/file.h>

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

		ret = sysTtyWrite(fd,ptr,len,&done);
		if(ret) (_ssize_t)lv2errno_r(r,ret);

		return (_ssize_t)done;
	} else {
		u64 done = 0;

		ret = sysLv2FsWrite(fd,ptr,len,&done);
		if(ret) (_ssize_t)lv2errno_r(r,ret);

		return (_ssize_t)done;
	}

	return (_ssize_t)-1;
}
