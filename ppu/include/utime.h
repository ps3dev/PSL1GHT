#ifndef _UTIME_H
#define _UTIME_H

#define SYS_FS_OK			0
#define SYS_FS_ENOTMOUNTED 	-2147418054     /* 0x8001003A */
#define SYS_FS_ENOENT      	-2147418106     /* 0x80010006 */
#define SYS_FS_ENOTSUP    	-2147418057     /* 0x80010037 */
#define SYS_FS_EIO        	-2147418069     /* 0x8001002B */
#define SYS_FS_ENOMEM      	-2147418108     /* 0x80010004 */
#define SYS_FS_ENOTDIR    	-2147418066     /* 0x8001002E */
#define SYS_FS_ENAMETOOLONG	-2147418060     /* 0x80010034 */
#define SYS_FS_EFSSPECIFIC 	-2147418056     /* 0x80010038 */
#define SYS_FS_EINVAL      	-2147418110     /* 0x80010002 */
#define SYS_FS_EROFS        -2147418074     /* 0x80010026 */
#define SYS_FS_EFAULT       -2147418099     /* 0x8001000D */
#define SYS_FS_EACCES       -2147418071     /* 0x80010029 */

#ifdef __cplusplus
extern "C" {
#endif

struct utimbuf {
               time_t actime;       /* access time */
               time_t modtime;      /* modification time */
           };

int utime(const char *filename, const struct utimbuf *times);

#ifdef __cplusplus
};
#endif
#endif
