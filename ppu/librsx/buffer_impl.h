/**
 * @brief Resets the RSX command ring buffer and rewinds host write pointer.
 *
 * Emits a hardware JUMP command directing RSX FIFO execution back to the base
 * offset (0x1000) within the mapped host IO area. Waits until the RSX GET pointer
 * acknowledges the jump, then rewinds context->current back to context->begin so
 * subsequent CPU command emissions resume cleanly from the start of the buffer.
 *
 * @param context Pointer to active gcmContextData structure.
 */
void RSX_FUNC(ResetCommandBuffer)(gcmContextData *context)
{
	u32 offset = 0;
	if (gcmAddressToOffset(context->begin, &offset) != 0)
		offset = 0x1000;

	RSX_FUNC(SetJumpCommand)(context, offset);

	/* Flush the jump command so RSX FIFO executes up to and through the jump */
	RSX_FUNC(FlushBuffer)(context);

	gcmControlRegister volatile *ctrl = gcmGetControlRegister(context);
	uint32_t spins = 0;
	while(ctrl->get != offset && spins < 100000) {
		usleep(30);
		spins++;
	}

	__sync();
	ctrl->put = offset;

	/* Synchronize host write pointer back to the buffer start */
	context->current = context->begin;
}

void RSX_FUNC(FlushBuffer)(gcmContextData *context)
{
	u32 offset = 0;
	gcmControlRegister volatile *ctrl = gcmGetControlRegister(context);
	
	__sync();
	gcmAddressToOffset(context->current,&offset);
	ctrl->put = offset;
}

void RSX_FUNC(Finish)(gcmContextData *context,u32 ref_value)
{
	RSX_FUNC(SetReferenceCommand)(context,ref_value);
	RSX_FUNC(FlushBuffer)(context);

	gcmControlRegister volatile *ctrl = gcmGetControlRegister(context);
	while(ctrl->ref!=ref_value) usleep(30);
}

