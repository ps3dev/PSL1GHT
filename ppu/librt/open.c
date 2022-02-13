#include <stdio.h>
#include <fcntl.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

#define UMASK(mode)		((mode)&~g_umask)

extern mode_t g_umask;

int __librt_open_r(struct _reent *r, const char *file, int flags, int mode)
{
	int fd = -1;

	int oflag = flags&(O_ACCMODE|SYS_O_MSELF);
	if(flags&O_CREAT)
		oflag |= SYS_O_CREAT;
	if(flags&O_TRUNC)
		oflag |= SYS_O_TRUNC;
	if(flags&O_EXCL)
		oflag |= SYS_O_EXCL;
	if(flags&O_APPEND)
		oflag |= SYS_O_APPEND;

	if(flags&O_CREAT)
		mode = UMASK(mode);
	else
		mode = 0;

	int ret = sysLv2FsOpen(file,oflag,&fd,mode,NULL,0);
	if(ret) return lv2errno_r(r,ret);

	if(flags&O_CREAT)
		sysLv2FsChmod(file,mode);

	return fd;
}
