#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

int
_DEFUN(__librt_fsync_r,(ptr,fd),
	   struct _reent *ptr _AND
		   int fd)
{
	return lv2errno_r(ptr,sysLv2FsFsync(fd));
}
