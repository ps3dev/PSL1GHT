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
_DEFUN(__librt_gettod_r,(r,ptimeval,ptimezone),
	   struct _reent *r _AND
	   struct timeval *ptimeval _AND
	   void *ptimezone)
{
	s32 ret;
	u64 sec,nsec;
	
	ret = sysGetCurrentTime(&sec,&nsec);
	if(ret) return lv2errno_r(r,ret);

	ptimeval->tv_sec = sec;
	ptimeval->tv_usec = nsec/1000;

	return 0;
}
