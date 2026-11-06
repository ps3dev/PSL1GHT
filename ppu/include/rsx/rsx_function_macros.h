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
	s32 __attribute__((noinline)) rsxContextCallback(CellGcmContextData *context,u32 count)
	{
		register s32 result asm("r3");
		__asm__ __volatile__ (
            "stdu	1,-128(1)\n"
            "mr		31,2\n"
            "lwz	0,0(%3)\n"
            "lwz	2,4(%3)\n"
            "mtctr	0\n"
            "bctrl\n"
            "mr		2,31\n"
            "addi	1,1,128\n"
            : "+r"(result)
            : "r"(context), "r"(count), "b"(context->callback)
            : "r31", "r0", "lr"
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
