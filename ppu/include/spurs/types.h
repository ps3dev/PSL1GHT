#ifndef __TYPES_H__
#define __TYPES_H__

#include <ppu-types.h>
#ifdef __cplusplus
	extern "C" {
#endif


#define CELL_SPURS_ALIGN	128
#define CELL_SPURS_SIZE	4096
#define CELL_SPURS_SIZE2	8192

#define CELL_SPURS_ATTRIBUTE_ALIGN	8
#define CELL_SPURS_ATTRIBUTE_SIZE	512

#define CELL_SPURS_MAX_SPU	8
#define CELL_SPURS_NAME_MAX_LENGTH	15




typedef struct CellSpursInfo {
	int nSpus;
	int spuGroupPriority;
	int ppuThreadPriority;
	bool exitIfNoWork;
	bool spurs2;
	u8	__padding24[2];
	void *traceBuffer ATTRIBUTE_PRXPTR;
	u32 __padding32;
	u64 traceBufferSize;
	u32 traceMode;
	sys_spu_group_t  spuGroup;
	sys_spu_thread_t spuThreads[CELL_SPURS_MAX_SPU];
	sys_ppu_thread_t spursHandlerThread0;
	sys_ppu_thread_t spursHandlerThread1;
	char namePrefix[CELL_SPURS_NAME_MAX_LENGTH+1];
	size_t namePrefixLength;
	u32 deadlineMissCounter;
	u32 deadlineMeetCounter;
	u8 padding[280-sizeof(int)*3-sizeof(bool)*2-sizeof(u8)*2-sizeof(void*)-sizeof(u32)-sizeof(u64)-sizeof(u32)-sizeof(sys_spu_group_t)-sizeof(sys_spu_thread_t)*CELL_SPURS_MAX_SPU-sizeof(sys_ppu_thread_t)*2-sizeof(u8)*(CELL_SPURS_NAME_MAX_LENGTH+1)-sizeof(size_t)-sizeof(u32)*2];
} CellSpursInfo;

typedef struct CellSpurs {
	unsigned char space[CELL_SPURS_SIZE]  ;
} __attribute__((aligned(CELL_SPURS_ALIGN))) CellSpurs;

typedef struct CellSpurs2 {
	unsigned char space[CELL_SPURS_SIZE2];
} __attribute__((aligned(CELL_SPURS_ALIGN))) CellSpurs2;

typedef struct CellSpursAttribute {
	unsigned char	skip[CELL_SPURS_ATTRIBUTE_SIZE];
} __attribute__((aligned(CELL_SPURS_ATTRIBUTE_ALIGN))) CellSpursAttribute;

#ifdef __cplusplus
	}
#endif

#endif
