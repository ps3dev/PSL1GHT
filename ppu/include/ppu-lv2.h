#ifndef __PPU_LV2_H__
#define __PPU_LV2_H__

#include <ppu-types.h>

#define LV2_INLINE static inline
#define LV2_SYSCALL LV2_INLINE s32

#define lv2syscall0(syscall)													\
	register u64 p1 asm("3");													\
	register u64 p2 asm("4");													\
	register u64 p3 asm("5");													\
	register u64 p4 asm("6");													\
	register u64 p5 asm("7");													\
	register u64 p6 asm("8");													\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall1(syscall,a1)													\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4");													\
	register u64 p3 asm("5");													\
	register u64 p4 asm("6");													\
	register u64 p5 asm("7");													\
	register u64 p6 asm("8");													\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall2(syscall,a1,a2)												\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5");													\
	register u64 p4 asm("6");													\
	register u64 p5 asm("7");													\
	register u64 p6 asm("8");													\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall3(syscall,a1,a2,a3)											\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5") = (a3);											\
	register u64 p4 asm("6");													\
	register u64 p5 asm("7");													\
	register u64 p6 asm("8");													\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall4(syscall,a1,a2,a3,a4)										\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5") = (a3);											\
	register u64 p4 asm("6") = (a4);											\
	register u64 p5 asm("7");													\
	register u64 p6 asm("8");													\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall5(syscall,a1,a2,a3,a4,a5)										\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5") = (a3);											\
	register u64 p4 asm("6") = (a4);											\
	register u64 p5 asm("7") = (a5);											\
	register u64 p6 asm("8");													\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall6(syscall,a1,a2,a3,a4,a5,a6)									\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5") = (a3);											\
	register u64 p4 asm("6") = (a4);											\
	register u64 p5 asm("7") = (a5);											\
	register u64 p6 asm("8") = (a6);											\
	register u64 p7 asm("9");													\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall7(syscall,a1,a2,a3,a4,a5,a6,a7)								\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5") = (a3);											\
	register u64 p4 asm("6") = (a4);											\
	register u64 p5 asm("7") = (a5);											\
	register u64 p6 asm("8") = (a6);											\
	register u64 p7 asm("9") = (a7);											\
	register u64 p8 asm("10");													\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define lv2syscall8(syscall,a1,a2,a3,a4,a5,a6,a7,a8)							\
	register u64 p1 asm("3") = (a1);											\
	register u64 p2 asm("4") = (a2);											\
	register u64 p3 asm("5") = (a3);											\
	register u64 p4 asm("6") = (a4);											\
	register u64 p5 asm("7") = (a5);											\
	register u64 p6 asm("8") = (a6);											\
	register u64 p7 asm("9") = (a7);											\
	register u64 p8 asm("10") = (a8);											\
	register u64 scn asm("11") = (syscall);										\
	__asm__ __volatile__("sc"													\
						 : "=r"(p1), "=r"(p2), "=r"(p3), "=r"(p4),				\
						   "=r"(p5), "=r"(p6), "=r"(p7), "=r"(p8), "=r"(scn)	\
						 : "r"(p1), "r"(p2), "r"(p3), "r"(p4),					\
						   "r"(p5), "r"(p6), "r"(p7), "r"(p8), "r"(scn)			\
						 : "r0","r12","lr","ctr","xer","cr0","cr1","cr5","cr6","cr7","memory")


#define return_to_user_prog(ret_type)			return (ret_type)(p1)

#define register_passing_1(type)				(type)(p2)
#define register_passing_2(type)				(type)(p3)
#define register_passing_3(type)				(type)(p4)
#define register_passing_4(type)				(type)(p5)
#define register_passing_5(type)				(type)(p6)
#define register_passing_6(type)				(type)(p7)
#define register_passing_7(type)				(type)(p8)

#define REG_PASS_SYS_EVENT_QUEUE_RECEIVE		\
event->source = register_passing_1(u64);		\
event->data_1 = register_passing_2(u64);		\
event->data_2 = register_passing_3(u64);		\
event->data_3 = register_passing_4(u64)

#endif
