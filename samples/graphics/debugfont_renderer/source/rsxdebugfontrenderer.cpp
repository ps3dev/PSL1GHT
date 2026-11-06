#include "rsxdebugfontrenderer.h"

#include "vpshader_dbgfont_vpo.h"
#include "fpshader_dbgfont_fpo.h"

u8* RSXDebugFontRenderer::spTextureData;

CellGcmContextData* RSXDebugFontRenderer::mContext = NULL;

u8* RSXDebugFontRenderer::mpTexture = NULL;
u8* RSXDebugFontRenderer::mPosition = NULL;
u8* RSXDebugFontRenderer::mTexCoord = NULL;
u8* RSXDebugFontRenderer::mColor = NULL;

rsxProgramAttrib* RSXDebugFontRenderer::mPosIndex = NULL;
rsxProgramAttrib* RSXDebugFontRenderer::mTexIndex = NULL;
rsxProgramAttrib* RSXDebugFontRenderer::mColIndex = NULL;
rsxProgramAttrib* RSXDebugFontRenderer::mTexUnit = NULL;

rsxVertexProgram* RSXDebugFontRenderer::mRSXVertexProgram;
rsxFragmentProgram* RSXDebugFontRenderer::mRSXFragmentProgram;

void* RSXDebugFontRenderer::mVertexProgramUCode;
void* RSXDebugFontRenderer::mFragmentProgramUCode;

vu32* RSXDebugFontRenderer::mLabel = NULL;
u32 RSXDebugFontRenderer::mLabelValue = 0;

u32 RSXDebugFontRenderer::mFragmentProgramOffset;
u32 RSXDebugFontRenderer::mTextureOffset;
u32 RSXDebugFontRenderer::mPositionOffset;
u32 RSXDebugFontRenderer::mTexCoordOffset;
u32 RSXDebugFontRenderer::mColorOffset;

RSXDebugFontRenderer::RSXDebugFontRenderer() : DebugFontRenderer()
{

}

RSXDebugFontRenderer::RSXDebugFontRenderer(CellGcmContextData *context) : DebugFontRenderer()
{
	mContext = context;
}

RSXDebugFontRenderer::~RSXDebugFontRenderer()
{

}

void RSXDebugFontRenderer::initShader()
{
	mRSXVertexProgram = (rsxVertexProgram*)vpshader_dbgfont_vpo;
	mRSXFragmentProgram = (rsxFragmentProgram*)fpshader_dbgfont_fpo;

	void *ucode;
	u32 ucodeSize;

	rsxFragmentProgramGetUCode(mRSXFragmentProgram, &ucode, &ucodeSize);

	mFragmentProgramUCode = rsxMemalign(64, ucodeSize);
	rsxAddressToOffset(mFragmentProgramUCode, &mFragmentProgramOffset);

	memcpy(mFragmentProgramUCode, ucode, ucodeSize);

	rsxVertexProgramGetUCode(mRSXVertexProgram, &mVertexProgramUCode, &ucodeSize);
}

void RSXDebugFontRenderer::init()
{
	mLabel = (vu32*) cellGcmGetLabelAddress(sLabelId);
	*mLabel = mLabelValue;

	initShader();

	mPosIndex = rsxVertexProgramGetAttrib(mRSXVertexProgram, "position");
	mTexIndex = rsxVertexProgramGetAttrib(mRSXVertexProgram, "texcoord");
	mColIndex = rsxVertexProgramGetAttrib(mRSXVertexProgram, "color");

	mTexUnit = rsxFragmentProgramGetAttrib(mRSXFragmentProgram, "texture");

	spTextureData = (u8*)rsxMemalign(128, DEBUGFONT_DATA_SIZE);
	mpTexture = (u8*)(intptr_t)rsxAlign(128, (intptr_t)spTextureData);

	u8 *pFontData = (u8*)getFontData();

	for(s32 i=0;i < DEBUGFONT_DATA_SIZE;i++)
		mpTexture[i] = pFontData[i];

	rsxAddressToOffset(mpTexture, &mTextureOffset);

	mPosition = (u8*)rsxMemalign(128, DEBUGFONT_MAX_CHAR_COUNT*NUM_VERTS_PER_GLYPH*sizeof(f32)*3);
	mTexCoord = (u8*)rsxMemalign(128, DEBUGFONT_MAX_CHAR_COUNT*NUM_VERTS_PER_GLYPH*sizeof(f32)*2);
	mColor = (u8*)rsxMemalign(128, DEBUGFONT_MAX_CHAR_COUNT*NUM_VERTS_PER_GLYPH*sizeof(f32)*4);

	rsxAddressToOffset(mPosition, &mPositionOffset);
	rsxAddressToOffset(mTexCoord, &mTexCoordOffset);
	rsxAddressToOffset(mColor, &mColorOffset);
}

void RSXDebugFontRenderer::shutdown()
{

}

