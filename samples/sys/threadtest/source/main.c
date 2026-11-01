#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <ppu-types.h>

#include <io/pad.h>

#include <sys/thread.h>

static void thread_start(void *arg)
{
	s32 running = 0;
	sys_ppu_thread_t id;
	sys_ppu_thread_stack_t stackinfo;

	sysThreadGetId(&id);
	sysThreadGetStackInformation(&stackinfo);

	printf("stack\naddr: %p, size: %d\n",stackinfo.addr,stackinfo.size);
	while(running<5) {
		printf("Thread: %08llX\n",(unsigned long long int)id);

		sysThreadYield();
		sleep(2);
		running++;
	}

	sysThreadExit(0);
}

int main(int argc,char *argv[])
{
	u64 retval;
	s32 i,ret,running;
	sys_ppu_thread_t id;
	u64 prio = 1500;
	CellPadInfo padinfo;
	CellPadData paddata;
	size_t stacksize = 0x1000;
	char *threadname = "myThread";
	void *threadarg = (void*)0x1337;

	cellPadInit(7);

	ret = sysThreadCreate(&id,thread_start,threadarg,prio,stacksize,THREAD_JOINABLE,threadname);
	printf("sysThreadCreate: %d\n",ret);

	ret = sysThreadJoin(id,&retval);
	printf("sysThreadJoin: %d - %llX\n",ret,(unsigned long long int)retval);

	running = 1;
	while(running) {
		cellPadGetInfo(&padinfo);
		for(i=0; i<CELL_MAX_PADS; i++){
			if(padinfo.status[i]){
				cellPadGetData(i, &paddata);

				if(paddata.BTN_CROSS){
					running = 0;
					break;
				}
			}
		}
	}

	printf("Exiting thread test\n");
	return 0;
}
