#include <stdio.h>
#include <_ansi.h>
#include <_syslist.h>
#include <unistd.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <ppu-lv2.h>
#include <sys/lv2errno.h>

#include <sys/systime.h>

int
_DEFUN(__librt_usleep_r,(r,usec),
	   struct _reent *r _AND
	   useconds_t usec)
{
	return lv2errno_r(r,sysUsleep(usec));
}

unsigned int
_DEFUN(__librt_sleep_r,(r,seconds),
	   struct _reent *r _AND
	   unsigned int seconds)
{
	s32 ret = sysSleep(seconds);
	if(ret) {
		lv2errno_r(r,ret);
		return seconds;
	}
	return 0;
}

