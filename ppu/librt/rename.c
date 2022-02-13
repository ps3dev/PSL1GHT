#include <stdio.h>
#include <fcntl.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/lv2errno.h>

#include <sys/file.h>

int __librt_rename_r(struct _reent *r, const char *old, const char *new)
{
	return lv2errno_r(r,sysLv2FsRename(old,new));
}
