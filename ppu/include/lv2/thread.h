/*! \file lv2/thread.h
 \brief PPU thread library.
*/

#ifndef __LV2_THREAD_H__
#define __LV2_THREAD_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Terminate the running PPU thread.
\param ret_val Thread return value.
*/
void sysThreadExit(u64 ret_val);

/*! \brief Get the thread id of the running PPU thread.
\param threadid Pointer to storage for the running PPU thread id.
\return zero if no error occured, nonzero otherwise.
*/
s32 sysThreadGetId(sys_ppu_thread_t *threadid);

#ifdef __cplusplus
	}
#endif

#endif
