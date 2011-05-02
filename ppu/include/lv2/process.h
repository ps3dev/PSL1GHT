/*! \file lv2/process.h
 \brief Process management.
*/

#ifndef __LV2_PROCESS_H__
#define __LV2_PROCESS_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Terminate the running process.

This function does not return.
\param status Process return status.
*/
void sysProcessExit(int status) __attribute__((noreturn));

/*! \brief Terminate the running process and execute another program.

This function returns only in case of error.
\param path Path and name of new program to be executed.
\param argv Commandline arguments for the new program.
\param envp Environment variables for the new program (may be \c NULL).
\param data Additional data (may be \c NULL).
\param size Size of additional data.
\param priority Priority.
\param flags Process flags. It contains an OR'ed combination of the following
values:
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_32K
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_64K
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_96K
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_128K
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_256K
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_512K
- \ref SYS_PROCESS_SPAWN_STACK_SIZE_1M
*/
void sysProcessExitSpawn2(const char *path,const char *argv[],const char *envp[],void *data,size_t size,int priority,u64 flags);

#ifdef __cplusplus
	}
#endif

#endif
