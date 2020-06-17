#include <stdio.h>
#include <fcntl.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>
#include <lv2/sysfs.h>
#include <time.h>
#include <utime.h>

int
_DEFUN(__librt_utime_r,(r,filename,times),
	   struct _reent *r _AND
	   const char *filename _AND
	   const struct utimbuf *times)
{
	if (times == NULL) {
		struct utimbuf now_time;
		time_t     now;
		now = time(NULL);
		now_time.actime = now;
		now_time.modtime = now;
		return lv2errno_r(r,sysLv2FsUtime(filename,(const struct sysFSUtimbuf *)&now_time));
	}
	return lv2errno_r(r,sysLv2FsUtime(filename,(const struct sysFSUtimbuf *)times));
}