#ifndef __LV2_FONT_H__
#define __LV2_FONT_H__

#include <ppu-types.h>

#include <font/fontset.h>

#define FONT_LIBRARY_TYPE_NONE 				0

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*CellFontMallocCallback)(void *object,u32 size);
typedef void  (*CellFontFreeCallback)(void *object,void *ptr);
typedef void* (*CellFontReallocCallback)(void *object,void *p,u32 reallocSize);
typedef void* (*CellFontCallocCallback)(void *object,u32 num,u32 size);

typedef struct _font_memory_interface
{
	void *object ATTRIBUTE_PRXPTR;
	CellFontMallocCallback malloc_func ATTRIBUTE_PRXPTR;
	CellFontFreeCallback free_func ATTRIBUTE_PRXPTR;
	CellFontReallocCallback realloc_func ATTRIBUTE_PRXPTR;
	CellFontCallocCallback calloc_func ATTRIBUTE_PRXPTR;	
} CellFontMemoryInterface;

typedef struct _font_entry
{
	u32 lock;
	u32 uniqueId;
	const void* fontLib ATTRIBUTE_PRXPTR;
	void* fontH ATTRIBUTE_PRXPTR;
} CellFontEntry;

typedef struct _font_config
{
	struct {
		u32 *buffer ATTRIBUTE_PRXPTR;
		u32 size;
	} fileCache;
	
	u32 userFontEntryMax;
	CellFontEntry *userFontEntries ATTRIBUTE_PRXPTR;
	u32 flags;
} CellFontConfig;

typedef struct _font_library
{
	u32 libraryType, libraryVersion;
	u32 systemClosed[];
} CellFontLibrary;

typedef struct _font_type
{
	u32 type;
	u32 map;
} CellFontType;

typedef struct _font
{
	void* ATTRIBUTE_PRXPTR systemReserved[64];
} CellFont;

typedef struct _font_renderer
{
	void* ATTRIBUTE_PRXPTR systemReserved[64];
} CellFontRenderer;

typedef struct _font_renderer_config
{
	struct {
		void* buffer ATTRIBUTE_PRXPTR;
		u32 initSize;
		u32 maxSize;
		u32 expandSize;
		u32 resetSize;
	} bufferingPolicy;
} CellFontRendererConfig;

typedef struct _font_horizontal_layout
{
	f32 baseLineY;
	f32 lineHeight;
	f32 effectHeight;
} CellFontHorizontalLayout;

typedef struct _font_vertical_layout
{
	f32 baseLineX;
	f32 lineWidth;
	f32 effectWidth;
} CellFontVerticalLayout;

typedef struct _font_glyph_metrics
{
	f32 width;
	f32 height;

	struct {
		f32 bearingX;
		f32 bearingY;
		f32 advance;
	} horizontal;
	struct {
		f32 bearingX;
		f32 bearingY;
		f32 advance;
	} vertical;
} CellFontGlyphMetrics;

typedef struct _font_glyph_outline
{
	s16 contoursCount;
	s16 pointsCount;
	
	struct {
		f32 x, y;
	} *Points ATTRIBUTE_PRXPTR;
	
	u8 * pointTags ATTRIBUTE_PRXPTR;
	u16* contourIndexs ATTRIBUTE_PRXPTR;
	u32 flags;
	void* generateEnv ATTRIBUTE_PRXPTR;
} CellFontGlyphOutline;

typedef struct _font_glyph
{
	u16 cf_type, type;
	u32 size;
	CellFontGlyphMetrics metrics;
	CellFontGlyphOutline outline;
} CellFontGlyph;

typedef struct _font_kerning
{
	f32 offsetX;
	f32 offsetY;
} CellFontKerning;

typedef struct _font_glyph_style
{
	struct {
		f32 widthPixel;
		f32 heightPixel;
	} Scale;
	struct {
		f32 weight;
		f32 slant;
	} Effect;
} CellFontGlyphStyle;

typedef struct _font_render_surface
{
	void* buffer ATTRIBUTE_PRXPTR;
	s32 widthByte;
	s32 pixelSizeByte;
	s32 width, height;
	struct {
		u32 x0, y0;
		u32 x1, y1;
	} Scissor;
} CellFontRenderSurface;

