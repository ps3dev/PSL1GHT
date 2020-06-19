#include <stdio.h>
#include <fcntl.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>
#include <utime.h>


int _utime(const char *path, const struct utimbuf *times, int* _errno)
{
	printf("CIAO\n");
if (times == NULL) {
		struct utimbuf now_time;
		time_t     now;
		now = time(NULL);
		now_time.actime = now;
		now_time.modtime = now;
		return lv2errno(sysLv2FsUtime(path,(sysFSUtimbuf *)&now_time));
	}
	else return lv2errno(sysLv2FsUtime(path,(sysFSUtimbuf *)times));
}

int
_DEFUN(__librt_utime_r,(r,path,times),
	   struct _reent *r _AND
	   const char *path _AND
	   const struct utimbuf *times)
{
	return _utime(path,times,&r->_errno);
}