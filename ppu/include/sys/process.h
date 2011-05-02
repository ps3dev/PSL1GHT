/*! \file sys/process.h
 \brief Process management.
*/

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <ppu-lv2.h>
#include <lv2/process.h>

/*! \brief set stack size to 32 kilobytes. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_32K	0x10
/*! \brief set stack size to 64 kilobytes. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_64K	0x20
/*! \brief set stack size to 96 kilobytes. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_96K	0x30
/*! \brief set stack size to 128 kilobytes. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_128K	0x40
/*! \brief set stack size to 256 kilobytes. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_256K	0x50
/*! \brief set stack size to 512 kilobytes. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_512K	0x60
/*! \brief set stack size to 1 megabyte. */
#define SYS_PROCESS_SPAWN_STACK_SIZE_1M		0x70

#define SYS_PROCESS_PARAM(prio,stacksize) \
	sys_process_param_t __sys_process_param __attribute__((aligned(8), section(".sys_proc_param"), unused)) = { \
		sizeof(sys_process_param_t), \
		0x13bcc5f6, \
		0x00009000, \
		0x00192001, \
		prio, \
		stacksize, \
		0x00100000, \
		0x00000000 \
	};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _sys_process_param
{
	u32 size;
	u32 magic;
	u32 version;
	u32 sdk_version;
	s32 prio;
	u32 stacksize;
	u32 malloc_pagesize;
	u32 ppc_seg;
} sys_process_param_t;

#ifdef __cplusplus
	}
#endif

#endif
