/*! \file sys/thread.h
 \brief PPU thread syscalls.
*/

#ifndef __SYS_THREAD_H__
#define __SYS_THREAD_H__

#include <ppu-lv2.h>
#include <lv2/thread.h>

/*! \brief The thread is joinable */
#define THREAD_JOINABLE			1

#ifdef __cplusplus
	extern "C" {
#endif

/*! \brief PPU thread stack data structure. */
typedef struct _sys_ppu_thread_stack_t {
	void *addr;			/*!< \brief pointer to the stack buffer */
	u32 size;			/*!< \brief stack size in bytes */
} sys_ppu_thread_stack_t;

/*! \brief Create a PPU thread.
\param threadid Pointer to storage for the thread id.
\param entry Pointer to the thread's main function.
\param arg Argument for the thread's main function.
\param priority Thread priority (0: highest)
\param stacksize Stack size in bytes.
\param flags Thread flags.

The value is either 0 or \ref THREAD_JOINABLE.
\param threadname The thread name.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysThreadCreate(sys_ppu_thread_t *threadid,void (*entry)(void *),void *arg,s32 priority,u64 stacksize,u64 flags,char *threadname);

/*! \brief Wait for the termination of a joinable PPU thread.
\param threadid The thread id.
\param retval Pointer to storage for the thread's returned value.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadJoin(sys_ppu_thread_t threadid,u64 *retval)
{
	lv2syscall2(44,threadid,(u64)retval);;
	return_to_user_prog(s32);
}

/*! \brief Preempt the calling PPU thread.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadYield()
{
	lv2syscall0(43);
	return_to_user_prog(s32);
}

/*! \brief Make a PPU thread detached (non-joinable).
\param threadid The thread id.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadDetach(sys_ppu_thread_t threadid)
{
	lv2syscall1(45,threadid);
	return_to_user_prog(s32);
}

/*! \brief Test the joinable or detached state of the calling PPU thread.
\param joinable Pointer to storage for the joinable state
	(0: detached, 1: joinable)
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadJoinState(s32 *joinable)
{
	lv2syscall1(46,(u64)joinable);
	return_to_user_prog(s32);
}

/*! \brief Set the priority of a PPU thread.
\param threadid The thread id.
\param prio The new priority.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadSetPriority(sys_ppu_thread_t threadid,s32 prio)
{
	lv2syscall2(47,threadid,prio);
	return_to_user_prog(s32);
}

/*! \brief Get the priority of a PPU thread.
\param threadid The thread id.
\param prio Pointer to storage for the current priority.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadGetPriority(sys_ppu_thread_t threadid,s32 *prio)
{
	lv2syscall2(48,threadid,(u64)prio);
	return_to_user_prog(s32);
}

/*! \brief Rename a PPU thread.
\param threadid The thread id.
\param name The new name.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadRename(sys_ppu_thread_t threadid,const char *name)
{
	lv2syscall2(56,threadid,(u64)name);
	return_to_user_prog(s32);
}

/*! \brief Recover a PPU thread after a page fault.
\param threadid The thread id.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadRecoverPageFault(sys_ppu_thread_t threadid)
{
	lv2syscall1(57,threadid);
	return_to_user_prog(s32);
}

/*! \brief Get stack information from the current PPU thread.
\param info Pointer to storage for the stack information.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysThreadGetStackInformation(sys_ppu_thread_stack_t *info)
{
	struct _sys_ppu_thread_stack_t_32 {
		u32 addr;
		u32 size;
	} info32;

	lv2syscall1(49,(u64)(&info32));
	info->addr = (void*)((u64)info32.addr);
	info->size = info32.size;

	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
