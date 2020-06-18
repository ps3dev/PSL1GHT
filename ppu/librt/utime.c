#include <sys/file.h>
#include <time.h>
#include <utime.h>
#include <sys/errno.h>

extern int errno;

int __attribute__((weak)) utime(const char *filename, const struct utimbuf *times)
{
	s32 ret;
	if (times == NULL) {
		struct utimbuf now_time;
		time_t     now;
		now = time(NULL);
		now_time.actime = now;
		now_time.modtime = now;
		ret = sysLv2FsUtime(filename,(sysFSUtimbuf *)&now_time);
	}
	else ret = sysLv2FsUtime(filename,(sysFSUtimbuf *)times);
	if (ret != SYS_FS_OK ) {
		switch (ret) {
			case SYS_FS_EACCES:
					errno = EACCES;
					break;
			case SYS_FS_ENOTDIR:
					errno = ENOTDIR;
					break;
			case SYS_FS_ENOENT:
					errno = ENOENT;
					break;
			case SYS_FS_ENAMETOOLONG:
					errno = ENAMETOOLONG;
					break;
			case SYS_FS_EROFS:
					errno = EROFS;
					break;
			case SYS_FS_ENOTSUP:
					errno = EPERM;
					break;		
				
			default:
				  errno = ENOENT;
		}
		return - 1;
	}
	return ret;
}