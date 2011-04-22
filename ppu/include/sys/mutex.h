/*! \file sys/mutex.h
 \brief Mutual exclusion (mutex) syscalls.
*/

#ifndef __SYS_MUTEX_H__
#define __SYS_MUTEX_H__

#include <ppu-lv2.h>
#include <lv2/mutex.h>

/*! \brief FIFO mutex scheduling policy. */
#define SYS_MUTEX_PROTOCOL_FIFO				1
/*! \brief Priority-based mutex scheduling policy. */
#define SYS_MUTEX_PROTOCOL_PRIO				2
/*! \brief Priority-based mutex scheduling policy with inheritance. */
#define SYS_MUTEX_PROTOCOL_PRIO_INHERIT		3

/*! \brief Mutex is recursive. */
#define SYS_MUTEX_ATTR_RECURSIVE		0x0010
/*! \brief Mutex is not recursive. */
#define SYS_MUTEX_ATTR_NOT_RECURSIVE	0x0020

/*! \brief Default sharing policy for mutex attributes. */
#define SYS_MUTEX_ATTR_PSHARED			0x0200

/*! \brief Mutex is adaptive. */
#define SYS_MUTEX_ATTR_ADAPTIVE			0x1000
/*! \brief Mutex is not adaptive. */
#define SYS_MUTEX_ATTR_NOT_ADAPTIVE		0x2000

#ifdef __cplusplus
	extern "C" {
#endif

/*! \brief Mutex identifier. */
typedef s32 sys_mutex_t;

/*! \brief Mutex attributes data structure.*/
typedef struct sys_mutex_attr
{
	/*! \brief Scheduling policy.

	Possible values are:
	- \ref SYS_MUTEX_PROTOCOL_FIFO
	- \ref SYS_MUTEX_PROTOCOL_PRIO
	- \ref SYS_MUTEX_PROTOCOL_PRIO_INHERIT
	*/
	u32 attr_protocol;
	/*! \brief Recursive setting.

	Possible values are:
	- \ref SYS_MUTEX_ATTR_RECURSIVE
	- \ref SYS_MUTEX_ATTR_NOT_RECURSIVE
	*/
	u32 attr_recursive;
	/*! \brief Sharing policy (only known value for it is \ref SYS_MUTEX_ATTR_PSHARED) */
	u32 attr_pshared;
	/*! \brief Adaptive setting.

	Possible values are:
	- \ref SYS_MUTEX_ATTR_ADAPTIVE
	- \ref SYS_MUTEX_ATTR_NOT_ADAPTIVE
	*/
	u32 attr_adaptive;
	/*! \brief Mutex key. */
	u64 key;
	/*! \brief Mutex flags. */
	s32 flags;
	/*! \brief Unused padding element. */
	u32 _pad;
	/*! \brief Mutex name. */
	char name[8];
} sys_mutex_attr_t;

/*! \brief Create a mutex.
\param mutex Pointer to storage for the mutex id.
\param attr Pointer to the mutex attributes.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysMutexCreate(sys_mutex_t *mutex,const sys_mutex_attr_t *attr)
{
	lv2syscall2(100,(u64)mutex,(u64)attr);
	return_to_user_prog(s32);
}

/*! \brief Destroy a mutex.
\param mutex The mutex id.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysMutexDestroy(sys_mutex_t mutex)
{
	lv2syscall1(101,mutex);
	return_to_user_prog(s32);
}

/*! \brief Lock a mutex.
\param mutex The mutex id.
\param timeout_usec Timeout value in microseconds, or 0 if no timeout is used.
\return zero if the mutex was successfully locked,
nonzero in case of error or if a timeout occured.
*/
LV2_SYSCALL sysMutexLock(sys_mutex_t mutex,u64 timeout_usec)
{
	lv2syscall2(102,mutex,timeout_usec);
	return_to_user_prog(s32);
}

/*! \brief Try to lock a mutex (non-blocking).
\param mutex The mutex id.
\return zero if the mutex was successfully locked,
nonzero in case of error or if the mutex is already locked by another thread.
*/
LV2_SYSCALL sysMutexTryLock(sys_mutex_t mutex)
{
	lv2syscall1(103,mutex);
	return_to_user_prog(s32);
}

/*! \brief Unlock a previously locked mutex.
\param mutex The mutex id.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysMutexUnlock(sys_mutex_t mutex)
{
	lv2syscall1(104,mutex);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
