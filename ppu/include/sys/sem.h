/*! \file sem.h
 \brief Semaphore management syscalls.
*/

#ifndef __SYS_SEM_H__
#define __SYS_SEM_H__

#include <ppu-lv2.h>

/*! \brief Default protocol for semaphore attributes. */
#define SYS_SEM_ATTR_PROTOCOL			0x0002
/*! \brief Default sharing policy for semaphore attributes. */
#define SYS_SEM_ATTR_PSHARED			0x0200

#ifdef __cplusplus
	extern "C" {
#endif

typedef u32 sys_sem_t;

/*! \brief Semaphore sttributes data structure. */
typedef struct sys_sem_attr
{
	/*! \brief Management protocol (default is \ref SYS_SEM_ATTR_PROTOCOL) */
	u32 attr_protocol;
	/*! \brief Sharing policy (default is \ref SYS_SEM_ATTR_PSHARED) */
	u32 attr_pshared;
	/*! \brief Key */
	u64 key;
	/*! \brief Flags */
	s32 flags;
	/*! \brief Unused padding member */
	u32 pad;
	/*! \brief Semaphore name */
	char name[8];
} sys_sem_attr_t;

/*! \brief Create a semaphore.
\param sem Pointer to storage to the semaphore id.
\param attr Pointer to the semaphore attributes.
\param initial_val Initial value.
\param max_val Maximum value.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSemCreate(sys_sem_t *sem,const sys_sem_attr_t *attr,s32 initial_val,s32 max_val)
{
	lv2syscall4(90,(u64)sem,(u64)attr,initial_val,max_val);
	return_to_user_prog(s32);
}

/*! \brief Destroy a semaphore.
\param sem The semaphore id.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSemDestroy(sys_sem_t sem)
{
	lv2syscall1(91,sem);
	return_to_user_prog(s32);
}

/*! \brief Wait and reserve a semaphore.
\param sem The semaphore id.
\param timeout_usec Timeout value in microseconds, or 0 if no timeout is used.
\return zero if the semaphore was successfully reserved,
nonzero in case of error or if a timeout occured.
*/
LV2_SYSCALL sysSemWait(sys_sem_t sem,u64 timeout_usec)
{
	lv2syscall2(92,sem,timeout_usec);
	return_to_user_prog(s32);
}

/*! \brief Reserve a semaphore (non-blocking).
\param sem The semaphore id.
\return zero if the semaphore was successfully reserved,
nonzero in case of error or if the semaphore value is below 1.
*/
LV2_SYSCALL sysSemTryWait(sys_sem_t sem)
{
	lv2syscall1(93,sem);
	return_to_user_prog(s32);
}

/*! \brief Release a semaphore.
\param sem The semaphore id.
\param count Increment value.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSemPost(sys_sem_t sem,s32 count)
{
	lv2syscall2(94,sem,count);
	return_to_user_prog(s32);
}

/*! \brief Get the value of a semaphore.
\param sem The semaphore id.
\param count Pointer to storage for the semaphore value.
\return zero if no error occured, nonzero otherwise.
*/
LV2_SYSCALL sysSemGetValue(sys_sem_t sem,s32 *count)
{
	lv2syscall2(114,sem,(u64)count);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
