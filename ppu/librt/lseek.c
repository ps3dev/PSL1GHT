#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

_off_t __librt_lseek_r(struct _reent *r, int fd, _off_t pos, int dir)
{
	s32 ret = 0;
	u64 position = 0;

	ret = sysLv2FsLSeek64(fd,pos,dir,&position);
	if(ret) return (_off_t)lv2errno_r(r,ret);

	return (_off_t)position;
}

_off64_t __librt_lseek64_r(struct _reent *r, int fd, _off64_t pos, int dir)
{
	s32 ret = 0;
	u64 position = 0;

	ret = sysLv2FsLSeek64(fd,pos,dir,&position);
	if(ret) return (_off64_t)lv2errno_r(r,ret);

	return (_off64_t)position;
}
