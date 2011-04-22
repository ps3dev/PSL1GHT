#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/spu.h>
#include <sys/event_queue.h>

#include "spu_bin.h"

#define SPUP 10
#define ptr2ea(x)			((u64)((void*)(x)))

int main(int argc,char *argv[])
{
	u32 cause,status;
	sysSpuImage image;
	u32 thread_id,group_id;
	sys_event_queue_t evQ;
	sys_event_t event;
	sys_event_queue_attr_t evQAttr = { SYS_EVENT_QUEUE_FIFO, SYS_EVENT_QUEUE_PPU, "myEvQ" };
	sysSpuThreadArgument arg = { 0, 0, 0, 0 };
	sysSpuThreadGroupAttribute grpattr = { 7+1, ptr2ea("mygroup"), 0, 0 };
	sysSpuThreadAttribute attr = { ptr2ea("mythread"), 8+1, SPU_THREAD_ATTR_NONE };

	sysSpuInitialize(6,0);
	sysSpuImageImport(&image,spu_bin,0);

	sysSpuThreadGroupCreate(&group_id,1,100,&grpattr);

	sysEventQueueCreate(&evQ, &evQAttr, 0x4242, 16);

	sysSpuThreadInitialize(&thread_id,group_id,0,&image,&attr,&arg);

	sysSpuThreadSetConfiguration(thread_id,(SPU_SIGNAL1_OVERWRITE | SPU_SIGNAL2_OVERWRITE));

	sysSpuThreadConnectEvent(thread_id, evQ, SPU_THREAD_EVENT_USER, SPUP);

	printf("Starting SPU thread group....\n");
	sysSpuThreadGroupStart(group_id);

	printf("input value: 11\n");

	sysSpuThreadWriteSignal(thread_id,0,11);

	printf("Waiting for SPU to return....\n");
	sysEventQueueReceive(evQ, &event, 0);

	if (event.source == SPU_THREAD_EVENT_USER_KEY
		&& event.data_1 == thread_id
		&& (event.data_2>>32) == SPUP)
	{
		int data0 = event.data_2&0xffffff;
		int data1 = event.data_3;
		printf("output values: %d %d\n", data0, data1);
	}
	else
		printf("error: unexpected event value!\n");

	sysSpuThreadGroupJoin(group_id,&cause,&status);
	sysSpuThreadDisconnectEvent(thread_id, SPU_THREAD_EVENT_USER, SPUP);

	sysSpuThreadGroupDestroy(group_id);
	sysSpuImageClose(&image);

	return 0;
}
