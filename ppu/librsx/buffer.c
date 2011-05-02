#include <stdlib.h>
#include <unistd.h>
#include <rsx/gcm_sys.h>
#include <rsx/commands.h>
#include <ppu_intrinsics.h>

void rsxResetCommandBuffer(gcmContextData *context)
{
	u32 offset = 0x1000;			// init state offset;
	rsxSetJumpCommand(context,offset);

	__sync();

	gcmControlRegister volatile *ctrl = gcmGetControlRegister(context);
	ctrl->put = offset;
	while(ctrl->get!=offset) usleep(30);
}

void rsxFlushBuffer(gcmContextData *context)
{
	u32 offset = 0;
	gcmControlRegister volatile *ctrl = gcmGetControlRegister(context);
	
	__sync();
	gcmAddressToOffset(context->current,&offset);
	ctrl->put = offset;
}

void rsxFinish(gcmContextData *context,u32 ref_value)
{
	rsxSetReferenceCommand(context,ref_value);
	rsxFlushBuffer(context);

	gcmControlRegister volatile *ctrl = gcmGetControlRegister(context);
	while(ctrl->ref!=ref_value) usleep(30);
}

s32 __attribute__((always_inline)) rsxAddressToOffset(void *ptr,u32 *offset)
{
	return gcmAddressToOffset(ptr,offset);
}

