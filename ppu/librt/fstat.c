#include <_ansi.h>
#include <_syslist.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

static void convertLv2Stat(struct stat *dst,CellFsStat *src)
{
	memset(dst,0,sizeof(struct stat));
	dst->st_mode = src->st_mode;
	dst->st_uid = src->st_uid;
	dst->st_gid = src->st_gid;
	dst->st_atime = src->st_atime;
	dst->st_mtime = src->st_mtime;
	dst->st_ctime = src->st_ctime;
	dst->st_size = src->st_size;
	dst->st_blksize = src->st_blksize;

}

int
_DEFUN(__librt_fstat_r,(r,fd,st),
	   struct _reent *r _AND
	   int fd _AND
	   struct stat *st)
{
	s32 ret = 0;
	CellFsStat stat;

	ret = sysLv2FsFStat(fd,&stat);
	if(!ret && st) convertLv2Stat(st,&stat);
	
	return lv2errno_r(r,ret);
}

int
_DEFUN(__librt_fstat64_r,(r,fd,st),
	   struct _reent *r _AND
	   int fd _AND
	   struct stat *st)
{
	s32 ret = 0;
	CellFsStat stat;

	ret = sysLv2FsFStat(fd,&stat);
	if(!ret && st) convertLv2Stat(st,&stat);
	
	return lv2errno_r(r,ret);
}

int
_DEFUN(__librt_stat_r,(r,path,st),
	   struct _reent *r _AND
	   const char *path _AND
	   struct stat *st)
{
	s32 ret = 0;
	CellFsStat stat;

	ret = sysLv2FsStat(path,&stat);
	if(!ret && st) convertLv2Stat(st,&stat);

	return lv2errno_r(r,ret);
}

int
_DEFUN(__librt_stat64_r,(r,path,st),
	   struct _reent *r _AND
	   const char *path _AND
	   struct stat *st)
{
	s32 ret = 0;
	CellFsStat stat;

	ret = sysLv2FsStat(path,&stat);
	if(!ret && st) convertLv2Stat(st,&stat);

	return lv2errno_r(r,ret);
}
