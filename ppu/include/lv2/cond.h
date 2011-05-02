/*! \file lv2/cond.h
 \brief Lightweight condition variable library.
*/

#ifndef __LV2_COND_H__
#define __LV2_COND_H__

#include <ppu-types.h>
#include <lv2/mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Lightweight condition variable data structure. */
typedef struct sys_lwcond
{
	sys_lwmutex_t *lwmutex;		/*!< \brief Associated lightweight mutex. */
	u32 lwcond_queue;			/*!< \brief Next element in waiting queue. */
} sys_lwcond_t;

/*! \brief Lightweight condition variable attributes data structure. */
typedef struct sys_lwcond_attr
{
	char name[8];				/*!< \brief Name. */
} sys_lwcond_attr_t;

/*! \brief Create a lightweight condition variable.
\param lwcond Pointer to storage for the created lightweight condition variable.
\param lwmutex Pointer to the associated lightweight mutex.
\param attr Pointer to the attributes data structure.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysLwCondCreate(sys_lwcond_t *lwcond,sys_lwmutex_t *lwmutex,sys_lwcond_attr_t *attr);

/*! \brief Destroy a lightweight condition variable.
\param lwcond Pointer to the allocated lightweight condition variable.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysLwCondDestroy(sys_lwcond_t *lwcond);

/*! \brief Wait for a lightweight condition variable to be signaled.

The associated lightweight mutex must have been previously locked by the
calling thread.
This function atomically unlocks the mutex and waits for the condition variable
to be signaled.
Before returning to the calling thread, this function re-acquires the mutex.
\param lwcond Pointer to the allocated lightweight condition variable.
\param timeout_usec Timeout value in microseconds, or 0 if no timeout is used.
\return zero if the lightweight condition variable was signaled,
nonzero in case of error or if a timeout occured.
*/
s32 sysLwCondWait(sys_lwcond_t *lwcond,u64 timeout_usec);

/*! \brief Signal a lightweight condition variable.
\param lwcond Pointer to the allocated lightweight condition variable.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysLwCondSignal(sys_lwcond_t *lwcond);

/*! \brief Signal a lightweight condition variable to a specific PPU thread.
\param lwcond Pointer to the allocated lightweight condition variable.
\param thr The PPU thread id.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysLwCondSignalTo(sys_lwcond_t *lwcond,sys_ppu_thread_t thr);

/*! \brief Signal a lightweight condition variable to all waiting PPU threads.
\param lwcond Pointer to the allocated lightweight condition variable.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysLwCondSignalAll(sys_lwcond_t *lwcond);

#ifdef __cplusplus
	}
#endif

#endif
