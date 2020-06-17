#ifndef _UTIME_H
#define _UTIME_H

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
