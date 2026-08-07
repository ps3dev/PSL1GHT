void RSX_FUNC(ResetCommandBuffer)(CellGcmContextData *context)
{
	u32 offset = 0x1000;			// init state offset;
	RSX_FUNC(SetJumpCommand)(context,offset);

	__sync();

	CellGcmControlRegister volatile *ctrl = cellGcmGetControlRegister();
	ctrl->put = offset;
	while(ctrl->get!=offset) usleep(30);
}

void RSX_FUNC(FlushBuffer)(CellGcmContextData *context)
{
	u32 offset = 0;
	CellGcmControlRegister volatile *ctrl = cellGcmGetControlRegister();
	
	__sync();
	cellGcmAddressToOffset(context->current,&offset);
	ctrl->put = offset;
}

void RSX_FUNC(Finish)(CellGcmContextData *context,u32 ref_value)
{
	RSX_FUNC(SetReferenceCommand)(context,ref_value);
	RSX_FUNC(FlushBuffer)(context);

	CellGcmControlRegister volatile *ctrl = cellGcmGetControlRegister();
	while(ctrl->ref!=ref_value) usleep(30);
}

