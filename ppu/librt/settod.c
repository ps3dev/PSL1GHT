#include <stdio.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/lv2errno.h>

#include <sys/systime.h>

int
_DEFUN(__librt_settod_r,(r,ptimeval,ptimezone),
	   struct _reent *r _AND
	   const struct timeval *ptimeval _AND
	   const struct timezone *ptimezone)
{
	u64 sec,nsec;

	sec = ptimeval->tv_sec;
	nsec = ptimeval->tv_usec*1000;

	return lv2errno_r(r,sysSetCurrentTime(sec,nsec));
}
