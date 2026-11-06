#include <ppu-types.h>
#include <spurs/spurs.h>
#define	SPURS_ATTRIBUTE_REVISION	(0x02)
#define SPURS_INTERNAL_VERSION    0x330000
extern s32 cellSpursAttributeInitializeEx(CellSpursAttribute *,u32 revision,u32 sdkVersion,u8 nSpus,s32 spuPriority,s32 ppuPriority,bool exitIfNoWork);

s32 cellSpursAttributeInitialize(CellSpursAttribute *attr, u8 nSpus,s32 spuPriority,s32 ppuPriority,bool exitIfNoWork)
{
	return cellSpursAttributeInitializeEx(attr,SPURS_ATTRIBUTE_REVISION,SPURS_INTERNAL_VERSION,nSpus,spuPriority,ppuPriority,exitIfNoWork);
	
}
