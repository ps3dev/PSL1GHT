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
_DEFUN(__librt_link_r,(r,old,new),
	   struct _reent *r _AND
	   const char *old _AND
	   const char *new)
{
	return lv2errno_r(r,sysLv2FsLink(old,new));
}
