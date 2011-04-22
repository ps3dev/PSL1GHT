#ifndef __PPU_ASM_H__
#define __PPU_ASM_H__

#include <ppu_intrinsics.h>

#define PPU_ALIGNMENT			8

#define __get_opd32(opd64)		((unsigned long long)((opd64) + 16))

#define __get_addr32(addr)		(unsigned int)((unsigned long long)(addr))

#define __build_opd32(opd64,opd32) __extension__ \
	({register unsigned long long func,rtoc; \
	asm volatile("ld %0,0(%2); stw %0,0(%3); ld %1,8(%2); stw %1,4(%3)" : "=&r"(func),"=&r"(rtoc) : "b"((opd64)), "b"((opd32)) : "memory"); \
	(u32)((u64)(opd32)); \
	})

#define __read8(addr) __extension__ \
	({register unsigned char result; \
	asm volatile ("lbz %0,0(%1); sync" : "=r"(result) : "b"((addr))); \
	result;})

#define __read16(addr) __extension__ \
	({register unsigned short result; \
	asm volatile ("lhz %0,0(%1); sync" : "=r"(result) : "b"((addr))); \
	result;})

#define __read32(addr) __extension__ \
	({register unsigned int result; \
	asm volatile ("lwz %0,0(%1); sync" : "=r"(result) : "b"((addr))); \
	result;})

#define __read64(addr) __extension__ \
	({register unsigned long long result; \
	asm volatile ("ld %0,0(%1); sync" : "=r"(result) : "b"((addr))); \
	result;})

#define __write8(addr,val) \
	asm volatile("stb %0,0(%1); eieio" : : "r"((val)), "b"((addr)) : "memory")

#define __write16(addr,val) \
	asm volatile("sth %0,0(%1); eieio" : : "r"((val)), "b"((addr)) : "memory")

#define __write32(addr,val) \
	asm volatile("stw %0,0(%1); eieio" : : "r"((val)), "b"((addr)) : "memory")

#define __write64(addr,val) \
	asm volatile("std %0,0(%1); eieio" : : "r"((val)), "b"((addr)) : "memory")

#define __gettime() __extension__ \
	({register unsigned long long tb; \
	asm volatile("1: mftb %[current_tb]; cmpwi 7,%[current_tb],0; beq- 7,1b" : [current_tb] "=r"(tb) : : "cr7"); \
	tb;})

#endif
