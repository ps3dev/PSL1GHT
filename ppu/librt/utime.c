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
_DEFUN(__librt_utime_r,(r,path,times),
	   struct _reent *r _AND
	   const char *path _AND
	   const struct utimbuf *times)
{
	sysFSUtimbuf t;
	if (times)
	{
		t.actime = times->actime;
		t.modtime = times->modtime;
	}
	else
	{
		time_t now = time(NULL);
		t.actime = now;
		t.modtime = now;
	}

	return lv2errno_r(r,sysLv2FsUtime(path,&t));
}