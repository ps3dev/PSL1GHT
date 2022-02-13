#include <_ansi.h>
#include <_syslist.h>
#include <sys/reent.h>
#include <sys/errno.h>
#include <sys/types.h>

#include <sys/process.h>

extern void _fini();

void __librt_exit(int rc)
{
	_fini();
	sysProcessExit(rc);
}
