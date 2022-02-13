#include <stdio.h>
#include <fcntl.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

int __librt_truncate_r(struct _reent *r, const char *path, off_t len)
{
	return lv2errno_r(r,sysLv2FsTruncate(path,len));
}

int __librt_ftruncate_r(struct _reent *r, int fd, off_t len)
{
	return lv2errno_r(r,sysLv2FsFtruncate(fd,len));
}
