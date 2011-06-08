#ifndef __LV2_SYSFS_H__
#define __LV2_SYSFS_H__

#include <time.h>
#include <unistd.h>
#include <ppu-types.h>

// Open Flags
#define SYS_O_RDONLY		000000
#define SYS_O_WRONLY		000001
#define SYS_O_RDWR			000002
#define SYS_O_ACCMODE		000003
#define SYS_O_CREAT			000100
#define SYS_O_EXCL			000200
#define SYS_O_TRUNC			001000
#define SYS_O_APPEND		002000
#define SYS_O_MSELF			010000

#define SYS_FS_IO_BUFFER_PAGE_SIZE_64KB 2
#define SYS_FS_IO_BUFFER_PAGE_SIZE_1MB  4

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct _sys_fs_stat
{
	s32 st_mode;
	s32 st_uid;
	s32 st_gid;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	u64 st_size;
	u64 st_blksize;
} __attribute__((packed)) sysFSStat;

typedef struct _sys_fs_dirent
{
	u8 d_type;
	u8 d_namlen;
	char d_name[MAXPATHLEN + 1];
} sysFSDirent;

typedef struct _sys_fs_utime
{
	time_t actime;
	time_t modtime;
} sysFSUtimbuf;

typedef struct _sys_fs_aio
{
	s32 fd;
	u64 offset;
	u32 buffer_addr;
	u64 size;
	u64 usrdata;
} sysFSAio;

typedef void (*sysFsAioCallback)(sysFSAio *aio,s32 error,s32 xid,u64 size);

s32 sysFsOpen(const char *path,s32 oflags,s32 *fd,const void *arg,u64 argsize);
s32 sysFsClose(s32 fd);
s32 sysFsRead(s32 fd,void *ptr,u64 len,u64 *read);
s32 sysFsWrite(s32 fd, const void* ptr, u64 size, u64* written);
s32 sysFsLseek(s32 fd, s64 offset, s32 whence, u64* position);
s32 sysFsStat(const char* path, sysFSStat* stat);
s32 sysFsFstat(s32 fd, sysFSStat* stat);
s32 sysFsChmod(const char* path, s32 mode);
s32 sysFsMkdir(const char* path, s32 mode);
s32 sysFsRmdir(const char *path);
s32 sysFsUnlink(const char *path);

s32 sysFsOpendir(const char *path, s32 *fd);
s32 sysFsClosedir(s32 fd);
s32 sysFsReaddir(s32 fd, sysFSDirent* entry, u64* read);

s32 sysFsAioInit(const char * path);
s32 sysFsAioFinish(const char * path);
s32 sysFsAioCancel(s32 id);

s32 sysFsAioRead(sysFSAio *aio, s32 *id, sysFsAioCallback cb);
s32 sysFsAioWrite(sysFSAio *aio, s32 *id, sysFsAioCallback cb);

s32 sysFsGetFreeSize(const char *path, u32 *blockSize, u64 *freeBlocks);

s32 sysFsSetIoBuffer(s32 fd, size_t bufferSizeLimit, s32 pageType, sys_mem_container_t container);
s32 sysFsSetDefaultContainer(sys_mem_container_t container, size_t totalLimit);
s32 sysFsSetIoBufferFromDefaultContainer(s32 fd, size_t bufferSizeLimit, s32 pageType);

#ifdef __cplusplus
	}
#endif

#endif
