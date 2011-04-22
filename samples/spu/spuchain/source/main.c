/*
* Sample program to illustrate a chain of SPU threads.
*
* 6 threads are created
* Each thread is assigned a different rank from 0 to 5.
*
* The PPU sends a signal notification to thread 0.
* Each thread waits for a signal notification, then do the following:
* - Thread 0 reads a 4 integer vector, multiplies it by 2 and writes it to
* thread 1's local store at the same address (which is the same among all
* threads) with dma
* - Thread n (n=1..5) multiplies the vector in memory (written by thread n-1)
* by 2, and sends it to thread n+1 (except thread 5 which writes the result
* to main storage)
*
* The original vector contains the integers : { 1, 2, 3, 4 } so the result
* should be { 64, 128, 192, 256 }.
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <sys/spu.h>

#include "spustr.h"
#include "spu_bin.h"

#define ptr2ea(x)			((u64)((void*)(x)))

int main(int argc,char *argv[])
{
	u32 *array;
	u32 group_id;
	spustr_t *spu;
	sysSpuImage image;
	u32 cause,status,i;
	sysSpuThreadArgument arg[6];
	sysSpuThreadGroupAttribute grpattr = { 7+1, ptr2ea("mygroup"), 0, 0 };
	sysSpuThreadAttribute attr = { ptr2ea("mythread"), 8+1, SPU_THREAD_ATTR_NONE };

	printf("spuchain starting....\n");

	sysSpuInitialize(6,0);
	sysSpuImageImport(&image,spu_bin,0);
	sysSpuThreadGroupCreate(&group_id,6,100,&grpattr);

	spu = (spustr_t*)memalign(128,6*sizeof(spustr_t));
	array = (u32*)memalign(128,4*sizeof(u32));

	for(i=0;i<6;i++) {
		spu[i].rank = i;
		spu[i].count = 6;
		spu[i].sync = 0;
		spu[i].array_ea = ptr2ea(array);
		arg[i].arg0 = ptr2ea(&spu[i]);

		printf("Creating SPU thread... ");
		sysSpuThreadInitialize(&spu[i].id,group_id,i,&image,&attr,&arg[i]);
		printf("%08x\n",spu[i].id);
		sysSpuThreadSetConfiguration(spu[i].id,(SPU_SIGNAL1_OVERWRITE | SPU_SIGNAL2_OVERWRITE));
	}

	printf("Starting SPU thread group....\n");
	sysSpuThreadGroupStart(group_id);

	printf("Initial array: ");
	for(i=0;i<4;i++) {
		array[i] = (i + 1);
		printf(" %d",array[i]);
	}
	printf("\n");

	/* Send signal notification to SPU 0 */
	printf("sending signal.... \n");
	sysSpuThreadWriteSignal(spu[0].id,0,1);

	/* Wait for SPU 5 to return */
	while(spu[5].sync==0);

	printf("Output array: ");
	for(i=0;i<4;i++) printf(" %d",array[i]);
	printf("\n");

	printf("Joining SPU thread group....\n");
	sysSpuThreadGroupJoin(group_id,&cause,&status);
	sysSpuImageClose(&image);

	free(array);
	free(spu);

	return 0;
}
