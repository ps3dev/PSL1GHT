#include <ppu-asm.h>
#include <ppu-types.h>

#include <font/font.h>

extern s32 cellFontGetLibraryEx(CellFont *f,const CellFontLibrary* ATTRIBUTE_PRXPTR *lib,u32 *type);
extern s32 cellFontGetBindingRendererEx(CellFont *f,CellFontRenderer* ATTRIBUTE_PRXPTR *renderer);
extern s32 cellFontGenerateCharGlyphEx(CellFont *f,u32 code,CellFontGlyph* ATTRIBUTE_PRXPTR *glyph);
extern s32 cellFontGenerateCharGlyphVerticalEx(CellFont *f,u32 code,CellFontGlyph* ATTRIBUTE_PRXPTR *glyph);

void cellFontGetStubRevisionFlags(u64 *revisionFlags)
{
	if(revisionFlags == NULL) return;
	*revisionFlags = 0x14;
}

s32 cellFontGetLibrary(CellFont *f,const CellFontLibrary **lib,u32 *type)
{
	s32 ret;
	const CellFontLibrary *l ATTRIBUTE_PRXPTR;
	
	if(f == NULL || lib == NULL || type == NULL) return 0x80540002;

	ret = cellFontGetLibraryEx(f,&l,type);
	*lib = ret == 0 ? l : NULL;
	
	return ret;	 
}

s32 cellFontGetBindingRenderer(CellFont *f,CellFontRenderer **renderer)
{
	s32 ret;
	CellFontRenderer *r ATTRIBUTE_PRXPTR;
	
	if(f == NULL || renderer == NULL) return 0x80540002;
	
	ret = cellFontGetBindingRendererEx(f,&r);
	*renderer = ret == 0 ? r : NULL;
	
	return ret;
}

s32 cellFontGenerateCharGlyph(CellFont *f,u32 code,CellFontGlyph **glyph)
{
	s32 ret;
	CellFontGlyph *g ATTRIBUTE_PRXPTR;
	
	if(f == NULL || glyph == NULL) return 0x80540002;
	
	ret = cellFontGenerateCharGlyphEx(f,code,&g);
	*glyph = ret == 0 ? g : NULL;
	
	return ret;
}

s32 cellFontGenerateCharGlyphVertical(CellFont *f,u32 code,CellFontGlyph **glyph)
{
	s32 ret;
	CellFontGlyph *g ATTRIBUTE_PRXPTR;
	
	if(f == NULL || glyph == NULL) return 0x80540002;
	
	ret = cellFontGenerateCharGlyphVerticalEx(f,code,&g);
	*glyph = ret == 0 ? g : NULL;
	
	return ret;
}



