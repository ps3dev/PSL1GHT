#ifndef __TYPES_H__
#define __TYPES_H__

#include <ppu-types.h>
#ifdef __cplusplus
	extern "C" {
#endif


#define SPURS_ALIGN	128
#define SPURS_SIZE	4096
#define SPURS_SIZE2	8192

#define SPURS_ATTRIBUTE_ALIGN	8
#define SPURS_ATTRIBUTE_SIZE	512

#define SPURS_MAX_SPU	8
#define SPURS_NAME_MAX_LENGTH	15




typedef struct SpursInfo {
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
	sys_spu_thread_t spuThreads[SPURS_MAX_SPU];
	sys_ppu_thread_t spursHandlerThread0;
	sys_ppu_thread_t spursHandlerThread1;
	char namePrefix[SPURS_NAME_MAX_LENGTH+1];
	size_t namePrefixLength;
	u32 deadlineMissCounter;
	u32 deadlineMeetCounter;
	u8 padding[280-sizeof(int)*3-sizeof(bool)*2-sizeof(u8)*2-sizeof(void*)-sizeof(u32)-sizeof(u64)-sizeof(u32)-sizeof(sys_spu_group_t)-sizeof(sys_spu_thread_t)*SPURS_MAX_SPU-sizeof(sys_ppu_thread_t)*2-sizeof(u8)*(SPURS_NAME_MAX_LENGTH+1)-sizeof(size_t)-sizeof(u32)*2];
} SpursInfo;

typedef struct Spurs {
	unsigned char space[SPURS_SIZE]  ;
} __attribute__((aligned(SPURS_ALIGN))) Spurs;

typedef struct Spurs2 {
	unsigned char space[SPURS_SIZE2];
} __attribute__((aligned(SPURS_ALIGN))) Spurs2;

typedef struct SpursAttribute {
	unsigned char	skip[SPURS_ATTRIBUTE_SIZE];
} __attribute__((aligned(SPURS_ATTRIBUTE_ALIGN))) SpursAttribute;

#ifdef __cplusplus
	}
#endif

#endif
