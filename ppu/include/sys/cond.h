/*! \file sys/cond.h
 \brief Condition variables management.
*/

#ifndef __SYS_COND_H__
#define __SYS_COND_H__

#include <lv2/cond.h>
#include <sys/mutex.h>

/*! \brief Pshared attribute for condition variables. */
#define SYS_COND_ATTR_PSHARED			0x0200

#ifdef __cplusplus
	extern "C" {
#endif

/*! \brief Condition variable identifier. */
typedef u32 sys_cond_t;

/*! \brief Condition variable attributes data structure */
typedef struct sys_cond_attr
{
	/*! \brief Pshared flag. Must be 0 or \ref SYS_COND_ATTR_PSHARED. */
	u32 attr_pshared;
	s32 flags;			/*!< Flags. */
	u64 key;			/*!< Key. */
	char name[8];		/*!< Name. */
} sys_cond_attr_t;

/*! \brief Create a condition variable.
\param cond Pointer to storage for the created condition variable identifier.
\param mutex Pointer to the associated mutex.
\param attr Pointer to the attributes data structure.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysCondCreate(sys_cond_t *cond,sys_mutex_t mutex,const sys_cond_attr_t *attr)
{
	lv2syscall3(105,(u64)cond,mutex,(u64)attr);
	return_to_user_prog(s32);
}

/*! \brief Destroy a condition variable.
\param cond Condition variable identifier.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysCondDestroy(sys_cond_t cond)
{
	lv2syscall1(106,cond);
	return_to_user_prog(s32);
}

/*! \brief Wait for a condition variable to be signaled.

The associated mutex must have been previously locked by the
calling thread.
This function atomically unlocks the mutex and waits for the condition variable
to be signaled.
Before returning to the calling thread, this function re-acquires the mutex.
\param cond Condition variable identifier.
\param timeout_usec Timeout value in microseconds, or 0 if no timeout is used.
\return zero if the condition variable was signaled,
nonzero in case of error or if a timeout occured.
*/
LV2_SYSCALL sysCondWait(sys_cond_t cond,u64 timeout_usec)
{
	lv2syscall2(107,cond,timeout_usec);
	return_to_user_prog(s32);
}

/*! \brief Signal a condition variable to at most one waiting thread.
\param cond Condition variable identifier.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysCondSignal(sys_cond_t cond)
{
	lv2syscall1(108,cond);
	return_to_user_prog(s32);
}

/*! \brief Signal a condition variable to all waiting threads.
\param cond Condition variable identifier.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysCondBroadcast(sys_cond_t cond)
{
	lv2syscall1(109,cond);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
