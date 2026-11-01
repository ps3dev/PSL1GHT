#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <rsx/rsx.h>
#include <sys/spu.h>

#include "spu.h"
#include "rsxutil.h"
#include "spu_bin.h"

static u32 spu_id;
static sysSpuImage spu_image;
static spu_param spuparam __attribute__((aligned(128)));

void setup_shared_buffer(u32 const_addr,u32 read_addr,u32 write_addr)
{
	spuparam.fragment_const_addr = const_addr;
	spuparam.spu_read_label_addr = read_addr;
	spuparam.spu_write_label_addr = write_addr;
}

s32 init_spu()
{
	printf("starting SPU......\n");

	sysSpuInitialize(MAX_PHYSICAL_SPU,MAX_RAW_SPU);
	sysSpuRawCreate(&spu_id,NULL);
	sysSpuImageImport(&spu_image,spu_bin,SPU_IMAGE_PROTECT);
	
	sysSpuRawImageLoad(spu_id,&spu_image);
	sysSpuRawWriteProblemStorage(spu_id,SPU_RunCtrl,1);

	printf("SPU started......\n");

	return 1;
}

void shutdown_spu()
{
	sysSpuRawDestroy(spu_id);
	sysSpuImageClose(&spu_image);
}

void signal_spu_ppu()
{
	sysSpuRawWriteProblemStorage(spu_id,SPU_In_MBox,(u32)((u64)&spuparam));
}

void signal_spu_rsx()
{
	u32 offset;
	u32 data[4] = { 0xbeefbeef,0xbeefbeef,0xbeefbeef,0xbeefbeef };

	rsxAddressToOffset((void*)(intptr_t)spuparam.spu_read_label_addr,&offset);
	rsxInlineTransfer(context,offset,data,4,CELL_GCM_LOCATION_CELL);
}

void wait_signal_spu()
{
	rsxSetWaitLabel(context,64,0xabcdabcd);
}

void clear_signal_spu()
{
	rsxSetWriteCommandLabel(context,64,0xdddddddd);
}
