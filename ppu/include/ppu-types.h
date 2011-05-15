#ifndef __PPU_TYPES_H__
#define __PPU_TYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef uint8_t			u8;
typedef uint16_t 		u16;
typedef uint32_t 		u32;
typedef uint64_t	 	u64;

typedef int8_t 			s8;
typedef int16_t 		s16;
typedef int32_t 		s32;
typedef int64_t		 	s64;

typedef volatile u8		vu8;
typedef volatile u16	vu16;
typedef volatile u32	vu32;
typedef volatile u64	vu64;

typedef volatile s8		vs8;
typedef volatile s16	vs16;
typedef volatile s32	vs32;
typedef volatile s64	vs64;

typedef float			f32;
typedef double			f64;

typedef volatile f32	vf32;
typedef volatile f64	vf64;

typedef u32 sys_mem_id_t;
typedef u32 sys_mem_addr_t;
typedef u32 sys_mem_container_t;

typedef u32 sys_raw_spu_t;
typedef u32 sys_spu_group_t;
typedef u32 sys_spu_thread_t;
typedef u32 sys_interrupt_tag_t;

/*! \brief PPU thread identifier. */
typedef u64 sys_ppu_thread_t;

typedef u32 sys_cond_t;

/*! \brief Mutex identifier. */
typedef s32 sys_mutex_t;

/*! \brief Event queue id */
typedef u32 sys_event_queue_t;

/*! \brief Event port id */
typedef u32 sys_event_port_t;

/*! \brief IPC key id */
typedef u64 sys_ipc_key_t;

typedef u32 sys_sem_t;

typedef struct _opd64
{
	void *func;
	void *rtoc;
	u64 zero;
} opd64;

typedef struct _opd32
{
	u32 func;
	u32 rtoc;
} opd32;

typedef union _ieee32
{
	u32 u;
	f32 f;
} ieee32;

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef NULL
#define NULL	((void*)0)
#endif

#define SPU_ALIGNMENT				128
#define SPU_ALIGNSIZE(x)			(((x) + 127)&~127)

#define ATTRIBUTE_PRXPTR			__attribute__((mode(SI)))

// courtesy of Marcan
#define STACK_ALIGN(type, name, cnt, alignment)		u8 _al__##name[((sizeof(type)*(cnt)) + (alignment) + (((sizeof(type)*(cnt))%(alignment)) > 0 ? ((alignment) - ((sizeof(type)*(cnt))%(alignment))) : 0))]; \
													type *name = (type*)(((u64)(_al__##name)) + ((alignment) - (((u64)(_al__##name))&((alignment)-1))))

#ifdef __cplusplus
	}
#endif

#endif

