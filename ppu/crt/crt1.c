#include <stdlib.h>
#include <ppu-types.h>
#include <sys/reent.h>
#include <sys/time.h>
#include <sys/dirent.h>
#include <sys/syscalls.h>
#include <utime.h>

extern void _init();

extern void __librt_exit(int rc);
extern int __librt_close_r(struct _reent *ptr,int fd);
extern int __librt_open_r(struct _reent *r,const char *file,int flags,int mode);
extern int __librt_fstat_r(struct _reent *r,int fd,struct stat *st);
extern int __librt_fstat64_r(struct _reent *r,int fd,struct stat *st);
extern int __librt_stat_r(struct _reent *r,const char *path,struct stat *st);
extern int __librt_stat64_r(struct _reent *r,const char *path,struct stat *st);
extern int __librt_ftruncate_r(struct _reent *r,int fd,off_t len);
extern int __librt_truncate_r(struct _reent *r,const char *path,off_t len);
extern int __librt_fsync_r(struct _reent *r,int fd);
extern _ssize_t __librt_read_r(struct _reent *r,int fd,void *ptr,size_t len);
extern _ssize_t __librt_write_r(struct _reent *r,int fd,const void *ptr,size_t len);
extern _off_t __librt_lseek_r(struct _reent *r,int fd,_off_t pos,int dir);
extern _off64_t __librt_lseek64_r(struct _reent *r,int fd,_off64_t pos,int dir);
extern caddr_t __librt_sbrk_r(struct _reent *ptr,ptrdiff_t incr);
extern int __librt_chmod_r(struct _reent *r,const char *path,mode_t mode);
extern int __librt_rename_r(struct _reent *r,const char *old,const char *new);
extern int __librt_isatty_r(struct _reent *r,int fd);

extern int __librt_mkdir_r(struct _reent *r,const char *path,mode_t mode);
extern DIR* __librt_opendir_r(struct _reent *r,const char *path);
extern struct dirent* __librt_readdir_r(struct _reent *r,DIR *dirp);
extern int __librt_readdir_r_r(struct _reent *r,DIR *dirp,struct dirent *entry,struct dirent **result);
extern int __librt_closedir_r(struct _reent *r,DIR *dirp);
extern long int __librt_telldir_r(struct _reent *r,DIR *dirp);
extern void __librt_rewinddir_r(struct _reent *r,DIR *dirp);
extern void __librt_seekdir_r(struct _reent *r,DIR *dirp,long int loc);
extern int __librt_rmdir_r(struct _reent *r,const char *dirname);
extern int __librt_link_r(struct _reent *r,const char *old,const char *new);
extern int __librt_unlink_r(struct _reent *r,const char *path);
extern int __librt_access_r(struct _reent *r,const char *path,int amode);
extern int __librt_utime_r(struct _reent *r,const char *path,const struct utimbuf *times);

extern int __librt_usleep_r(struct _reent *r,useconds_t usec);
extern unsigned int __librt_sleep_r(struct _reent *r,unsigned int seconds);
extern clock_t __librt_times_r(struct _reent *r,struct tms *buf);
extern int __librt_gettod_r(struct _reent *r,struct timeval *ptimeval,void *ptimezone);
extern int __librt_settod_r(struct _reent *r,const struct timeval *ptimeval,const struct timezone *ptimezone);
extern mode_t __librt_umask_r(struct _reent *r,mode_t cmask);

extern int __librt_sys_lwmutex_create_r(struct _reent *r,sys_lwmutex_t *lwmutex,const sys_lwmutex_attr_t *attr);
extern int __librt_sys_lwmutex_destroy_r(struct _reent *r,sys_lwmutex_t *lwmutex);
extern int __librt_sys_lwmutex_lock_r(struct _reent *r,sys_lwmutex_t *lwmutex,unsigned long long timeout);
extern int __librt_sys_lwmutex_trylock_r(struct _reent *r,sys_lwmutex_t *lwmutex);
extern int __librt_sys_lwmutex_unlock_r(struct _reent *r,sys_lwmutex_t *lwmutex);

extern int main(int argc,const char *[]);

static void __syscalls_init(void) __attribute__((constructor(104)));
static void __syscalls_init(void)
{
	__syscalls.sbrk_r = __librt_sbrk_r;
	__syscalls.exit = __librt_exit;
	__syscalls.open_r = __librt_open_r;
	__syscalls.close_r = __librt_close_r;
	__syscalls.lseek_r = __librt_lseek_r;
	__syscalls.lseek64_r = __librt_lseek64_r;
	__syscalls.read_r = __librt_read_r;
	__syscalls.write_r = __librt_write_r;
	__syscalls.fstat_r = __librt_fstat_r;
	__syscalls.fstat64_r = __librt_fstat64_r;
	__syscalls.stat_r = __librt_stat_r;
	__syscalls.stat64_r = __librt_stat64_r;
	__syscalls.ftruncate_r = __librt_ftruncate_r;
	__syscalls.truncate_r = __librt_truncate_r;
	__syscalls.fsync_r = __librt_fsync_r;
	__syscalls.chmod_r = __librt_chmod_r;
	__syscalls.rename_r = __librt_rename_r;
	__syscalls.isatty_r = __librt_isatty_r;

	__syscalls.mkdir_r = __librt_mkdir_r;
	__syscalls.opendir_r = __librt_opendir_r;
	__syscalls.readdir_r = __librt_readdir_r;
	__syscalls.readdir_r_r = __librt_readdir_r_r;
	__syscalls.closedir_r = __librt_closedir_r;
	__syscalls.telldir_r = __librt_telldir_r;
	__syscalls.rewinddir_r = __librt_rewinddir_r;
	__syscalls.seekdir_r = __librt_seekdir_r;
	__syscalls.rmdir_r = __librt_rmdir_r;
	__syscalls.link_r = __librt_link_r;
	__syscalls.unlink_r = __librt_unlink_r;
	//__syscalls.access_r = __librt_access_r;
	__syscalls.utime_r = __librt_utime_r;

	__syscalls.sleep_r = __librt_sleep_r;
	__syscalls.usleep_r = __librt_usleep_r;
	__syscalls.gettod_r = __librt_gettod_r;
	__syscalls.settod_r = __librt_settod_r;
	__syscalls.times_r = __librt_times_r;
	__syscalls.umask_r = __librt_umask_r;

	__syscalls.sys_lwmutex_create_r = __librt_sys_lwmutex_create_r;
	__syscalls.sys_lwmutex_destroy_r = __librt_sys_lwmutex_destroy_r;
	__syscalls.sys_lwmutex_lock_r = __librt_sys_lwmutex_lock_r;
	__syscalls.sys_lwmutex_trylock_r = __librt_sys_lwmutex_trylock_r;
	__syscalls.sys_lwmutex_unlock_r = __librt_sys_lwmutex_unlock_r;
}

void _initialize(int argc,const char *argv[])
{
	_init();

	int ret = main(argc,argv);

	exit(ret);
}
