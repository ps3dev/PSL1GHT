#include <stdio.h>
#include <string.h>
#include <ppu-types.h>
#include <spurs/spurs.h>
#include <lv2/spu.h>
#include <sys/thread.h>
#include <sys/spu.h>
#include <sys/memory.h>
#include <sys/process.h>

#define SPU_NUMBER 6
#define SPURS_PREFIX_NAME "samplespurs"
#define SPU_THREAD_GROUP_PRIORITY 250


SYS_PROCESS_PARAM(1001, 0x10000);


int main(void)
{
	int ret;
	int i;
	sys_ppu_thread_t ppu_thread_id;
	int ppu_prio;
	unsigned int nthread;
	ret=sysSpuInitialize(SPU_NUMBER, 0);
	printf("sysSpuInitialize return %d\n",ret);
	
	ret= sysThreadGetId(&ppu_thread_id);
	printf("sysThreadGetId return %d ppu_thread_id %x\n",ret,ppu_thread_id);

	ret = sysThreadGetPriority(ppu_thread_id, &ppu_prio);
	printf("sysThreadGetPriority return %d ppu_prio %d\n",ret,ppu_prio);
	
	
	
	
	
	/* initialize spurs */
	printf("Initializing spurs\n");
	Spurs *spurs= (void*)memalign(SPURS_ALIGN, sizeof(Spurs));
	printf("Initializing spurs attribute\n");
	SpursAttribute	attributeSpurs;
	
	ret=spursAttributeInitialize(&attributeSpurs,SPU_NUMBER, SPU_THREAD_GROUP_PRIORITY, ppu_prio-1, true);
	if (ret) {
		printf("error : spursAttributeInitialize failed  %x\n", ret);
		return (ret);
	}
	
	printf("Setting name prefix\n");
	ret=spursAttributeSetNamePrefix(&attributeSpurs, SPURS_PREFIX_NAME, strlen(SPURS_PREFIX_NAME));
	if (ret) {
		printf("error : spursAttributeInitialize failed %x\n", ret);
		return (ret);
	}
	
	printf("Initializing with attribute\n");
	ret=spursInitializeWithAttribute(spurs, &attributeSpurs);
	if (ret) {
		printf("error: spursInitializeWithAttribute failed  %x\n", ret);
		return (ret);
	}

	ret = spursGetNumSpuThread(spurs, &nthread);
	if(ret){
		printf("error: spursGetNumSpuThread failed %x\n", ret);
	}

	sys_spu_thread_t *threads = (sys_spu_thread_t *)malloc(sizeof(sys_spu_thread_t) * nthread);

	ret = spursGetSpuThreadId(spurs, threads, &nthread);
	if(ret){
		printf("error: spursGetSpuThreadId failed %x\n", ret);
	}
	
	printf("SPURS %d spu threads availables\n", nthread);
	for(i=0; i<nthread; i++) {
		printf("SPU Number:%d\tSPU Thread ID:%x\n", i, threads[i]);
	}
	printf("\n");
	
	printf("checking SpursInfo\n");
	SpursInfo info;
	ret=spursGetInfo(spurs, &info);
	if(ret)
	{
	 	printf("error: spursGetInfo failed %x\n", ret);
	}
	printf("SpursInfo: \n");
	printf("nSpus=%d \n",info.nSpus);
	printf("spuGroupPriority=%d \n",info.spuGroupPriority);
	printf("ppuThreadPriority=%d \n",info.ppuThreadPriority);
	printf("exitIfNoWork=%d \n",info.exitIfNoWork);
	printf("namePrefix=%s \n",info.namePrefix);
	for(i=0;i<info.nSpus;i++)
	{
		printf("SPU Number:%d\tSPU Thread ID:%x\n", i, info.spuThreads[i]);
	}
	/* finalize spurs */
	printf("Finalize spurs\n");
	ret = spursFinalize (spurs);
	if (ret) {
		printf("error: spursFinalize failed : %x\n", ret);
		return ret;
	}
	free(spurs);
	
	return 0;
}