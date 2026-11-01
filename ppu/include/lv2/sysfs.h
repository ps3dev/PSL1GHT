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
} __attribute__((packed)) CellFsStat;

typedef struct _sys_fs_dirent
{
	u8 d_type;
	u8 d_namlen;
	char d_name[MAXPATHLEN + 1];
} CellFsDirent;

typedef struct _sys_fs_directory_entry
{
	CellFsStat attribute;
	CellFsDirent entry_name;
} CellFsDirectoryEntry;

typedef struct _sys_fs_utime
{
	time_t actime;
	time_t modtime;
} CellFsUtimbuf;

typedef struct _sys_fs_aio
{
	s32 fd;
	u64 offset;
	u32 buffer_addr;
	u64 size;
	u64 usrdata;
} CellFsAio;

typedef void (*CellFsAioCallback)(CellFsAio *aio,s32 error,s32 xid,u64 size);

s32 cellFsOpen(const char *path,s32 oflags,s32 *fd,const void *arg,u64 argsize);
s32 cellFsClose(s32 fd);
s32 cellFsRead(s32 fd,void *ptr,u64 len,u64 *read);
s32 cellFsWrite(s32 fd, const void* ptr, u64 size, u64* written);
s32 cellFsLseek(s32 fd, s64 offset, s32 whence, u64* position);
s32 cellFsStat(const char* path, CellFsStat* stat);
s32 cellFsFstat(s32 fd, CellFsStat* stat);
s32 cellFsChmod(const char* path, s32 mode);
s32 cellFsMkdir(const char* path, s32 mode);
s32 cellFsRmdir(const char *path);
s32 cellFsUnlink(const char *path);
s32 cellFsAccess(const char *path,s32 amode);
s32 cellFsUtime(const char *path, CellFsUtimbuf *times);

s32 cellFsOpendir(const char *path, s32 *fd);
s32 cellFsClosedir(s32 fd);
s32 cellFsReaddir(s32 fd, CellFsDirent* entry, u64* read);

s32 cellFsAioInit(const char * path);
s32 cellFsAioFinish(const char * path);
s32 cellFsAioCancel(s32 id);

s32 cellFsAioRead(CellFsAio *aio, s32 *id, CellFsAioCallback cb);
s32 cellFsAioWrite(CellFsAio *aio, s32 *id, CellFsAioCallback cb);

s32 cellFsGetFreeSize(const char *path, u32 *blockSize, u64 *freeBlocks);
s32 cellFsGetDirectoryEntries(s32 fd, CellFsDirectoryEntry *entries, u32 entrySize, u32 *dataCount);

s32 cellFsSetIoBuffer(s32 fd, size_t bufferSizeLimit, s32 pageType, sys_mem_container_t container);
s32 cellFsSetDefaultContainer(sys_mem_container_t container, size_t totalLimit);
s32 cellFsSetIoBufferFromDefaultContainer(s32 fd, size_t bufferSizeLimit, s32 pageType);

#ifdef __cplusplus
	}
#endif

#endif
