/*! \file lv2/interrupt.h
 \brief Thread Interrupt functions.
*/ 

#ifndef __LV2_INTERRUPT_H__
#define __LV2_INTERRUPT_H__

#include <ppu-types.h>
#include <ppu-lv2.h>

#ifdef __cplusplus
extern "C" {
#endif 

typedef u64 sys_irqoutlet_id_t;
typedef u32 sys_hw_thread_t;
typedef u32 sys_interrupt_thread_handle_t;
typedef u32 sys_interrupt_handler_handle_t;

#define SYS_HW_THREAD_ANY		0xfffffffeU
#define SYS_HW_THREAD_INVALID	0xffffffffU
#define SYS_INTERRUPT_TAG_ID_INVALID 0xffffffffU

LV2_SYSCALL sysInterruptTagCreate(sys_interrupt_tag_t * intrTag,
										   sys_irqoutlet_id_t irq,
										   sys_hw_thread_t hwThread) 
{
	lv2syscall3(80, (u64)intrTag, irq, hwThread);
	return_to_user_prog(s32);
}


LV2_SYSCALL sysInterruptTagDestroy(sys_interrupt_tag_t intrTag) 
{
	lv2syscall1(81, intrTag);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysInterruptThreadEstablish(sys_interrupt_thread_handle_t *ih,
												 sys_interrupt_tag_t intrTag,
												 sys_ppu_thread_t intrThread,
												 u64 arg) 
{
	lv2syscall5(84, (u64)ih, intrTag, intrThread, arg, 0);
	return_to_user_prog(s32);
}

LV2_SYSCALL _sysInterruptThreadDisestablish(sys_interrupt_thread_handle_t ih, 
								   u64 * tlsMem)
{
	lv2syscall2(89, ih, (u64)tlsMem);
	return_to_user_prog(s32);
}

static inline void sysInterruptThreadEOI(void) 
{
	lv2syscall0(88);
}

s32 sysInterruptThreadDisestablish(sys_interrupt_thread_handle_t ih);

#ifdef __cplusplus
	}
#endif 
#endif /* __LV2_INTERRUPT_H__ */
