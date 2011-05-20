/*! \file lv2/interrupt.h
 \brief Thread Interrupt functions.
*/ 

#ifndef __LV2_INTERRUPT_H__
#define __LV2_INTERRUPT_H__

#include <ppu-types.h>

#ifdef __cplusplus
extern "C" {
#endif 

s32 sysInterruptThreadDisestablish(sys_interrupt_thread_handle_t ih);

#ifdef __cplusplus
	}
#endif 
#endif /* __LV2_INTERRUPT_H__ */
