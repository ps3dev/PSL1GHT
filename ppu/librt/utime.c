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

int __librt_utime_r(struct _reent *r, const char *path, const struct utimbuf *times)
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