#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/spu.h>
#include <spu_data.h>

int main(int argc,char *argv[])
{
	u32 spu_id = 0;
	sysSpuImage image;

	printf("sputest starting....\n");

	printf("Initializing 6 SPUs...\n");
	sysSpuInitialize(6,5);

	printf("Initializing raw SPU...\n");
	sysSpuRawCreate(&spu_id,NULL);

	printf("Importing spu image...\n");
	sysSpuImageImport(&image,spu_elf,SPU_IMAGE_PROTECT);

	printf("Loading spu image into SPU %d...\n",spu_id);
	sysSpuRawImageLoad(spu_id,&image);

	printf("Starting SPU %d...\n",spu_id);
	sysSpuRawWriteProblemStorage(spu_id,SPU_RunCtrl,1);

	printf("Waiting for SPU to return...\n");
	while (!(sysSpuRawReadProblemStorage(spu_id,SPU_MBox_Status) & 1));

	printf("SPU Mailbox return value: %08x\n",sysSpuRawReadProblemStorage(spu_id,SPU_Out_MBox));

	printf("Destroying SPU %d...\n",spu_id);
	sysSpuRawDestroy(spu_id);

	printf("Closing SPU image...\n");
	sysSpuImageClose(&image);

	return 0;
}