typedef struct _font_image_trans_info
{
	u8 *image ATTRIBUTE_PRXPTR;
	u32 imageWidthByte;
	u32 imageWidth;
	u32 imageHeight;
	void *surface ATTRIBUTE_PRXPTR;
	u32 surfWidthByte;
} CellFontImageTransInfo;

static inline void cellFontMemoryInterface_initialize(CellFontMemoryInterface *mIF)
{
	mIF->object       = NULL;
	mIF->malloc_func  = NULL;
	mIF->free_func    = NULL;
	mIF->realloc_func = NULL;
	mIF->calloc_func  = NULL;
}

static inline void cellFontConfig_initialize(CellFontConfig *config)
{
	config->fileCache.buffer = NULL;
	config->fileCache.size   = 0;
	config->userFontEntryMax = 0;
	config->userFontEntries  = NULL;
	config->flags 			 = 0x00000000;
}

static inline s32 cellFontInit(CellFontConfig *config)
{
	extern void cellFontGetStubRevisionFlags(u64 *revisionFlags);
	extern s32 cellFontInitializeWithRevision(u64 revision, CellFontConfig *config);
	u64 revisionFlags = 0LL;
	
	cellFontGetStubRevisionFlags(&revisionFlags);
	return cellFontInitializeWithRevision(revisionFlags, config);
}

