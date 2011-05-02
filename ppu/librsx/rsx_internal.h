#ifndef __RSX_INTERNAL_H__
#define __RSX_INTERNAL_H__

#include <ppc-asm.h>
#include <ppu-types.h>

#define RSX_METHOD_FLAG_NO_INCREMENT			(0x40000000)
#define RSX_METHOD_FLAG_JUMP					(0x20000000)
#define RSX_METHOD_FLAG_CALL					(0x00000002)
#define RSX_METHOD_FLAG_RETURN					(0x00020000)

#define RSX_MAX_METHOD_COUNT					0x7ff

#define RSX_CONTEXT_CURRENTP					(context->current)

#define RSX_CONTEXT_CURRENT_BEGIN(count) do { \
	if((context->current + (count)) > context->end) { \
		if(rsxContextCallback(context,(count))!=0) return; \
	} \
} while(0)

#define RSX_CONTEXT_CURRENT_END(x)				context->current += (x)

#define RSX_METHOD_COUNT_SHIFT					(18)
#define RSX_METHOD(method,count)				(((count)<<RSX_METHOD_COUNT_SHIFT)|(method))

#define RSX_SUBCHANNEL_SHIFT						(13)
#define RSX_SUBCHANNEL_METHOD(channel,method,count)	(((count)<<RSX_METHOD_COUNT_SHIFT)|((channel)<<RSX_SUBCHANNEL_SHIFT)|(method))

s32 rsxContextCallback(gcmContextData *context,u32 count);

#endif
