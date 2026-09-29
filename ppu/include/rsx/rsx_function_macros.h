#ifdef RSX_FUNCTION_MACROS

#if RSX_UNSAFE
	#define RSX_FUNC(func)				rsx##func##Unsafe
#else
	#define RSX_FUNC(func)				rsx##func
#endif

#if RSX_INTERNAL

#if RSX_UNSAFE
	#define RSX_FUNC_INTERNAL(func)		__rsx##func##Unsafe
	#define RSX_CONTEXT_CURRENT_BEGIN(count) do {} while(0)	
#else
	/**
	 * @brief Dispatches the GCM command buffer wrap callback handler.
	 *
	 * When the command buffer current write pointer reaches or exceeds the end
	 * threshold, this function executes the user-registered callback handler via
	 * its 32-bit Procedure Descriptor (OPD).
	 *
	 * Architectural & ABI Invariants:
	 * - PowerPC 64-bit ELF ABI passes argument 1 (context) in r3 and argument 2 (count) in r4.
	 * - The callback returns an s32 status code in r3 (0 = success, non-zero = fail).
	 * - A 32-bit OPD descriptor layout places the 32-bit function entry address at offset 0
	 *   and the 32-bit callee Table of Contents (TOC) register value at offset 4.
	 * - Callers must preserve their own TOC (r2) across indirect calls; offset 40(r1) is the
	 *   standard ABI TOC save slot in the caller's 128-byte stack frame.
	 * - The called C function is permitted to clobber all volatile registers (r0, r4-r12,
	 *   lr, ctr, xer, cr0-cr1, cr5-cr7) and read/write arbitrary system memory. A full
	 *   volatile clobber list and "memory" clobber are mandatory to prevent compiler
	 *   register reuse and unsafe caching across the ring wrap boundary.
	 *
	 * @param context Pointer to the active RSX GCM context structure.
	 * @param count Number of 32-bit command words requested for reservation.
	 * @return s32 0 on success, non-zero if the buffer could not be reset.
	 */
	s32 __attribute__((noinline)) rsxContextCallback(gcmContextData *context, u32 count)
	{
		register s32 result asm("r3");
		register gcmContextData *r3_ctx asm("r3") = context;
		register u32 r4_cnt asm("r4") = count;
		register void *r12_cb asm("r12") = (void *)context->callback;

		__asm__ __volatile__ (
			"stdu	1,-128(1)\n"		/* Allocate 128-byte ABI frame and update backchain */
			"std	2,40(1)\n"			/* Save caller's TOC in standard ABI slot 40(r1) */
			"lwz	0,0(%3)\n"			/* Load 32-bit function entry point from OPD */
			"lwz	2,4(%3)\n"			/* Load 32-bit callee TOC into r2 */
			"mtctr	0\n"				/* Move target entry address to Count Register */
			"bctrl\n"					/* Branch to callback with link (r3=ctx, r4=count) */
			"ld		2,40(1)\n"			/* Restore caller's original TOC from slot 40(r1) */
			"addi	1,1,128\n"			/* Deallocate 128-byte ABI frame */
			: "=r"(result), "+r"(r4_cnt)
			: "0"(r3_ctx), "b"(r12_cb)
			: "r0", "r5", "r6", "r7", "r8", "r9", "r10", "r11",
			  "lr", "ctr", "xer", "cr0", "cr1", "cr5", "cr6", "cr7", "memory"
		);
		return result;
	}
	
	#define RSX_FUNC_INTERNAL(func)		__rsx##func
	#define RSX_CONTEXT_CURRENT_BEGIN(count) do { \
		if((context->current + (count)) > context->end) { \
			if(rsxContextCallback(context,(count))!=0) return; \
		} \
	} while(0)
#endif

#endif

#endif

#ifndef RSX_FUNCTION_MACROS
#undef RSX_CONTEXT_CURRENT_BEGIN
#undef RSX_FUNC
#undef RSX_FUNC_INTERNAL
#endif
