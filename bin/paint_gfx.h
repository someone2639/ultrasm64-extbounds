#include <sm64.h>

#include "src/game/paint.h"

u8 txt_paint_banner[] =
{
#include "banner.i4.h"
};

u8 txt_paint_clear[] =
{
#include "clear.i4.h"
};

u16 txt_paint_done[] =
{
#include "done.rgba16.h"
};

u8 txt_paint_tool[] =
{
#include "tool.ci4.h"
};

u16 pal_paint_tool[] =
{
#include "tool.rgba16.h"
};

Gfx gfx_paint_start[] =
{
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
	gsSPSetOtherMode(
		G_SETOTHERMODE_H, 0, 24,
		G_PM_NPRIMITIVE |
		G_CYC_1CYCLE |
		G_TP_NONE |
		G_TD_CLAMP |
		G_TL_TILE |
		G_TT_NONE |
		G_TF_BILERP |
		G_TC_FILT |
		G_CK_NONE |
		G_CD_MAGICSQ |
		G_AD_DISABLE |
		15
	),
	gsSPSetOtherMode(
		G_SETOTHERMODE_L, 0, 32,
		G_RM_OPA_SURF | G_RM_OPA_SURF2 |
		G_ZS_PIXEL |
		G_AC_NONE
	),
	gsSPEndDisplayList(),
};

Gfx gfx_paint_done_clear[] =
{
	gsDPPipeSync(),
	gsDPSetTextureFilter(G_TF_POINT),

	gsDPLoadTextureBlock(txt_paint_done, G_IM_FMT_RGBA, G_IM_SIZ_16b, PAINT_DONE_TW, PAINT_DONE_TH, 0, G_TX_WRAP, G_TX_WRAP, 5, 4, 0, 0),
	gsSPTextureRectangle(4*(PAINT_DONE_X), 4*(PAINT_DONE_Y), 4*(PAINT_DONE_X+PAINT_DONE_W), 4*(PAINT_DONE_Y+PAINT_DONE_H), G_TX_RENDERTILE, 0, 0, 0x400, 0x400),

	gsDPLoadTextureBlock_4b(txt_paint_clear, G_IM_FMT_I, PAINT_CLEAR_TW, PAINT_DONE_TH, 0, G_TX_WRAP, G_TX_WRAP, 4, 4, 0, 0),
	gsSPTextureRectangle(4*(PAINT_CLEAR_X), 4*(PAINT_CLEAR_Y), 4*(PAINT_CLEAR_X+PAINT_CLEAR_W), 4*(PAINT_CLEAR_Y+PAINT_CLEAR_H), G_TX_RENDERTILE, 0, 0, 0x400, 0x400),

	gsSPEndDisplayList(),
};

Gfx gfx_paint_pal[] =
{
	gsDPPipeSync(),
	gsDPSetTextureLUT(G_TT_RGBA16),

	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0),
	gsDPSetTile(0, 0, 0, 256, 6, 0, 0, 0, 0, 0, 0, 0),
	gsDPLoadSync(),
	gsDPLoadTLUTCmd(6, 15),
	gsSPEndDisplayList(),
};

Gfx gfx_paint_txt[] =
{
	gsDPLoadSync(),
	gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((PAINT_CANVAS_TW)*(PAINT_CANVAS_TH)+3)>>2)-1, CALC_DXT_4b(PAINT_CANVAS_TW)),
	gsDPPipeSync(),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_4b, ((((PAINT_CANVAS_TW)>>1)+7)>>3), 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 6, 0, G_TX_WRAP, 6, 0),
	gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, ((PAINT_CANVAS_TW)-1) << G_TEXTURE_IMAGE_FRAC, ((PAINT_CANVAS_TH)-1) << G_TEXTURE_IMAGE_FRAC),
	gsSPTextureRectangle(4*(PAINT_CANVAS_X), 4*(PAINT_CANVAS_Y), 4*(PAINT_CANVAS_X+PAINT_CANVAS_W), 4*(PAINT_CANVAS_Y+PAINT_CANVAS_H), G_TX_RENDERTILE, 0, 0, 0x200, 0x200),

	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_16b, 1, txt_paint_tool),
	gsDPLoadSync(),
	gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (((PAINT_TOOL_TW)*(PAINT_TOOL_TH)+3)>>2)-1, CALC_DXT_4b(PAINT_TOOL_TW)),
	gsDPPipeSync(),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_4b, ((((PAINT_TOOL_TW)>>1)+7)>>3), 0, 0, 0, G_TX_WRAP, 7, 0, G_TX_WRAP, 5, 0),
	gsDPSetTileSize(0, 0, 0, ((PAINT_TOOL_TW)-1) << G_TEXTURE_IMAGE_FRAC, ((PAINT_TOOL_TH)-1) << G_TEXTURE_IMAGE_FRAC),

	gsSPEndDisplayList(),
};

Gfx gfx_paint_fill[] =
{
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
	gsDPSetPrimColor(0, 0, 0, 0, 0, 0),
	gsDPFillRectangle(PAINT_CANVAS_X-1, PAINT_CANVAS_Y-1, PAINT_CANVAS_X+PAINT_CANVAS_W+1, PAINT_CANVAS_Y),
	gsDPFillRectangle(PAINT_CANVAS_X-1, PAINT_CANVAS_Y+PAINT_CANVAS_H, PAINT_CANVAS_X+PAINT_CANVAS_W+1, PAINT_CANVAS_Y+PAINT_CANVAS_H+1),
	gsDPFillRectangle(PAINT_CANVAS_X-1, PAINT_CANVAS_Y, PAINT_CANVAS_X, PAINT_CANVAS_Y+PAINT_CANVAS_H),
	gsDPFillRectangle(PAINT_CANVAS_X+PAINT_CANVAS_W, PAINT_CANVAS_Y, PAINT_CANVAS_X+PAINT_CANVAS_W+1, PAINT_CANVAS_Y+PAINT_CANVAS_H),
	gsSPEndDisplayList(),
};

Gfx gfx_paint_fade[] =
{
	gsDPPipeSync(),
	gsDPPipelineMode(G_PM_1PRIMITIVE),
	gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
	gsDPFillRectangle(0, 0, SCREEN_WD, SCREEN_HT),
	gsSPEndDisplayList(),
};