s32 cellFontOpenFontset(const CellFontLibrary *lib,CellFontType *type,CellFont *f);
s32 cellFontOpenFontsetOnMemory(const CellFontLibrary *lib,CellFontType *type,CellFont *f);
s32 cellFontOpenFontFile(const CellFontLibrary *lib,const char *fontPath,u32 subNum,s32 uniqueID,CellFont *f);
s32 cellFontOpenFontMemory(const CellFontLibrary *lib,void *fontAddr,u32 fontSize,u32 subNum,s32 uniqueID,CellFont *f);
s32 cellFontOpenFontInstance(CellFont *openedFont,CellFont *f);
s32 cellFontGetLibrary(CellFont *f,const CellFontLibrary **lib,u32 *type);
s32 cellFontAdjustGlyphExpandBuffer(CellFont *f,s32 pointN,s32 contourN);
s32 cellFontGetGlyphExpandBufferInfo(CellFont *f,s32 *pointN,s32 *contourN);
s32 cellFontAdjustFontScaling(CellFont *f,f32 fontScale);
s32 cellFontSetResolutionDpi(CellFont *f,u32 hDpi,u32 vDpi);
s32 cellFontSetScalePoint(CellFont *f,f32 w,f32 h);
s32 cellFontontSetScalePixel(CellFont *f,f32 w,f32 h);
s32 cellFontSetEffectWeight(CellFont *f,f32 effectWeight);
s32 cellFontSetEffectSlant(CellFont *f,f32 effectSlant);
s32 cellFontGetResolutionDpi(CellFont *f,u32 *hDpi,u32 *vDpi);
s32 cellFontGetScalePoint(CellFont *f,f32 *w,f32 *h);
s32 cellFontGetScalePixel(CellFont *f,f32 *w,f32 *h);
s32 cellFontGetEffectWeight(CellFont *f,f32 *effectWeight);
s32 cellFontGetEffectSlant(CellFont *f,f32 *effectSlant);
s32 cellFontGetHorizontalLayout(CellFont *f,CellFontHorizontalLayout *layout);
s32 cellFontGetVerticalLayout(CellFont *f,CellFontVerticalLayout *layout);
s32 cellFontGetFontIdCode(CellFont *f,u32 code,u32 *fontId,u32 *fontcode);
s32 cellFontGetCharGlyphMetrics(CellFont *f,u32 code,CellFontGlyphMetrics *metrics);
s32 cellFontGetCharGlyphMetricsVertical(CellFont *f,u32 code,CellFontGlyphMetrics *metrics);
s32 cellFontGetKerning(CellFont *f,u32 preCode,u32 code,CellFontKerning *kerning);
s32 cellFontCreateRenderer(const CellFontLibrary *lib,CellFontRendererConfig *confing,CellFontRenderer *renderer);
s32 cellFontBindRenderer(CellFont *f,CellFontRenderer *renderer);
s32 cellFontGetBindingRenderer(CellFont *f,CellFontRenderer **renderer);
s32 cellFontSetupRenderScalePoint(CellFont *f,f32 w,f32 h);
s32 cellFontSetupRenderScalePixel(CellFont *f,f32 w,f32 h);
s32 cellFontSetupRenderEffectWeight(CellFont *f,f32 additionalWeight);
s32 cellFontSetupRenderEffectSlant(CellFont *f,f32 effectSlant);
s32 cellFontGetRenderScalePoint(CellFont *f,f32 *w,f32 *h);
s32 cellFontGetRenderScalePixel(CellFont *f,f32 *w,f32 *h);
s32 cellFontGetRenderEffectWeight(CellFont *f,f32 *effectWeight);
s32 cellFontGetRenderEffectSlant(CellFont *f,f32 *effectSlant);
s32 cellFontGetRenderCharGlyphMetrics(CellFont *f,u32 code,CellFontGlyphMetrics *metrics);
s32 cellFontGetRenderCharGlyphMetricsVertical(CellFont *cfEx,u32 code,CellFontGlyphMetrics *metrics);
s32 cellFontGetRenderScaledKerning(CellFont *f,u32 preCode,u32 code,CellFontKerning *kerning);
s32 cellFontGenerateCharGlyph(CellFont *f,u32 code,CellFontGlyph **glyph);
s32 cellFontGenerateCharGlyphVertical(CellFont *f,u32 code,CellFontGlyph **glyph);
s32 cellFontDeleteGlyph(CellFont *f,CellFontGlyph *glyph);
s32 cellFontDelete(const CellFontLibrary *library,void *p);
void cellFontRenderSurfaceInit(CellFontRenderSurface *surface,void *buffer,s32 bufWidthByte,s32 pixelSizeByte,s32 w,s32 h);
void cellFontRenderSurfaceSetScissor(CellFontRenderSurface *surface,s32 x0,s32 y0,u32 w,u32 h);
s32 cellFontRenderCharGlyphImage(CellFont *f,u32 code,CellFontRenderSurface *surface,f32 x,f32 y,CellFontGlyphMetrics *metrics,CellFontImageTransInfo *transInfo);
s32 cellFontRenderCharGlyphImageHorizontal(CellFont *f,u32 code,CellFontRenderSurface *surface,f32 x,f32 y,CellFontGlyphMetrics *metrics,CellFontImageTransInfo *transInfo);
s32 cellFontRenderCharGlyphImageVertical(CellFont *f,u32 code,CellFontRenderSurface *surface,f32 x,f32 y,CellFontGlyphMetrics *metrics,CellFontImageTransInfo *transInfo);
s32 cellFontUnbindRenderer(CellFont *f);
s32 cellFontGlyphRenderImage(CellFontGlyph *glyph,CellFontGlyphStyle *style,CellFontRenderer *renderer,CellFontRenderSurface *surface,f32 x,f32 y,CellFontGlyphMetrics *metrics,CellFontImageTransInfo *transInfo);
s32 cellFontGlyphRenderImageHorizontal(CellFontGlyph *glyph,CellFontGlyphStyle *style,CellFontRenderer *renderer,CellFontRenderSurface *surface,f32 x,f32 y,CellFontGlyphMetrics *metrics,CellFontImageTransInfo *transInfo);
s32 cellFontGlyphRenderImageVertical(CellFontGlyph *glyph,CellFontGlyphStyle *style,CellFontRenderer *renderer,CellFontRenderSurface *surface,f32 x,f32 y,CellFontGlyphMetrics *metrics,CellFontImageTransInfo *transInfo);
s32 cellFontGlyphGetHorizontalShift(CellFontGlyph *glyph,f32 *shiftX,f32 *shiftY);
s32 cellFontGlyphGetVerticalShift(CellFontGlyph *glyph,f32 *shiftX,f32 *shiftY);
s32 cellFontDestroyRenderer(CellFontRenderer *renderer);
s32 cellFontCloseFont(CellFont *cf);
s32 cellFontEndLibrary(const CellFontLibrary *lib);
s32 cellFontEnd();

#ifdef __cplusplus
	}
#endif

#endif
