#include <spu_intrinsics.h>
#include <spu_mfcio.h>

#include <sys/spu_thread.h>
#include <sys/spu_event.h>

#define SPUP 10

int main(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
	/* get input value from ppu via signal notification register 1 (blocking read) */
	uint32_t x = spu_read_signal1();

	/* return the value multiplied by 3 and by 5 to response variable with event */
	spu_thread_send_event(SPUP, x*3, x*5);

	/* properly exit the thread */
	spu_thread_exit(0);
	return 0;
}

