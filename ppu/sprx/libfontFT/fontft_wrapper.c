#include <ppu-asm.h>
#include <ppu-types.h>

#include <font/font.h>
#include <font/fontFT.h>

extern s32 cellFontInitLibraryFreeTypeWithRevision(u64 revisionFlags,CellFontLibraryConfigFT *config,const CellFontLibrary* ATTRIBUTE_PRXPTR *lib);

void cellFontFTGetStubRevisionFlags(u64 *revisionFlags)
{
	if(revisionFlags == NULL) return;
	*revisionFlags = 0x14;
}

s32 cellFontInitLibraryFreeType(CellFontLibraryConfigFT *config,const CellFontLibrary **lib)
{
	s32 ret;
	u64 revisionFlags = 0LL;
	const CellFontLibrary *l ATTRIBUTE_PRXPTR;
	
	if(config == NULL || lib == NULL) return 0x80540002;
	
	cellFontFTGetStubRevisionFlags(&revisionFlags);
	
	ret = cellFontInitLibraryFreeTypeWithRevision(revisionFlags,config,&l);
	*lib = ret == 0 ? l : NULL;
	
	return ret;
}
