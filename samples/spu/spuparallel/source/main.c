/*
 * Sample program to illustrate a parallel algorithm manipulating a shared
 * array.
 *
 * 6 threads are created
 * 
 *
 * The PPU performs the following tasks:
 *  - initialize a 4x6 array
 *  - create 6 threads. Each thread is assigned a different rank from 0 to 5.
 *    The threads also get the address of the array. They put themselves in
 *    a blocking waiting mode for a signal notification.
 *  - The PPU fills the array with the consecutive elements { 1, 2, .., 24 }.
 *  - The PPU sends a signal to the threads to unblock them.
 *  - Each thread reads a different vector of 4 integers from the array (using
 *    dma), multiplies all elements by 2 and sends the vector back to main
 *    storage.
 *
 * The original array contains the integers : { 1, 2, ... 24 } so the result
 * is the values { 2, 4, ..., 48 }.
 *
 * All 24 multiplications are done in parallel:
 *  - Each SPU perform a vector multiplication of the { 2, 2, 2, 2 } vector with
 *    the read value, hence 4 multiplications per SPU.
 *  - All 6 SPUs do the same job in parallel, hence 4*6 for all SPUs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <sys/spu.h>

#include "spu_bin.h"
#include "spustr.h"

#define ptr2ea(x) ((u64)(void *)(x))

int main(int argc, const char* argv[])
{
	sysSpuImage image;
	u32 group_id;
	sysSpuThreadAttribute attr = { ptr2ea("mythread"), 8+1, SPU_THREAD_ATTR_NONE };
	sysSpuThreadGroupAttribute grpattr = { 7+1, ptr2ea("mygroup"), 0, 0 };
	sysSpuThreadArgument arg[6];
	u32 cause, status;
	int i;
	spustr_t *spu = memalign(16, 6*sizeof(spustr_t));
	uint32_t *array = memalign(16, 24*sizeof(uint32_t));

	printf("Initializing 6 SPUs... ");
	printf("%08x\n", sysSpuInitialize(6, 0));

	printf("Loading ELF image... ");
	printf("%08x\n", sysSpuImageImport(&image, spu_bin, 0));

	printf("Creating thread group... ");
	printf("%08x\n", sysSpuThreadGroupCreate(&group_id, 6, 100, &grpattr));
	printf("group id = %d\n", group_id);

	/* create 6 spu threads */
	for (i = 0; i < 6; i++) {
		spu[i].rank = i;
		spu[i].count = 6;
		spu[i].sync = 0;
		spu[i].array_ea = ptr2ea(array);
		arg[i].arg0 = ptr2ea(&spu[i]);

		printf("Creating SPU thread... ");
		printf("%08x\n", sysSpuThreadInitialize(&spu[i].id, group_id, i, &image, &attr, &arg[i]));
		printf("thread id = %d\n", spu[i].id);

		printf("Configuring SPU... %08x\n",
		sysSpuThreadSetConfiguration(spu[i].id, SPU_SIGNAL1_OVERWRITE|SPU_SIGNAL2_OVERWRITE));
	}

	printf("Starting SPU thread group... ");
	printf("%08x\n", sysSpuThreadGroupStart(group_id));

	printf("Initial array: ");
	for (i = 0; i < 24; i++) {
		array[i] = i+1;
		printf(" %d", array[i]);
	}
	printf("\n");

	/* Send signal notification to waiting spus */
	for (i = 0; i < 6; i++)
		printf("Sending signal... %08x\n",
			sysSpuThreadWriteSignal(spu[i].id, 0, 1));

	printf("Waiting for SPUs to return...\n");
	for (i = 0; i < 6; i++)
		while (spu[i].sync == 0);

	printf("Output array: ");
	for (i = 0; i < 24; i++)
		printf(" %d", array[i]);
	printf("\n");

	printf("Joining SPU thread group... ");
	printf("%08x\n", sysSpuThreadGroupJoin(group_id, &cause, &status));
	printf("cause=%d status=%d\n", cause, status);

	printf("Closing image... %08x\n", sysSpuImageClose(&image));

	free(array);
	free(spu);

	return 0;
}
