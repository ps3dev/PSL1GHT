#ifndef __RSXDEBUGFONTRENDERER_H__
#define __RSXDEBUGFONTRENDERER_H__

#include <debugfontrenderer.h>

#include <rsx/rsx.h>

#ifdef DEBUGFONT_USE_QUADS
#define DEBUGFONT_PRIMITIVE					CELL_GCM_TYPE_QUADS
#define NUM_VERTS_PER_GLYPH					4
#else
#define DEBUGFONT_PRIMITIVE					CELL_GCM_TYPE_TRIANGLES
#define NUM_VERTS_PER_GLYPH					6
#endif

class RSXDebugFontRenderer : public DebugFontRenderer
{
public:
	RSXDebugFontRenderer();
	RSXDebugFontRenderer(CellGcmContextData *context);
	virtual ~RSXDebugFontRenderer();

	virtual void init();
	virtual void shutdown();
	virtual void printStart(f32 r, f32 g, f32 b, f32 a);
	virtual void printPass(DebugFont::Position *pPositions, DebugFont::TexCoord *pTexCoords, DebugFont::Color *pColors, s32 numVerts);
	virtual void printEnd();

private:
	void initShader();

	static CellGcmContextData *mContext;

	static u8 *spTextureData;

	static u8 *mPosition;
	static u8 *mTexCoord;
	static u8 *mColor;

	static u32 mPositionOffset;
	static u32 mTexCoordOffset;
	static u32 mColorOffset;

	static u8 *mpTexture;
	static rsxProgramAttrib *mPosIndex;
	static rsxProgramAttrib *mTexIndex;
	static rsxProgramAttrib *mColIndex;
	static rsxProgramAttrib *mTexUnit;

	static rsxVertexProgram *mRSXVertexProgram;
	static rsxFragmentProgram *mRSXFragmentProgram;

	static u32 mFragmentProgramOffset;
	static u32 mTextureOffset;

	static void *mVertexProgramUCode;				// this is sysmem
	static void *mFragmentProgramUCode;			// this is vidmem

	static vu32 *mLabel;
	static u32 mLabelValue;

	static const u32 sLabelId = 254;
};

#endif // __RSXDEBUGFONTRENDERER_H__