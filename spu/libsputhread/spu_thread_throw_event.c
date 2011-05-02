#include <stdint.h>
#include <spu_intrinsics.h>
#include <spu_event.h>

#include <sys/spu_event.h>

int spu_thread_throw_event(uint8_t spup,uint32_t data0,uint32_t data1)
{
	uint32_t val = (0x40000000 | (spup<<EVENT_PORT_SHIFT) | (data0&EVENT_DATA0_MASK));

	if(spup>EVENT_PORT_MAX_NUM) return 0x80010002;

	spu_writech(SPU_WrOutMbox,data1);
	spu_writech(SPU_WrOutIntrMbox,val);

	return 0;
}
