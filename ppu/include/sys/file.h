/*! \file file.h
 \brief File management functions.
*/

#ifndef __SYS_FILE_H__
#define __SYS_FILE_H__

#include <ppu-lv2.h>
#include <lv2/sysfs.h>

#ifdef __cplusplus
	extern "C" {
#endif

LV2_SYSCALL sysLv2FsOpen(const char *path,s32 oflags,s32 *fd,u32 mode,const void *arg,u64 argsize)
{
	lv2syscall6(801,(u64)path,oflags,(u64)fd,mode,(u64)arg,argsize);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsClose(s32 fd)
{
	lv2syscall1(804,fd);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsRead(s32 fd,void *ptr,u64 len,u64 *read)
{
	lv2syscall4(802,fd,(u64)ptr,len,(u64)read);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsWrite(s32 fd,const void *ptr,u64 len,u64 *written)
{
	lv2syscall4(803,fd,(u64)ptr,len,(u64)written);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsStat(const char *path,sysFSStat *stat)
{
	lv2syscall2(808,(u64)path,(u64)stat);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsFStat(s32 fd,sysFSStat *stat)
{
	lv2syscall2(809,fd,(u64)stat);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsChmod(const char *path,s32 mode)
{
	lv2syscall2(834,(u64)path,mode);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsOpenDir(const char *path,s32 *fd)
{
	lv2syscall2(805,(u64)path,(u64)fd);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsReadDir(s32 fd,sysFSDirent *entry,u64 *read)
{
	lv2syscall3(806,fd,(u64)entry,(u64)read);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsCloseDir(s32 fd)
{
	lv2syscall1(807,fd);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsMkdir(const char *path,s32 mode)
{
	lv2syscall2(811,(u64)path,mode);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsRename(const char *path,const char *newpath)
{
	lv2syscall2(812,(u64)path,(u64)newpath);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsRmdir(const char *path)
{
	lv2syscall1(813,(u64)path);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsLSeek64(s32 fd,u64 offset,s32 dir,u64 *pos)
{
	lv2syscall4(818,fd,offset,dir,(u64)pos);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsUnlink(const char *path)
{
	lv2syscall1(814,(u64)path);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsFsync(s32 fd)
{
	lv2syscall1(820,fd);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsTruncate(const char *path,u64 size)
{
	lv2syscall2(831,(u64)path,size);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsFtruncate(s32 fd,u64 size)
{
	lv2syscall2(832,fd,size);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsUtime(const char *path,const sysFSUtimbuf *times)
{
	lv2syscall2(815,(u64)path,(u64)times);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysLv2FsLink(const char *oldpath,const char *newpath)
{
	lv2syscall2(810,(u64)oldpath,(u64)newpath);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