void RSXDebugFontRenderer::printStart(f32 r, f32 g, f32 b, f32 a)
{
	sR = r;
	sG = g;
	sB = b;
	sA = a;

	rsxSetBlendFunc(mContext, CELL_GCM_SRC_ALPHA, CELL_GCM_ONE_MINUS_SRC_ALPHA, CELL_GCM_SRC_ALPHA, CELL_GCM_ONE_MINUS_SRC_ALPHA);
	rsxSetBlendEquation(mContext, CELL_GCM_FUNC_ADD, CELL_GCM_FUNC_ADD);
	rsxSetBlendEnable(mContext, CELL_GCM_TRUE);
	rsxSetLogicOpEnable(mContext, CELL_GCM_FALSE);

	rsxSetDepthTestEnable(mContext, CELL_GCM_FALSE);

	rsxLoadVertexProgram(mContext, mRSXVertexProgram, mVertexProgramUCode);
	rsxLoadFragmentProgramLocation(mContext, mRSXFragmentProgram, mFragmentProgramOffset, CELL_GCM_LOCATION_RSX);

	CellGcmTexture tex;
	tex.format = CELL_GCM_TEXTURE_FORMAT_B8|CELL_GCM_TEXTURE_FORMAT_LIN;
	tex.mipmap = 1;
	tex.dimension = CELL_GCM_TEXTURE_DIMS_2D;
	tex.cubemap = CELL_GCM_FALSE;
	tex.remap = CELL_GCM_TEXTURE_REMAP_TYPE_REMAP<<CELL_GCM_TEXTURE_REMAP_TYPE_B_SHIFT |
				CELL_GCM_TEXTURE_REMAP_TYPE_REMAP<<CELL_GCM_TEXTURE_REMAP_TYPE_G_SHIFT |
				CELL_GCM_TEXTURE_REMAP_TYPE_REMAP<<CELL_GCM_TEXTURE_REMAP_TYPE_R_SHIFT |
				CELL_GCM_TEXTURE_REMAP_TYPE_REMAP<<CELL_GCM_TEXTURE_REMAP_TYPE_A_SHIFT |
				CELL_GCM_TEXTURE_REMAP_COLOR_B<<CELL_GCM_TEXTURE_REMAP_COLOR_B_SHIFT |
				CELL_GCM_TEXTURE_REMAP_COLOR_B<<CELL_GCM_TEXTURE_REMAP_COLOR_G_SHIFT |
				CELL_GCM_TEXTURE_REMAP_COLOR_B<<CELL_GCM_TEXTURE_REMAP_COLOR_R_SHIFT |
				CELL_GCM_TEXTURE_REMAP_COLOR_B<<CELL_GCM_TEXTURE_REMAP_COLOR_A_SHIFT;
	tex.width = DEBUGFONT_TEXTURE_WIDTH;
	tex.height = DEBUGFONT_TEXTURE_HEIGHT;
	tex.depth = 1;
	tex.pitch = DEBUGFONT_TEXTURE_WIDTH;
	tex.location = CELL_GCM_LOCATION_RSX;
	tex.offset = mTextureOffset;
	rsxLoadTexture(mContext, mTexUnit->index, &tex);

	rsxTextureControl(mContext, mTexUnit->index, CELL_GCM_TRUE, 0<<8, 12<<8, CELL_GCM_TEXTURE_MAX_ANISO_1);
	rsxTextureFilter(mContext, mTexUnit->index, 0, CELL_GCM_TEXTURE_NEAREST_MIPMAP_LINEAR, CELL_GCM_TEXTURE_LINEAR, CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX);
	rsxTextureWrapMode(mContext, mTexUnit->index, CELL_GCM_TEXTURE_REPEAT, CELL_GCM_TEXTURE_REPEAT, CELL_GCM_TEXTURE_REPEAT, CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, CELL_GCM_TEXTURE_ZFUNC_LESS, 0);
}

void RSXDebugFontRenderer::printPass(DebugFont::Position *pPositions, DebugFont::TexCoord *pTexCoords, DebugFont::Color *pColors, s32 numVerts)
{
	while(*mLabel != mLabelValue)
		usleep(10);
	mLabelValue++;

	memcpy(mPosition, pPositions, numVerts*sizeof(f32)*3);
	memcpy(mTexCoord, pTexCoords, numVerts*sizeof(f32)*2);
	memcpy(mColor, pColors, numVerts*sizeof(f32)*4);

	rsxBindVertexArrayAttrib(mContext, mPosIndex->index, 0, mPositionOffset, sizeof(f32)*3, 3, CELL_GCM_VERTEX_DATA_TYPE_F32, CELL_GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(mContext, mTexIndex->index, 0, mTexCoordOffset, sizeof(f32)*2, 2, CELL_GCM_VERTEX_DATA_TYPE_F32, CELL_GCM_LOCATION_RSX);
	rsxBindVertexArrayAttrib(mContext, mColIndex->index, 0, mColorOffset, sizeof(f32)*4, 4, CELL_GCM_VERTEX_DATA_TYPE_F32, CELL_GCM_LOCATION_RSX);

	rsxDrawVertexArray(mContext, DEBUGFONT_PRIMITIVE, 0, numVerts);
	rsxInvalidateVertexCache(mContext);
	rsxSetWriteBackendLabel(mContext, sLabelId, mLabelValue);

	rsxFlushBuffer(mContext);
}

void RSXDebugFontRenderer::printEnd()
{
	rsxSetDepthTestEnable(mContext, CELL_GCM_TRUE);
	rsxSetBlendEnable(mContext, CELL_GCM_FALSE);
}
