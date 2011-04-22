#ifndef __SPUSTR_H__
#define __SPUSTR_H__

#include <stdint.h>

typedef struct _spustr
{
	uint32_t id;
	uint32_t rank;
	uint32_t count;
	volatile uint32_t sync;
	uint32_t response;
	uint32_t array_ea;
	uint32_t dummy[2];
} spustr_t;

#endif
