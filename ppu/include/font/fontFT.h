#ifndef __LV2_FONTFT_H__
#define __LV2_FONTFT_H__

#include <ppu-types.h>

#include <font/font.h>

#define FONT_LIBRARY_TYPE_FREETYPE  			2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _font_library_configFT
{
	void *library ATTRIBUTE_PRXPTR;
	CellFontMemoryInterface memoryIF;
} CellFontLibraryConfigFT;

typedef struct _font_renderer_configFT
{
	struct {
		void *buffer ATTRIBUTE_PRXPTR;
		u32 initSize;
		u32 maxSize;
		u32 expandSize;
		u32 resetSize;
	} bufferingPolicy;
} CellFontRendererConfigFT;

static inline void cellFontLibraryConfigFT_initialize(CellFontLibraryConfigFT *config)
{
	config->library = NULL;
	cellFontMemoryInterface_initialize(&config->memoryIF);
}

s32 cellFontInitLibraryFreeType(CellFontLibraryConfigFT *config,const CellFontLibrary **lib);

#ifdef __cplusplus
	}
#endif

#endif
