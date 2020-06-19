#include <stdio.h>
#include <fcntl.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

int
_DEFUN(__librt_truncate_r,(r,path,len),
	   struct _reent *r _AND
	   const char *path _AND
	   off_t len)
{
	return lv2errno_r(r,sysLv2FsTruncate(path,len));
}

int
_DEFUN(__librt_ftruncate_r,(r,fd,len),
	   struct _reent *r _AND
	   int fd _AND
	   off_t len)
{
	return lv2errno_r(r,sysLv2FsFtruncate(fd,len));
}
