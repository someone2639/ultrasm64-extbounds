#include <ultra64.h>
#include <stdarg.h>
#include <stdio.h>

#include "debug_utils.h"
#include "draw_objects.h"
#include "dynlist_proc.h"
#include "dynlists/dynlists.h"
#include "gd_macros.h"
#include "gd_main.h"
#include "gd_math.h"
#include "gd_memory.h"
#include "gd_types.h"
#include "macros.h"
#include "objects.h"
#include "renderer.h"
#include "sfx.h"
#include "shape_helper.h"
#include "skin.h"
#include "types.h"

struct ObjView *sMSceneView; // Mario scene view
struct ObjGroup *sMarioSceneGrp;
struct ObjView *sHandView = NULL;
// two DLs, double buffered one per frame.
//  seem to be basic dls that branch to actual lists?
struct GdDisplayList *sMHeadMainDls[2];

// bss
static OSContStatus D_801BAE60[4];
static OSContPadEx sPrevFrameCont[4];
static u8 D_801BAEA0;
static struct ObjGadget *sTimerGadgets[GD_NUM_TIMERS];
static u32 D_801BAF28;                                 // RAM addr offset?
static s16 sTriangleBuf[13][8];                          // [[s16; 8]; 13]? vert indices?
static u8 *sMemBlockPoolBase;
static u32 sAllocMemory;      // malloc-ed bytes
static s32 D_801BB018;
static s32 D_801BB01C;
static void *sLoadedTextures[0x10];          // texture pointers
static s32 sTextureDisplayLists[0x10];            // gd_dl indices
static s16 sVtxCvrtTCBuf[2];
static s32 D_801BB0B4;                  // second offset into sTriangleBuf
static s32 sVertexBufCount; // vtx's to load into RPD? Vtx len in GD Dl and in the lower bank (AF30)
static s32 sTriangleBufCount;                  // number of triangles in sTriangleBuf
static s32 sVertexBufStartIndex;                  // Vtx start in GD Dl
static struct GdVec3f sTextDrawPos;  // position to draw text? only set in one function, never used
static Mtx gdRCPIdentityMatrix;
static Mat4f gdIdentityMatrix;
static s8 sVtxCvrtNormBuf[3];
static s16 sAlpha;
static s32 sNumLights;
static struct GdColour sAmbScaleColour;
static struct GdColour sLightScaleColours[2];
static struct LightDirVec sLightDirections[2];
static s32 sLightId;
static Hilite sHilites[600];
static struct GdVec3f D_801BD758;
static struct GdVec3f D_801BD768; // had to migrate earlier
static struct GdObj *sMenuGadgets[9]; // d_obj ptr storage? menu?
static struct ObjView *sDebugViews[2];  // Seems to be a list of ObjViews for displaying debug info
static struct GdDisplayList *sStaticDl;
static struct GdDisplayList *sDynamicMainDls[2];
static struct GdDisplayList *sGdDlStash;
static struct GdDisplayList *sViewDls[3][2];
static struct GdDisplayList *sGdDLArray[MAX_GD_DLS]; // indexed by dl number (gddl+0x44)
static s32 sPickBufLen;
static s32 sPickBufPosition;
static s16 *sPickBuf;
static LookAt D_801BE790[2];
static LookAt D_801BE7D0[3];
static OSMesgQueue D_801BE830; // controller msg queue
static OSMesg D_801BE848[10];
static OSMesgQueue sGdDMAQueue;
static OSMesg sGdMesgBuf[1];
static OSMesg sGdDMACompleteMsg; // msg buf for D_801BE8B0 queue
static OSIoMesg sGdDMAReqMesg;
static struct ObjView *D_801BE994; // store if View flag 0x40 set

// data
static s32 D_801A8674 = 0;
static s32 D_801A867C = 0;
static s32 D_801A8680 = 0;
static f32 sTracked1FrameTime = 0.0f;
static f32 sDynamicsTime = 0.0f;
static f32 sDLGenTime = 0.0f;
static f32 sRCPTime = 0.0f;
static f32 sTimeScaleFactor = 1.0f;
static u32 sMemBlockPoolSize = 1;
static s32 sMemBlockPoolUsed = 0;
static s32 sTextureCount = 0;  // maybe?
static struct GdTimer *D_801A86A4 = NULL; // timer for dlgen, dynamics, or rcp
static struct GdTimer *D_801A86A8 = NULL; // timer for dlgen, dynamics, or rcp
static struct GdTimer *D_801A86AC = NULL; // timer for dlgen, dynamics, or rcp
s32 gGdFrameBufNum = 0;
static struct ObjShape *sHandShape = NULL;
static s32 D_801A86BC = 1;
static s32 sMtxParamType = G_MTX_PROJECTION;
static struct ObjView *sActiveView = NULL;  // current view? used when drawing dl
static struct ObjView *sScreenView = NULL;
static struct ObjView *D_801A86E0 = NULL;
static struct ObjView *sMenuView = NULL;
static u32 sItemsInMenu = 0;
static s32 sDebugViewsCount = 0;               // number of elements in the sDebugViews array
static s32 sCurrDebugViewIndex = 0;             // timing activate cool down counter?
struct GdDisplayList *sCurrentGdDl = NULL;
static u32 sGdDlCount = 0;
static struct DynListBankInfo sDynLists[] = {
    { STD_LIST_BANK, GODDARD_MAIN_FACE_SCENE },
    { TABLE_END, NULL }
};

// textures and display list data
ALIGNED8 static Texture gd_texture_hand_open[] = {
#include "textures/intro_raw/hand_open.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_hand_closed[] = {
#include "textures/intro_raw/hand_closed.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_0[] = {
#include "textures/intro_raw/red_star_0.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_1[] = {
#include "textures/intro_raw/red_star_1.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_2[] = {
#include "textures/intro_raw/red_star_2.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_3[] = {
#include "textures/intro_raw/red_star_3.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_4[] = {
#include "textures/intro_raw/red_star_4.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_5[] = {
#include "textures/intro_raw/red_star_5.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_6[] = {
#include "textures/intro_raw/red_star_6.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_red_star_7[] = {
#include "textures/intro_raw/red_star_7.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_0[] = {
#include "textures/intro_raw/white_star_0.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_1[] = {
#include "textures/intro_raw/white_star_1.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_2[] = {
#include "textures/intro_raw/white_star_2.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_3[] = {
#include "textures/intro_raw/white_star_3.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_4[] = {
#include "textures/intro_raw/white_star_4.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_5[] = {
#include "textures/intro_raw/white_star_5.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_6[] = {
#include "textures/intro_raw/white_star_6.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_white_star_7[] = {
#include "textures/intro_raw/white_star_7.rgba16.inc.c"
};

static Vtx_t gd_vertex_star[] = {
    {{-64,   0, 0}, 0, {  0, 992}, {0x00, 0x00, 0x7F}},
    {{ 64,   0, 0}, 0, {992, 992}, {0x00, 0x00, 0x7F}},
    {{ 64, 128, 0}, 0, {992,   0}, {0x00, 0x00, 0x7F}},
    {{-64, 128, 0}, 0, {  0,   0}, {0x00, 0x00, 0x7F}},
};

static Gfx gd_dl_star_common[] = {
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_NOOP2),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPVertex(gd_vertex_star, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2),
    gsSPEndDisplayList(),
};

static Gfx gd_dl_red_star_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_0),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_1[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_1),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_2[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_2),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_3[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_3),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_4[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_4),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_5),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_6[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_6),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_red_star_7[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_red_star_7),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_0[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_0),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_1[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_1),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_2[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_2),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_3[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_3),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_4[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_4),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_5[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_5),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_6[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_6),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx gd_dl_silver_star_7[] = {
    gsDPPipeSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_white_star_7),
    gsSPBranchList(gd_dl_star_common),
};

static Gfx *gd_red_star_dl_array[] = {
    gd_dl_red_star_0,
    gd_dl_red_star_0,
    gd_dl_red_star_1,
    gd_dl_red_star_1,
    gd_dl_red_star_2,
    gd_dl_red_star_2,
    gd_dl_red_star_3,
    gd_dl_red_star_3,
    gd_dl_red_star_4,
    gd_dl_red_star_4,
    gd_dl_red_star_5,
    gd_dl_red_star_5,
    gd_dl_red_star_6,
    gd_dl_red_star_6,
    gd_dl_red_star_7,
    gd_dl_red_star_7,
};

static Gfx *gd_silver_star_dl_array[] = {
    gd_dl_silver_star_0,
    gd_dl_silver_star_0,
    gd_dl_silver_star_1,
    gd_dl_silver_star_1,
    gd_dl_silver_star_2,
    gd_dl_silver_star_2,
    gd_dl_silver_star_3,
    gd_dl_silver_star_3,
    gd_dl_silver_star_4,
    gd_dl_silver_star_4,
    gd_dl_silver_star_5,
    gd_dl_silver_star_5,
    gd_dl_silver_star_6,
    gd_dl_silver_star_6,
    gd_dl_silver_star_7,
    gd_dl_silver_star_7,
};

ALIGNED8 static Texture gd_texture_sparkle_0[] = {
#include "textures/intro_raw/sparkle_0.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_sparkle_1[] = {
#include "textures/intro_raw/sparkle_1.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_sparkle_2[] = {
#include "textures/intro_raw/sparkle_2.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_sparkle_3[] = {
#include "textures/intro_raw/sparkle_3.rgba16.inc.c"
};

ALIGNED8 static Texture gd_texture_sparkle_4[] = {
#include "textures/intro_raw/sparkle_4.rgba16.inc.c"
};

//! No reference to this texture. Two DL's uses the same previous texture
//  instead of using this texture.
UNUSED ALIGNED8 static Texture gd_texture_sparkle_5[] = {
#include "textures/intro_raw/sparkle_5.rgba16.inc.c"
};

static Vtx_t gd_vertex_sparkle[] = {
    {{   -32,      0,      0}, 0, {      0,   1984}, {  0x00, 0x00, 0x7F, 0x00}},
    {{    32,      0,      0}, 0, {   1984,   1984}, {  0x00, 0x00, 0x7F, 0x00}},
    {{    32,     64,      0}, 0, {   1984,      0}, {  0x00, 0x00, 0x7F, 0x00}},
    {{   -32,     64,      0}, 0, {      0,      0}, {  0x00, 0x00, 0x7F, 0x00}},
};

static Gfx gd_dl_sparkle[] = {
    gsDPSetCombineMode(G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_NOOP2),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPVertex(gd_vertex_sparkle, 4, 0),
    gsSP2Triangles(0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2),
    gsSPEndDisplayList(),
};

static Gfx gd_dl_sparkle_red_color[] = {
    gsDPSetPrimColor(0, 0, 255, 0, 0, 255),
    gsSPEndDisplayList(),
};

static Gfx gd_dl_sparkle_white_color[] = {
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPEndDisplayList(),
};

static Gfx gd_dl_red_sparkle_0[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_red_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_0),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_red_sparkle_1[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_red_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_1),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_red_sparkle_2[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_red_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_2),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_red_sparkle_3[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_red_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_3),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_red_sparkle_4[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_red_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_4),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_red_sparkle_4_dup[] ={
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_red_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_4), // 4 again, correct texture would be 5
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_silver_sparkle_0[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_white_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_0),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_silver_sparkle_1[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_white_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_1),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_silver_sparkle_2[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_white_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_2),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_silver_sparkle_3[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_white_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_3),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_silver_sparkle_4[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_white_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_4),
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx gd_dl_silver_sparkle_4_dup[] = {
    gsDPPipeSync(),
    gsSPDisplayList(gd_dl_sparkle_white_color),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gd_texture_sparkle_4), // 4 again, correct texture would be 5
    gsSPBranchList(gd_dl_sparkle),
};

static Gfx *gd_red_sparkle_dl_array[] = {
    gd_dl_red_sparkle_4,
    gd_dl_red_sparkle_4,
    gd_dl_red_sparkle_3,
    gd_dl_red_sparkle_3,
    gd_dl_red_sparkle_2,
    gd_dl_red_sparkle_2,
    gd_dl_red_sparkle_1,
    gd_dl_red_sparkle_1,
    gd_dl_red_sparkle_0,
    gd_dl_red_sparkle_0,
    gd_dl_red_sparkle_4_dup,
    gd_dl_red_sparkle_4_dup,
};

static Gfx *gd_silver_sparkle_dl_array[] = {
    gd_dl_silver_sparkle_4,
    gd_dl_silver_sparkle_4,
    gd_dl_silver_sparkle_3,
    gd_dl_silver_sparkle_3,
    gd_dl_silver_sparkle_2,
    gd_dl_silver_sparkle_2,
    gd_dl_silver_sparkle_1,
    gd_dl_silver_sparkle_1,
    gd_dl_silver_sparkle_0,
    gd_dl_silver_sparkle_0,
    gd_dl_silver_sparkle_4_dup,
    gd_dl_silver_sparkle_4_dup,
};

UNUSED static Gfx gd_texture3_dummy_aligner1[] = {
    gsSPEndDisplayList(),
};

ALIGNED8 static Texture gd_texture_mario_face_shine[] = {
#include "textures/intro_raw/mario_face_shine.ia8.inc.c"
};

static Gfx gd_dl_mario_face_shine[] = {
    gsSPSetGeometryMode(G_TEXTURE_GEN),
    gsSPTexture(0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCombineMode(G_CC_HILITERGBA, G_CC_HILITERGBA),
    gsDPLoadTextureBlock(gd_texture_mario_face_shine, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

static Gfx gd_dl_rsp_init[] = {
    gsSPClearGeometryMode(0xFFFFFFFF),
    gsSPSetGeometryMode(G_SHADING_SMOOTH | G_SHADE),
    gsSPEndDisplayList(),
};

static Gfx gd_dl_rdp_init[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPNoOp(),
    gsDPSetColorDither(G_CD_MAGICSQ),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

float sGdPerspTimer = 1.0;

static Gfx gd_dl_sprite_start_tex_block[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsDPSetBlendColor(0, 0, 0, 1),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_NOOP2),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsSPEndDisplayList(),
};

static void update_render_mode(void);

// TODO: make a gddl_num_t?

u32 get_alloc_mem_amt(void) {
    return sAllocMemory;
}

/**
 * Returns the current time
 */
s32 gd_get_ostime(void) {
    return osGetTime();
}

f32 get_time_scale(void) {
    return sTimeScaleFactor;
}

void dump_disp_list(void) {
    gd_printf("%d\n", sCurrentGdDl->id);
    gd_printf("Vtx=%d/%d, Mtx=%d/%d, Light=%d/%d, Gfx=%d/%d\n", sCurrentGdDl->curVtxIdx,
              sCurrentGdDl->totalVtx, sCurrentGdDl->curMtxIdx, sCurrentGdDl->totalMtx,
              sCurrentGdDl->curLightIdx, sCurrentGdDl->totalLights, sCurrentGdDl->curGfxIdx,
              sCurrentGdDl->totalGfx);
}

/**
 * Increments the current display list's Gfx index list and returns a pointer to the next Gfx element
 */
static Gfx *next_gfx(void) {
    if (sCurrentGdDl->curGfxIdx >= sCurrentGdDl->totalGfx) {
        dump_disp_list();
        fatal_printf("Gfx list overflow");
    }

    return &sCurrentGdDl->gfx[sCurrentGdDl->curGfxIdx++];
}

/**
 * Increments the current display list's Light index list and returns a pointer to the next Light element
 */
static Lights4 *next_light(void) {
    if (sCurrentGdDl->curLightIdx >= sCurrentGdDl->totalLights) {
        dump_disp_list();
        fatal_printf("Light list overflow");
    }

    return &sCurrentGdDl->light[sCurrentGdDl->curLightIdx++];
}

/**
 * Increments the current display list's matrix index list and returns a pointer to the next matrix element
 */
static Mtx *next_mtx(void) {
    if (sCurrentGdDl->curMtxIdx >= sCurrentGdDl->totalMtx) {
        dump_disp_list();
        fatal_printf("Mtx list overflow");
    }

    return &sCurrentGdDl->mtx[sCurrentGdDl->curMtxIdx++];
}

/**
 * Increments the current display list's vertex index list and returns a pointer to the next vertex element
 */
static Vtx *next_vtx(void) {
    if (sCurrentGdDl->curVtxIdx >= sCurrentGdDl->totalVtx) {
        dump_disp_list();
        fatal_printf("Vtx list overflow");
    }

    return &sCurrentGdDl->vtx[sCurrentGdDl->curVtxIdx++];
}

/**
 * Increments the current display list's viewport list and returns a pointer to the next viewport element
 */
static Vp *next_vp(void) {
    if (sCurrentGdDl->curVpIdx >= sCurrentGdDl->totalVp) {
        dump_disp_list();
        fatal_printf("Vp list overflow");
    }

    return &sCurrentGdDl->vp[sCurrentGdDl->curVpIdx++];
}

/* 249AAC -> 249AEC */
f64 gd_sin_d(f64 x) {
    return sinf(x);
}

/* 249AEC -> 249B2C */
f64 gd_cos_d(f64 x) {
    return cosf(x);
}

/* 249B2C -> 249BA4 */
f64 gd_sqrt_d(f64 x) {
    if (x < 1.0e-7) {
        return 0.0;
    }
    return sqrtf(x);
}

/* 24A19C -> 24A1D4 */
void gd_exit(UNUSED s32 code) {
    gd_printf("exit\n");
    while (TRUE) {
    }
}

void gd_free(void *ptr) {
    sAllocMemory -= gd_free_mem(ptr);
}

/* 24A220 -> 24A318 */
void *gd_allocblock(u32 size) {
    void *block; // 1c

    size = ALIGN8(size);
    if ((sMemBlockPoolUsed + size) > sMemBlockPoolSize) {
        gd_printf("gd_allocblock(): Failed request: %dk (%d bytes)\n", size / 1024, size);
        gd_printf("gd_allocblock(): Heap usage: %dk (%d bytes) \n", sMemBlockPoolUsed / 1024,
                  sMemBlockPoolUsed);
        print_all_memtrackers();
        mem_stats();
        fatal_printf("exit");
    }

    block = sMemBlockPoolBase + sMemBlockPoolUsed;
    sMemBlockPoolUsed += size;
    return block;
}

/* 24A318 -> 24A3E8 */
void *gd_malloc(u32 size, u8 perm) {
    void *ptr; // 1c
    size = ALIGN8(size);
    ptr = gd_request_mem(size, perm);

    if (ptr == NULL) {
        gd_printf("gd_malloc(): Failed request: %dk (%d bytes)\n", size / 1024, size);
        gd_printf("gd_malloc(): Heap usage: %dk (%d bytes) \n", sAllocMemory / 1024, sAllocMemory);
        print_all_memtrackers();
        mem_stats();
        return NULL;
    }

    sAllocMemory += size;

    return ptr;
}

void *gd_malloc_perm(u32 size) {
    return gd_malloc(size, PERM_G_MEM_BLOCK);
}

void *gd_malloc_temp(u32 size) {
    return gd_malloc(size, TEMP_G_MEM_BLOCK);
}

/* 24A4DC -> 24A598 */
void draw_indexed_dl(s32 dlNum, s32 gfxIdx) {
    Gfx *dl;

    if (gfxIdx != 0) {
        dl = sGdDLArray[dlNum]->dlptr[gfxIdx - 1];  // multiple display lists (determined by frame)
    } else {
        dl = sGdDLArray[dlNum]->gfx;  // only one display list
    }
    gSPDisplayList(next_gfx(), GD_VIRTUAL_TO_PHYSICAL(dl));
}

void branch_cur_dl_to_num(s32 dlNum) {
    Gfx *dl;

    dl = sGdDLArray[dlNum]->gfx;
    gSPDisplayList(next_gfx(), GD_VIRTUAL_TO_PHYSICAL(dl));
}

/**
 * Creates `ObjShape`s for the stars and sparkles
 */
void setup_stars(void) {
    gShapeRedStar = make_shape(0, "redstar");
    gShapeRedStar->dlNums[0] = new_gddl_from(NULL, 0);
    gShapeRedStar->dlNums[1] = gShapeRedStar->dlNums[0];
    sGdDLArray[gShapeRedStar->dlNums[0]]->dlptr = gd_red_star_dl_array;
    sGdDLArray[gShapeRedStar->dlNums[1]]->dlptr = gd_red_star_dl_array;

    gShapeSilverStar = make_shape(0, "silverstar");
    gShapeSilverStar->dlNums[0] = new_gddl_from(NULL, 0);
    gShapeSilverStar->dlNums[1] = gShapeSilverStar->dlNums[0];
    sGdDLArray[gShapeSilverStar->dlNums[0]]->dlptr = gd_silver_star_dl_array;
    sGdDLArray[gShapeSilverStar->dlNums[1]]->dlptr = gd_silver_star_dl_array;

    // make_shape names of the dl array they call are misnamed (swapped)
    // "sspark" calls red sparkles and "rspark" calls silver sparkles
    gShapeRedSpark = make_shape(0, "sspark");
    gShapeRedSpark->dlNums[0] = new_gddl_from(NULL, 0);
    gShapeRedSpark->dlNums[1] = gShapeRedSpark->dlNums[0];
    sGdDLArray[gShapeRedSpark->dlNums[0]]->dlptr = gd_red_sparkle_dl_array;
    sGdDLArray[gShapeRedSpark->dlNums[1]]->dlptr = gd_red_sparkle_dl_array;

    gShapeSilverSpark = make_shape(0, "rspark");
    gShapeSilverSpark->dlNums[0] = new_gddl_from(NULL, 0);
    gShapeSilverSpark->dlNums[1] = gShapeSilverSpark->dlNums[0];
    sGdDLArray[gShapeSilverSpark->dlNums[0]]->dlptr = gd_silver_sparkle_dl_array;
    sGdDLArray[gShapeSilverSpark->dlNums[1]]->dlptr = gd_silver_sparkle_dl_array;
}

/* 24AA58 -> 24AAA8 */
void Unknown8019C288(s32 stickX, s32 stickY) {
    struct GdControl *ctrl = &gdControllerInfo; // 4

    ctrl->stickXf = (f32) stickX;
    ctrl->stickYf = (f32)(stickY / 2);
}

void gdAddMemoryToHeap(void *addr, u32 size) {
    // TODO: is this `1` for permanence special?
    gd_add_mem_to_heap(size, addr, 1);
}

/* 24AAE0 -> 24AB7C */
void gdInitMemory(void *blockpool, u32 size) {
    imin("gdInitMemory");
    // Align downwards?
    size = (size - 8) & ~7;
    // Align to next double word boundry?
    blockpool = (void *) (((uintptr_t) blockpool + 8) & ~7);
    sMemBlockPoolBase = blockpool;
    sMemBlockPoolSize = size;
    sMemBlockPoolUsed = 0;
    sAllocMemory = 0;
    init_mem_block_lists();
    gd_reset_sfx();
    imout();
}

/**
 * Initializes the Mario head demo
 */
void gdSetupFace(void) {

    imin("gdSetupFace");
    sMarioSceneGrp = NULL;
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreateMesgQueue(&sGdDMAQueue, sGdMesgBuf, ARRAY_COUNT(sGdMesgBuf));
    gdInitSystem();
    gdResetDynListAndShapeProcessors();
    reset_cur_dl_indices();
    setup_stars();
    imout();
}

void print_gdm_stats(void) {
    stop_memtracker("total");
    gd_printf("\ngdm stats:\n");
    print_all_memtrackers();
    mem_stats();
    start_memtracker("total");
}

struct ObjView *make_view_withgrp(char *name, struct ObjGroup *grp) {
    struct ObjView *view = make_view(name, (VIEW_DRAW | VIEW_ALLOC_ZBUF | VIEW_MOVEMENT), 1, 0, 0, 320, 240, grp);

    if (view == NULL) {
        fatal_printf("make_view_withgrp(): make_view failed!\n");
    }

    UNUSED struct ObjGroup *viewgrp = make_group(2, grp, view);

    if (viewgrp == NULL) {
        fatal_printf("make_view_withgrp(): make_viewgrp failed!\n");
    }

    view->lights = gGdLightGroup;
    return view;
}

/* 24AD14 -> 24AEB8 */
void gdLoadScene(s32 id) {
    imin("gdLoadScene");
    switch (id) {
        case GD_SCENE_REGULAR_MARIO: // normal Mario head
            if (sMarioSceneGrp == NULL) {
                load_mario_head(animate_mario_head_normal);
                sMarioSceneGrp = gMarioFaceGrp; // gMarioFaceGrp set by load_mario_head
                gdInitMouse(NULL);
            }
            sMSceneView = make_view_withgrp("mscene", sMarioSceneGrp);
            break;
        case GD_SCENE_DIZZY_MARIO: // game over Mario head
            if (sMarioSceneGrp == NULL) {
                load_mario_head(animate_mario_head_gameover);
                sMarioSceneGrp = gMarioFaceGrp;
                gdInitMouse(NULL);
            }
            sMSceneView = make_view_withgrp("mscene", sMarioSceneGrp);
            break;
        default:
            fatal_printf("gdLoadScene(%d): unknown dl", id);
    }
    imout();
}

/* 24AEB8 -> 24AED0 */
void set_time_scale(f32 factor) {
    sTimeScaleFactor = factor;
}

/* 24AED0 -> 24AF04 */
void Unknown8019C840(void) {
    gd_printf("\n");
    print_all_timers();
}

void gdFinishDrawing(void) {
    gSPEndDisplayList(next_gfx());
}

/* 24B418 -> 24B4CC; not called */
void gdm_getpos(s32 id, struct GdVec3f *dst) {
    struct GdObj *dobj; // 1c
    switch (id) {
        case 5:
            set_gd_mtx_parameters(G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);
            dobj = dUseObject("testnet2");
            dst->x = ((struct ObjNet *) dobj)->worldPos.x;
            dst->y = ((struct ObjNet *) dobj)->worldPos.y;
            dst->z = ((struct ObjNet *) dobj)->worldPos.z;
            break;
        default:
            fatal_printf("gdm_getpos(): %d out of range", id);
    }
    return;
}

/**
 * Clamps the coordinates so that they are within the active view
 */
static void clamp_coords_to_active_view(f32 *x, f32 *y) {
    struct ObjView *view = sActiveView;

    if (*x < 0.0f) {
        *x = 0.0f;
    } else if (*x > view->lowerRight.x) {
        *x = view->lowerRight.x;
    }

    if (*y < 0.0f) {
        *y = 0.0f;
    } else if (*y > view->lowerRight.y) {
        *y = view->lowerRight.y;
    }
}

void fatal_no_dl_mem(void) {
    fatal_printf("Out of DL mem\n");
}

/* 24B5D4 -> 24B6AC */
struct GdDisplayList *alloc_displaylist(u32 id) {
    struct GdDisplayList *gdDl;

    gdDl = gd_malloc_perm(sizeof(struct GdDisplayList));
    if (gdDl == NULL) {
        fatal_no_dl_mem();
    }

    gdDl->number = sGdDlCount++;
    if (sGdDlCount >= MAX_GD_DLS) {
        fatal_printf("alloc_displaylist() too many display lists %d (MAX %d)", sGdDlCount + 1,
                     MAX_GD_DLS);
    }
    sGdDLArray[gdDl->number] = gdDl;
    gdDl->id = id;
    return gdDl;
}

void cpy_remaining_gddl(struct GdDisplayList *dst, struct GdDisplayList *src) {
    dst->vtx = &DL_CURRENT_VTX(src);
    dst->mtx = &DL_CURRENT_MTX(src);
    dst->light = &DL_CURRENT_LIGHT(src);
    dst->gfx = &DL_CURRENT_GFX(src);
    dst->vp = &DL_CURRENT_VP(src);
    dst->totalVtx = src->totalVtx - src->curVtxIdx;
    dst->totalMtx = src->totalMtx - src->curMtxIdx;
    dst->totalLights = src->totalLights - src->curLightIdx;
    dst->totalGfx = src->totalGfx - src->curGfxIdx;
    dst->totalVp = src->totalVp - src->curVpIdx;
    dst->curVtxIdx = 0;
    dst->curMtxIdx = 0;
    dst->curLightIdx = 0;
    dst->curGfxIdx = 0;
    dst->curVpIdx = 0;
}

struct GdDisplayList *create_child_gdl(s32 id, struct GdDisplayList *srcDl) {
    struct GdDisplayList *newDl;

    newDl = alloc_displaylist(id);
    newDl->parent = srcDl;
    cpy_remaining_gddl(newDl, srcDl);

    return newDl;
}

struct GdDisplayList *new_gd_dl(s32 id, s32 gfxs, s32 verts, s32 mtxs, s32 lights, s32 vps) {
    struct GdDisplayList *dl; // 24

    dl = alloc_displaylist(id);
    dl->parent = NULL;
    if (verts == 0) {
        verts = 1;
    }
    dl->curVtxIdx = 0;
    dl->totalVtx = verts;
    if ((dl->vtx = gd_malloc_perm(verts * sizeof(Vtx))) == NULL) {
        fatal_no_dl_mem();
    }

    if (mtxs == 0) {
        mtxs = 1;
    }
    dl->curMtxIdx = 0;
    dl->totalMtx = mtxs;
    if ((dl->mtx = gd_malloc_perm(mtxs * sizeof(Mtx))) == NULL) {
        fatal_no_dl_mem();
    }

    if (lights == 0) {
        lights = 1;
    }
    dl->curLightIdx = 0;
    dl->totalLights = lights;
    if ((dl->light = gd_malloc_perm(lights * sizeof(Lights4))) == NULL) {
        fatal_no_dl_mem();
    }

    if (gfxs == 0) {
        gfxs = 1;
    }
    dl->curGfxIdx = 0;
    dl->totalGfx = gfxs;
    if ((dl->gfx = gd_malloc_perm(gfxs * sizeof(Gfx))) == NULL) {
        fatal_no_dl_mem();
    }

    if (vps == 0) {
        vps = 1;
    }
    dl->curVpIdx = 0;
    dl->totalVp = vps;
    if ((dl->vp = gd_malloc_perm(vps * sizeof(Vp))) == NULL) {
        fatal_no_dl_mem();
    }

    dl->dlptr = NULL;
    return dl;
}

/* 24BA48 -> 24BABC; not called */
void gd_rsp_init(void) {
    gSPDisplayList(next_gfx(), osVirtualToPhysical(&gd_dl_rsp_init));
    gDPPipeSync(next_gfx());
}

/* 24BABC -> 24BB30; not called */
void gd_rdp_init(void) {
    gSPDisplayList(next_gfx(), osVirtualToPhysical(&gd_dl_rdp_init));
    gDPPipeSync(next_gfx());
}

void gd_draw_rect(f32 ulx, f32 uly, f32 lrx, f32 lry) {
    clamp_coords_to_active_view(&ulx, &uly);
    clamp_coords_to_active_view(&lrx, &lry);

    if (lrx > ulx && lry > uly) {
        gDPFillRectangle(next_gfx(), (u32)(sActiveView->upperLeft.x + ulx),
                         (u32)(uly + sActiveView->upperLeft.y), (u32)(sActiveView->upperLeft.x + lrx),
                         (u32)(lry + sActiveView->upperLeft.y));
    }

    gDPPipeSync(next_gfx());
    gDPSetCycleType(next_gfx(), G_CYC_1CYCLE);
    gDPSetRenderMode(next_gfx(), G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2);
}

void gd_draw_border_rect(f32 ulx, f32 uly, f32 lrx, f32 lry) {
    clamp_coords_to_active_view(&ulx, &uly);
    clamp_coords_to_active_view(&lrx, &lry);

    if (lrx > ulx && lry > uly) {
        gDPFillRectangle(
            next_gfx(), (u32)(sActiveView->upperLeft.x + ulx), (u32)(uly + sActiveView->upperLeft.y),
            (u32)(sActiveView->upperLeft.x + ulx + 5.0f), (u32)(lry + sActiveView->upperLeft.y));
        gDPFillRectangle(next_gfx(), (u32)(sActiveView->upperLeft.x + lrx - 5.0f),
                         (u32)(uly + sActiveView->upperLeft.y), (u32)(sActiveView->upperLeft.x + lrx),
                         (u32)(lry + sActiveView->upperLeft.y));
        gDPFillRectangle(next_gfx(), (u32)(sActiveView->upperLeft.x + ulx),
                         (u32)(uly + sActiveView->upperLeft.y), (u32)(sActiveView->upperLeft.x + lrx),
                         (u32)(uly + sActiveView->upperLeft.y + 5.0f));
        gDPFillRectangle(next_gfx(), (u32)(sActiveView->upperLeft.x + ulx),
                         (u32)(lry + sActiveView->upperLeft.y - 5.0f),
                         (u32)(sActiveView->upperLeft.x + lrx), (u32)(lry + sActiveView->upperLeft.y));
    }

    gDPPipeSync(next_gfx());
    gDPSetCycleType(next_gfx(), G_CYC_1CYCLE);
    gDPSetRenderMode(next_gfx(), G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2);
}

void gdDisplayListSetFillColor(struct GdColour *colour) {
    u8 r, g, b;

    r = colour->r * 255.0f;
    g = colour->g * 255.0f;
    b = colour->b * 255.0f;

    gDPPipeSync(next_gfx());
    gDPSetCycleType(next_gfx(), G_CYC_FILL);
    gDPSetRenderMode(next_gfx(), G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetFillColor(next_gfx(), GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1));
}

void gdSetZBuffer(void) {
    gDPSetDepthImage(next_gfx(), GD_LOWER_24(sActiveView->parent->zbuf));
}

void gdSetFrameBuffer(void) {
    gDPSetColorImage(next_gfx(), G_IM_FMT_RGBA, G_IM_SIZ_16b, sActiveView->parent->lowerRight.x,
                     GD_LOWER_24(sActiveView->parent->colourBufs[gGdFrameBufNum]));
}

void reset_cur_dl_indices(void) {
    sMHeadMainDls[gGdFrameBufNum]->curGfxIdx = 0;
    sCurrentGdDl = sDynamicMainDls[gGdFrameBufNum];
    sCurrentGdDl->curVtxIdx = 0;
    sCurrentGdDl->curMtxIdx = 0;
    sCurrentGdDl->curLightIdx = 0;
    sCurrentGdDl->curGfxIdx = 0;
    sCurrentGdDl->curVpIdx = 0;
}

void begin_gddl(s32 num) {
    sCurrentGdDl = sGdDLArray[num];
    sCurrentGdDl->curVtxIdx = 0;
    sCurrentGdDl->curMtxIdx = 0;
    sCurrentGdDl->curLightIdx = 0;
    sCurrentGdDl->curGfxIdx = 0;
    sCurrentGdDl->curVpIdx = 0;
}

void gdDisplayListStash(void) {
    sGdDlStash = sCurrentGdDl;
}

void gdDisplayListStashPop(void) {
    sCurrentGdDl = sGdDlStash;
}

/* 24D084 -> 24D1D4 */
s32 gd_startdisplist(s32 memarea) {
    D_801BB018 = 0;
    D_801BB01C = 1;

    switch (memarea) {
        case 7:  // Create new display list as a child of sStaticDl
            sCurrentGdDl = create_child_gdl(0, sStaticDl);
            break;
        case 8:  // Use the active view's display list
            if (sActiveView->id > 2) {
                fatal_printf("gd_startdisplist(): Too many views to display");
            }

            sCurrentGdDl = sViewDls[sActiveView->id][gGdFrameBufNum];
            cpy_remaining_gddl(sCurrentGdDl, sCurrentGdDl->parent);
            break;
        default:
            fatal_printf("gd_startdisplist(): Unknown memory area");
            break;
    }
    gDPPipeSync(next_gfx());

    return sCurrentGdDl->number;
}

/* 24D1D4 -> 24D23C */
void gd_enddlsplist(void) {
    gDPPipeSync(next_gfx());
    gSPEndDisplayList(next_gfx());
}

s32 gd_enddlsplist_parent(void) {
    s32 curDlIdx = 0; // 24

    gDPPipeSync(next_gfx());
    gSPEndDisplayList(next_gfx());
    if (sCurrentGdDl->parent != NULL) {
        sCurrentGdDl->parent->curVtxIdx = (sCurrentGdDl->parent->curVtxIdx + sCurrentGdDl->curVtxIdx);
        sCurrentGdDl->parent->curMtxIdx = (sCurrentGdDl->parent->curMtxIdx + sCurrentGdDl->curMtxIdx);
        sCurrentGdDl->parent->curLightIdx =
            (sCurrentGdDl->parent->curLightIdx + sCurrentGdDl->curLightIdx);
        sCurrentGdDl->parent->curGfxIdx = (sCurrentGdDl->parent->curGfxIdx + sCurrentGdDl->curGfxIdx);
        sCurrentGdDl->parent->curVpIdx = (sCurrentGdDl->parent->curVpIdx + sCurrentGdDl->curVpIdx);
    }
    curDlIdx = sCurrentGdDl->curGfxIdx;
    return curDlIdx;
}

/* 24D39C -> 24D3D8 */
void Unknown8019EBCC(s32 num, uintptr_t gfxptr) {
    sGdDLArray[num]->gfx = (Gfx *) (GD_LOWER_24(gfxptr) + D_801BAF28);
}

u32 new_gddl_from(Gfx *dl, UNUSED s32 arg1) {
    struct GdDisplayList *gddl;

    gddl = new_gd_dl(0, 0, 0, 0, 0, 0);
    gddl->gfx = (Gfx *) (GD_LOWER_24((uintptr_t) dl) + D_801BAF28);
    return gddl->number;
}

/* 24D458 -> 24D4C4 */
u32 Unknown8019EC88(Gfx *dl, UNUSED s32 arg1) {
    struct GdDisplayList *gddl;

    gddl = new_gd_dl(0, 0, 0, 0, 0, 0);
    gddl->gfx = dl;
    return gddl->number;
}

void mat4_to_mtx(Mat4f *src, Mtx *dst) {
#ifndef GBI_FLOATS
    s32 i; // 14
    s32 j; // 10
    s32 w1;
    s32 w2;
    s32 *mtxInt = (s32 *) dst->m[0]; // s32 part
    s32 *mtxFrc = (s32 *) dst->m[2]; // frac part

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            w1 = (s32)((*src)[i][j * 2] * 65536.0f);
            w2 = (s32)((*src)[i][j * 2 + 1] * 65536.0f);
            *mtxInt = MTX_INTPART_PACK(w1, w2);
            mtxInt++;
            *mtxFrc = MTX_FRACPART_PACK(w1, w2);
            mtxFrc++;
        }
    }
#else
    guMtxF2L(*src, dst);
#endif
}

/**
 * Adds a display list operation that multiplies the current matrix with `mtx`.
 */
void gd_dl_mul_matrix(Mat4f *mtx) {
    mat4_to_mtx(mtx, &DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), osVirtualToPhysical(&DL_CURRENT_MTX(sCurrentGdDl)), sMtxParamType | G_MTX_MUL | G_MTX_NOPUSH);
    next_mtx();
}

/**
 * Adds a display list operation that replaces the current matrix with `mtx`.
 */
void gd_dl_load_matrix(Mat4f *mtx) {
    mat4_to_mtx(mtx, &DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), osVirtualToPhysical(&DL_CURRENT_MTX(sCurrentGdDl)),
              sMtxParamType | G_MTX_LOAD | G_MTX_NOPUSH);
    next_mtx();
}

/**
 * Adds a display list operation that replaces the current matrix with the
 * identity matrix.
 */
void gd_dl_load_identity_matrix(void) {
    gSPMatrix(next_gfx(), osVirtualToPhysical(&gdRCPIdentityMatrix), sMtxParamType | G_MTX_LOAD | G_MTX_NOPUSH);
}

/**
 * Adds a display list operation that pushes the current matrix onto the matrix
 * stack.
 */
void gdDisplayListPushMatrix(void) {
    gSPMatrix(next_gfx(), osVirtualToPhysical(&gdRCPIdentityMatrix), sMtxParamType | G_MTX_MUL | G_MTX_PUSH);
}

/**
 * Adds a display list operation that pops a matrix from the matrix stack.
 */
void gdDisplayListPopMatrix(void) {
    gSPPopMatrix(next_gfx(), sMtxParamType);
}

/**
 * Adds a display list operation that translates the current matrix by `x`, `y`, and `z`.
 */
void gdDisplayListDisplacement(f32 x, f32 y, f32 z) {
    guTranslate(&DL_CURRENT_MTX(sCurrentGdDl), x, y, z);
    gSPMatrix(next_gfx(), osVirtualToPhysical(&DL_CURRENT_MTX(sCurrentGdDl)), sMtxParamType | G_MTX_MUL | G_MTX_NOPUSH);
    next_mtx();
}

/**
 * Adds a display list operation that loads a translation matrix.
 */
void gdDisplayListTranslate(f32 x, f32 y, f32 z) {
    guTranslate(&DL_CURRENT_MTX(sCurrentGdDl), x, y, z);
    gSPMatrix(next_gfx(), osVirtualToPhysical(&DL_CURRENT_MTX(sCurrentGdDl)),
              sMtxParamType | G_MTX_LOAD | G_MTX_NOPUSH);
    next_mtx();
}

/**
 * Adds a display list operation that scales the current matrix by `x`, `y`, and `z`.
 */
void gd_dl_scale(f32 x, f32 y, f32 z) {
    Mat4f mtx;
    struct GdVec3f vec;

    vec.x = x;
    vec.y = y;
    vec.z = z;
    gd_set_identity_mat4(&mtx);
    gd_scale_mat4f_by_vec3f(&mtx, &vec);
    gd_dl_mul_matrix(&mtx);
}

/* 24DA94 -> 24DAE8 */
void func_8019F2C4(f32 arg0, s8 arg1) {
    Mat4f mtx; // 18

    gd_set_identity_mat4(&mtx);
    gd_absrot_mat4(&mtx, arg1 - 120, -arg0);
    gd_dl_mul_matrix(&mtx);
}

/* 24DAE8 -> 24E1A8 */
void gd_dl_lookat(struct ObjCamera *cam, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    LookAt *lookat;

    arg7 *= RAD_PER_DEG;

    gd_mat4f_lookat(&cam->unkE8, arg1, arg2, arg3, arg4, arg5, arg6, gd_sin_d(arg7), gd_cos_d(arg7),
                  0.0f);

    mat4_to_mtx(&cam->unkE8, &DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), osVirtualToPhysical(&DL_CURRENT_MTX(sCurrentGdDl)),
            G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

    /*  col           colc          dir
        0  1  2   3   4  5  6   7   8  9  10  11
    { { 0, 0, 0}, _, {0, 0, 0}, _, {0, 0, 0}, _}
       16 17 18  19  20 21  22 23  24 25  26  27
    { { 0, 0, 0}, _, {0, 0, 0}, _, {0, 0, 0}, _}
    */
    lookat = &D_801BE7D0[gGdFrameBufNum];

    lookat->l[0].l.dir[0] = LOOKAT_PACK(cam->unkE8[0][0]);
    lookat->l[0].l.dir[1] = LOOKAT_PACK(cam->unkE8[1][0]);
    lookat->l[0].l.dir[2] = LOOKAT_PACK(cam->unkE8[2][0]);

    lookat->l[1].l.dir[0] = LOOKAT_PACK(cam->unkE8[0][1]);
    lookat->l[1].l.dir[1] = LOOKAT_PACK(cam->unkE8[1][1]);
    lookat->l[1].l.dir[2] = LOOKAT_PACK(cam->unkE8[2][1]);

    lookat->l[0].l.col[0] = 0;
    lookat->l[0].l.col[1] = 0;
    lookat->l[0].l.col[2] = 0;
    lookat->l[0].l.pad1 = 0;
    lookat->l[0].l.colc[0] = 0;
    lookat->l[0].l.colc[1] = 0;
    lookat->l[0].l.colc[2] = 0;
    lookat->l[0].l.pad2 = 0;
    lookat->l[1].l.col[0] = 0;
    lookat->l[1].l.col[1] = 0x80;
    lookat->l[1].l.col[2] = 0;
    lookat->l[1].l.pad1 = 0;
    lookat->l[1].l.colc[0] = 0;
    lookat->l[1].l.colc[1] = 0x80;
    lookat->l[1].l.colc[2] = 0;
    lookat->l[1].l.pad2 = 0;

    lookat = &D_801BE790[0];
    lookat->l[0].l.dir[0] = 1;
    lookat->l[0].l.dir[1] = 0;
    lookat->l[0].l.dir[2] = 0;

    lookat->l[1].l.dir[0] = 0;
    lookat->l[1].l.dir[1] = 1;
    lookat->l[1].l.dir[2] = 0;

    lookat->l[0].l.col[0] = 0;
    lookat->l[0].l.col[1] = 0;
    lookat->l[0].l.col[2] = 0;
    lookat->l[0].l.pad1 = 0;
    lookat->l[0].l.colc[0] = 0;
    lookat->l[0].l.colc[1] = 0;
    lookat->l[0].l.colc[2] = 0;
    lookat->l[0].l.pad2 = 0;
    lookat->l[1].l.col[0] = 0;
    lookat->l[1].l.col[1] = 0x80;
    lookat->l[1].l.col[2] = 0;
    lookat->l[1].l.pad1 = 0;
    lookat->l[1].l.colc[0] = 0;
    lookat->l[1].l.colc[1] = 0x80;
    lookat->l[1].l.colc[2] = 0;
    lookat->l[1].l.pad2 = 0;

    gSPLookAt(next_gfx(), osVirtualToPhysical(&D_801BE7D0[gGdFrameBufNum]));
    next_mtx();
}

void check_tri_display(s32 vtxcount) {
    D_801BB0B4 = 0;
    if (vtxcount != 3) {
        fatal_printf("cant display no tris\n");
    }
}

/**
 * Adds a vertex to the current display list. Returns a pointer to the vertex if
 * it is new, or NULL if the vertex already exists.
 */
Vtx *gdMakeVertex(f32 x, f32 y, f32 z, f32 alpha) {
    Vtx *vtx = NULL;
    s32 i;

    // Add the vertex index to the buffer if it doesn't already exist
    for (i = sVertexBufStartIndex; i < (sVertexBufStartIndex + sVertexBufCount); i++) {
        // the ifs need to be separate to match...
        if (sCurrentGdDl->vtx[i].n.ob[0] == (s16) x) {
            if (sCurrentGdDl->vtx[i].n.ob[1] == (s16) y) {
                if (sCurrentGdDl->vtx[i].n.ob[2] == (s16) z) {
                    sTriangleBuf[sTriangleBufCount][D_801BB0B4++] = (s16) i;
                    return NULL;
                }
            }
        }
    }

    sVertexBufCount++;
    sTriangleBuf[sTriangleBufCount][D_801BB0B4++] = (s16) sCurrentGdDl->curVtxIdx;

    DL_CURRENT_VTX(sCurrentGdDl).n.ob[0] = (s16) x;
    DL_CURRENT_VTX(sCurrentGdDl).n.ob[1] = (s16) y;
    DL_CURRENT_VTX(sCurrentGdDl).n.ob[2] = (s16) z;
    DL_CURRENT_VTX(sCurrentGdDl).n.flag = 0;
    DL_CURRENT_VTX(sCurrentGdDl).n.tc[0] = sVtxCvrtTCBuf[0];
    DL_CURRENT_VTX(sCurrentGdDl).n.tc[1] = sVtxCvrtTCBuf[1];
    DL_CURRENT_VTX(sCurrentGdDl).n.n[0] = sVtxCvrtNormBuf[0];
    DL_CURRENT_VTX(sCurrentGdDl).n.n[1] = sVtxCvrtNormBuf[1];
    DL_CURRENT_VTX(sCurrentGdDl).n.n[2] = sVtxCvrtNormBuf[2];
    DL_CURRENT_VTX(sCurrentGdDl).n.a = (u8)(alpha * 255.0f);

    vtx = &DL_CURRENT_VTX(sCurrentGdDl);
    next_vtx();
    return vtx;
}

void gdTriangleRegistered(void) {
    sTriangleBufCount++;
    if (sVertexBufCount >= 12) {
        // We have ~4 triangles to draw
        gdDrawVtxTriBuffers();
        gdFlushVtxBuffer();
    }
    D_801BB018 = 0;
}

/**
 * Adds a triange to the current display list.
 */
void gdMakeTriangle(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3) {
    Vtx *vtx;

    vtx = &DL_CURRENT_VTX(sCurrentGdDl);
    gdMakeVertex(x1, y1, z1, 1.0f);
    gdMakeVertex(x2, y2, z2, 1.0f);
    gdMakeVertex(x3, y3, z3, 1.0f);

    gSPVertex(next_gfx(), osVirtualToPhysical(vtx), 3, 0);
    gSP1Triangle(next_gfx(), 0, 1, 2, 0);
}

/* 24E808 -> 24E840 */
void gdFlushVtxBuffer(void) {
    sVertexBufCount = 0;
    sTriangleBufCount = 0;
    sVertexBufStartIndex = sCurrentGdDl->curVtxIdx;
}


void gdDrawVtxTriBuffers(void) {
    s32 start_vtx = sVertexBufStartIndex;
    u32 vtxbuf_count = sVertexBufCount;

    if (sVertexBufCount != 0) {
        // load vertex data
        gSPVertex(next_gfx(), osVirtualToPhysical(&sCurrentGdDl->vtx[start_vtx]), vtxbuf_count, 0);
        // load triangle data

        for (s32 i = 0; i < sTriangleBufCount - 1; i += 2) {
            gSP2Triangles(next_gfx(),
                sTriangleBuf[i][0] - start_vtx,
                sTriangleBuf[i][1] - start_vtx,
                sTriangleBuf[i][2] - start_vtx,
                0,
                sTriangleBuf[i + 1][0] - start_vtx,
                sTriangleBuf[i + 1][1] - start_vtx,
                sTriangleBuf[i + 1][2] - start_vtx,
                0
            );
        }
        if ((sTriangleBufCount % 2) != 0) {
            // we're missing a triangle
            gSP1Triangle(next_gfx(),
                sTriangleBuf[sTriangleBufCount - 1][0] - start_vtx,
                sTriangleBuf[sTriangleBufCount - 1][1] - start_vtx,
                sTriangleBuf[sTriangleBufCount - 1][2] - start_vtx,
                0
            );
        }
    }
    gdFlushVtxBuffer();
}

/**
 * Unused - called by func_801A520C
 */
UNUSED static void func_801A01EC(void) {

}

/**
 * Unused - called by func_801A520C
 */
UNUSED static void func_801A025C(void) {
    gGdFrameBufNum ^= 1;
    osViSwapBuffer(sScreenView->parent->colourBufs[gGdFrameBufNum]);
}

/* 24EA88 -> 24EAF4 */
void set_render_alpha(f32 alpha) {
    sAlpha = alpha * 255.0f;
    update_render_mode();
}

/* 24EAF4 -> 24EB0C */
// light id?
void set_light_id(s32 index) {
    sLightId = index;
}

void set_light_num(s32 n) {
    sNumLights = n;
}

/* 24EB24 -> 24EC18 */
s32 create_mtl_gddl(UNUSED s32 mtlType) {
    s32 dlnum;            // 24
    struct GdColour blue; // 18

    blue.r = 0.0f;
    blue.g = 0.0f;
    blue.b = 1.0f;
    dlnum = gd_startdisplist(7);
    gd_dl_material_lighting(dlnum, &blue, GD_MTL_TEX_OFF);
    gd_enddlsplist_parent();
    sCurrentGdDl->totalVtx = sCurrentGdDl->curVtxIdx;
    sCurrentGdDl->totalMtx = sCurrentGdDl->curMtxIdx;
    sCurrentGdDl->totalLights = sCurrentGdDl->curLightIdx;
    sCurrentGdDl->totalGfx = sCurrentGdDl->curGfxIdx;
    sCurrentGdDl->totalVp = sCurrentGdDl->curVpIdx;
    return dlnum;
}

void branch_to_gddl(s32 dlNum) {
    branch_cur_dl_to_num(dlNum);
}

/* 24EC48 -> 24F03C */
// phong shading function?
void gd_dl_hilite(s32 idx, // material GdDl number; offsets into hilite array
                   struct ObjCamera *cam, UNUSED struct GdVec3f *arg2, UNUSED struct GdVec3f *arg3,
                   struct GdVec3f *arg4,   // vector to light source?
                   struct GdColour *colour // light color
) {
    Hilite *hilite; // 4c
    struct GdVec3f sp40;
    f32 sp3C; // magnitude of sp40
    f32 sp38;
    f32 sp34;

    sp38 = 32.0f; // x scale factor?
    sp34 = 32.0f; // y scale factor?
    if (idx >= 0xc8) {
        fatal_printf("too many hilites");
    }
    hilite = &sHilites[idx];

    gDPSetPrimColor(next_gfx(), 0, 0, (s32)(colour->r * 255.0f), (s32)(colour->g * 255.0f),
                    (s32)(colour->b * 255.0f), 255);
    sp40.z = cam->unkE8[0][2] + arg4->x;
    sp40.y = cam->unkE8[1][2] + arg4->y;
    sp40.x = cam->unkE8[2][2] + arg4->z;
    sp3C = sqrtf(SQ(sp40.z) + SQ(sp40.y) + SQ(sp40.x));
    if (sp3C > 0.1) {
        sp3C = 1.0 / sp3C; //? 1.0f
        sp40.z *= sp3C;
        sp40.y *= sp3C;
        sp40.x *= sp3C;

        hilite->h.x1 =
            (((sp40.z * cam->unkE8[0][0]) + (sp40.y * cam->unkE8[1][0]) + (sp40.x * cam->unkE8[2][0]))
             * sp38 * 2.0f)
            + (sp38 * 4.0f);
        hilite->h.y1 =
            (((sp40.z * cam->unkE8[0][1]) + (sp40.y * cam->unkE8[1][1]) + (sp40.x * cam->unkE8[2][1]))
             * sp34 * 2.0f)
            + (sp34 * 4.0f);
    } else {
        hilite->h.x1 = sp38 * 2.0f;
        hilite->h.y1 = sp34 * 2.0f;
    }
}

/**
 * Adds some display list commands that perform lighting for a material
 */
s32 gd_dl_material_lighting(s32 id, struct GdColour *colour, s32 material) {
    s32 i;
    s32 numLights = sNumLights;
    s32 scaledColours[3];
    s32 lightDir[3];

    if (id > 0) {
        begin_gddl(id);
    }
    switch (material) {
        case GD_MTL_TEX_OFF:
            gddl_is_loading_shine_dl(FALSE);
            gddl_is_loading_shine_dl(FALSE);
            gddl_is_loading_shine_dl(FALSE);
            gddl_is_loading_shine_dl(FALSE);
            numLights = NUMLIGHTS_2;
            break;
        case GD_MTL_STUB_DL:
            break;
        case GD_MTL_SHINE_DL:
            gddl_is_loading_shine_dl(TRUE);
            if (id >= 200) {
                fatal_printf("too many hilites");
            }
            gDPSetHilite1Tile(next_gfx(), G_TX_RENDERTILE, &sHilites[id], 32, 32);
            break;
        case GD_MTL_BREAK:
            break;
        default:
            gddl_is_loading_shine_dl(FALSE);

            DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[0] = colour->r * 255.0f;
            DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[1] = colour->g * 255.0f;
            DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[2] = colour->b * 255.0f;

            DL_CURRENT_LIGHT(sCurrentGdDl).a.l.colc[0] = DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[0];
            DL_CURRENT_LIGHT(sCurrentGdDl).a.l.colc[1] = DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[1];
            DL_CURRENT_LIGHT(sCurrentGdDl).a.l.colc[2] = DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[2];
            // 801A0D04
            DL_CURRENT_LIGHT(sCurrentGdDl).l[0].l.col[0] = 0;
            DL_CURRENT_LIGHT(sCurrentGdDl).l[0].l.col[1] = 0;
            DL_CURRENT_LIGHT(sCurrentGdDl).l[0].l.col[2] = 0;

            DL_CURRENT_LIGHT(sCurrentGdDl).l[0].l.colc[0] = 0;
            DL_CURRENT_LIGHT(sCurrentGdDl).l[0].l.colc[1] = 0;
            DL_CURRENT_LIGHT(sCurrentGdDl).l[0].l.colc[2] = 0;

            gSPNumLights(next_gfx(), NUMLIGHTS_1);
            gSPLight(next_gfx(), osVirtualToPhysical(&DL_CURRENT_LIGHT(sCurrentGdDl).l), LIGHT_1);
            gSPLight(next_gfx(), osVirtualToPhysical(&DL_CURRENT_LIGHT(sCurrentGdDl).a), LIGHT_2);
            next_light();
            if (id > 0) {
                gd_enddlsplist();
            }
            return 0;
            break;
    }
    // L801A0EF4
    scaledColours[0] = (s32)(colour->r * sAmbScaleColour.r * 255.0f);
    scaledColours[1] = (s32)(colour->g * sAmbScaleColour.g * 255.0f);
    scaledColours[2] = (s32)(colour->b * sAmbScaleColour.b * 255.0f);
    // 801A0FE4
    DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[0] = scaledColours[0];
    DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[1] = scaledColours[1];
    DL_CURRENT_LIGHT(sCurrentGdDl).a.l.col[2] = scaledColours[2];
    // 801A1068
    DL_CURRENT_LIGHT(sCurrentGdDl).a.l.colc[0] = scaledColours[0];
    DL_CURRENT_LIGHT(sCurrentGdDl).a.l.colc[1] = scaledColours[1];
    DL_CURRENT_LIGHT(sCurrentGdDl).a.l.colc[2] = scaledColours[2];
    // 801A10EC
    gSPNumLights(next_gfx(), numLights);
    for (i = 0; i < numLights; i++) { // L801A1134
        scaledColours[0] = colour->r * sLightScaleColours[i].r * 255.0f;
        scaledColours[1] = colour->g * sLightScaleColours[i].g * 255.0f;
        scaledColours[2] = colour->b * sLightScaleColours[i].b * 255.0f;
        // 801A1260
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.col[0] = scaledColours[0];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.col[1] = scaledColours[1];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.col[2] = scaledColours[2];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.colc[0] = scaledColours[0];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.colc[1] = scaledColours[1];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.colc[2] = scaledColours[2];

        // 801A13B0
        lightDir[0] = (s8)sLightDirections[i].x;
        lightDir[1] = (s8)sLightDirections[i].y;
        lightDir[2] = (s8)sLightDirections[i].z;
        // 801A141C
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.dir[0] = lightDir[0];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.dir[1] = lightDir[1];
        DL_CURRENT_LIGHT(sCurrentGdDl).l[i].l.dir[2] = lightDir[2];
        // 801A14C4
        gSPLight(next_gfx(), osVirtualToPhysical(&DL_CURRENT_LIGHT(sCurrentGdDl).l[i]), i + 1);
    }
    // L801A1550
    gSPLight(next_gfx(), osVirtualToPhysical(&DL_CURRENT_LIGHT(sCurrentGdDl)), i + 1);
    next_light();
    gd_enddlsplist();
    return 0;
}

void set_Vtx_norm_buf_1(struct GdVec3f *norm) {
    sVtxCvrtNormBuf[0] = (s8)(norm->x * 127.0f);
    sVtxCvrtNormBuf[1] = (s8)(norm->y * 127.0f);
    sVtxCvrtNormBuf[2] = (s8)(norm->z * 127.0f);
}

void set_Vtx_norm_buf_2(struct GdVec3f *norm) {
    sVtxCvrtNormBuf[0] = (s8)(norm->x * 127.0f);
    sVtxCvrtNormBuf[1] = (s8)(norm->y * 127.0f);
    sVtxCvrtNormBuf[2] = (s8)(norm->z * 127.0f);
}

void set_gd_mtx_parameters(s32 params) {
    switch (params) {
        case G_MTX_PROJECTION | G_MTX_MUL | G_MTX_PUSH:
            sMtxParamType = G_MTX_PROJECTION;
            break;
        case G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH:
            sMtxParamType = G_MTX_MODELVIEW;
            break;
    }
}

/**
 * Adds a viewport to the current display list based on the current active view
 */
static void gd_dl_viewport(void) {
    Vp *vp;

    vp = &DL_CURRENT_VP(sCurrentGdDl);

    vp->vp.vscale[0] = (s16)(sActiveView->lowerRight.x * 2.0f);  // x scale
    vp->vp.vscale[1] = (s16)(sActiveView->lowerRight.y * 2.0f);  // y scale
    vp->vp.vscale[2] = 0x1FF;  // z scale
    vp->vp.vscale[3] = 0x000;

    vp->vp.vtrans[0] = (s16)((sActiveView->upperLeft.x * 4.0f) + (sActiveView->lowerRight.x * 2.0f));  // x offset
    vp->vp.vtrans[1] = (s16)((sActiveView->upperLeft.y * 4.0f) + (sActiveView->lowerRight.y * 2.0f));  // y offset
    vp->vp.vtrans[2] = 0x1FF;  // z offset
    vp->vp.vtrans[3] = 0x000;

    gSPViewport(next_gfx(), osVirtualToPhysical(vp));
    next_vp();
}

/* 2501D0 -> 250300 */
static void update_render_mode(void) {
    if ((sActiveView->flags & VIEW_ALLOC_ZBUF) != 0) {
        if (sAlpha != 0xff) {
            gDPSetRenderMode(next_gfx(), G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
        } else {
            gDPSetRenderMode(next_gfx(), G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2);
        }
    } else {
        if (sAlpha != 0xff) {
            gDPSetRenderMode(next_gfx(), G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        } else {
            gDPSetRenderMode(next_gfx(), G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2);
        }
    }
}

/* 250300 -> 250640 */
void Unknown801A1B30(void) {
    gDPPipeSync(next_gfx());
    gdSetFrameBuffer();
    gdDisplayListSetFillColor(&sActiveView->colour);
    gDPFillRectangle(next_gfx(), (u32)(sActiveView->upperLeft.x), (u32)(sActiveView->upperLeft.y),
                     (u32)(sActiveView->upperLeft.x + sActiveView->lowerRight.x - 1.0f),
                     (u32)(sActiveView->upperLeft.y + sActiveView->lowerRight.y - 1.0f));
    gDPPipeSync(next_gfx());
}

/* 250640 -> 250AE0 */
void gdClearZBuffer(void) {
    gDPPipeSync(next_gfx());
    gDPSetCycleType(next_gfx(), G_CYC_FILL);
    gDPSetRenderMode(next_gfx(), G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gdSetZBuffer();
    gDPSetColorImage(next_gfx(), G_IM_FMT_RGBA, G_IM_SIZ_16b, sActiveView->parent->lowerRight.x,
                     GD_LOWER_24(sActiveView->parent->zbuf));
    gDPSetFillColor(next_gfx(), GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0));
    gDPFillRectangle(next_gfx(), (u32)(sActiveView->upperLeft.x), (u32)(sActiveView->upperLeft.y),
                     (u32)(sActiveView->upperLeft.x + sActiveView->lowerRight.x - 1.0f),
                     (u32)(sActiveView->upperLeft.y + sActiveView->lowerRight.y - 1.0f));
    gDPPipeSync(next_gfx());
    gdSetFrameBuffer();
}

void gd_set_one_cycle(void) {
    gDPSetCycleType(next_gfx(), G_CYC_1CYCLE);
    update_render_mode();
}

/* 250B58 -> 250C18 */
void gddl_is_loading_shine_dl(s32 dlLoad) {
    if (dlLoad) {
        gSPDisplayList(next_gfx(), osVirtualToPhysical(&gd_dl_mario_face_shine));
    } else {
        gSPTexture(next_gfx(), 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineMode(next_gfx(), G_CC_SHADE, G_CC_SHADE);
    }
}

void start_view_dl(struct ObjView *view) {
    f32 ulx;
    f32 uly;
    f32 lrx;
    f32 lry;

    if (view->upperLeft.x < view->parent->upperLeft.x) {
        ulx = view->parent->upperLeft.x;
    } else {
        ulx = view->upperLeft.x;
    }

    if (view->upperLeft.x + view->lowerRight.x
        > view->parent->upperLeft.x + view->parent->lowerRight.x) {
        lrx = view->parent->upperLeft.x + view->parent->lowerRight.x;
    } else {
        lrx = view->upperLeft.x + view->lowerRight.x;
    }

    if (view->upperLeft.y < view->parent->upperLeft.y) {
        uly = view->parent->upperLeft.y;
    } else {
        uly = view->upperLeft.y;
    }

    if (view->upperLeft.y + view->lowerRight.y
        > view->parent->upperLeft.y + view->parent->lowerRight.y) {
        lry = view->parent->upperLeft.y + view->parent->lowerRight.y;
    } else {
        lry = view->upperLeft.y + view->lowerRight.y;
    }

    if (ulx >= lrx) {
        ulx = lrx - 1.0f;
    }
    if (uly >= lry) {
        uly = lry - 1.0f;
    }

    gDPSetScissor(next_gfx(), G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    gSPClearGeometryMode(next_gfx(), 0xFFFFFFFF);
    gSPSetGeometryMode(next_gfx(), G_LIGHTING | G_CULL_BACK | G_SHADING_SMOOTH | G_SHADE);
    if (view->flags & VIEW_ALLOC_ZBUF) {
        gSPSetGeometryMode(next_gfx(), G_ZBUFFER);
    }
    gd_dl_viewport();
    update_render_mode();
    gDPPipeSync(next_gfx());
}

void parse_p1_controller(void) {
    u32 i;
    struct GdControl *gdctrl = &gdControllerInfo;
    OSContPadEx *currInputs;
    OSContPadEx *prevInputs;

    // Copy current inputs to previous
    u8 *src = (u8 *) gdctrl;
    u8 *dest = (u8 *) gdctrl->prevFrame;
    for (i = 0; i < sizeof(struct GdControl); i++) {
        *dest++ = *src++;
    }

    gdctrl->unk50 = gdctrl->unk4C = gdctrl->dup = gdctrl->ddown = 0;

    currInputs = &sGdContPads[0];
    prevInputs = &sPrevFrameCont[0];
    // stick values
    gdctrl->stickXf     = currInputs->stick_x;
    gdctrl->stickYf     = currInputs->stick_y;
    gdctrl->stickDeltaX = gdctrl->stickX;
    gdctrl->stickDeltaY = gdctrl->stickY;
    gdctrl->stickX      = currInputs->stick_x;
    gdctrl->stickY      = currInputs->stick_y;
    gdctrl->stickDeltaX -= gdctrl->stickX;
    gdctrl->stickDeltaY -= gdctrl->stickY;
    // button values (as bools)
    gdctrl->trgL   = (currInputs->button & L_TRIG) != 0;
    gdctrl->trgR   = (currInputs->button & R_TRIG) != 0;
    gdctrl->btnA   = (currInputs->button & A_BUTTON) != 0;
    gdctrl->btnB   = (currInputs->button & B_BUTTON) != 0;
    gdctrl->cleft  = (currInputs->button & L_CBUTTONS) != 0;
    gdctrl->cright = (currInputs->button & R_CBUTTONS) != 0;
    gdctrl->cup    = (currInputs->button & U_CBUTTONS) != 0;
    gdctrl->cdown  = (currInputs->button & D_CBUTTONS) != 0;
    // but not these buttons??
    gdctrl->dleft  = currInputs->button & L_JPAD;
    gdctrl->dright = currInputs->button & R_JPAD;
    gdctrl->dup    = currInputs->button & U_JPAD;
    gdctrl->ddown  = currInputs->button & D_JPAD;

    if (gdctrl->btnA && !gdctrl->dragging) {
        gdctrl->startedDragging = TRUE;
    } else {
        gdctrl->startedDragging = FALSE;
    }
    // toggle if A is pressed? or is this just some seed for an rng?
    gdctrl->dragging = gdctrl->btnA;
    gdctrl->unkD8b20 = gdctrl->unkD8b40 = FALSE;
    gdctrl->AbtnPressWait = FALSE;

    if (gdctrl->startedDragging) {
        gdctrl->dragStartX = gdctrl->cursorX;
        gdctrl->dragStartY = gdctrl->cursorY;

        if (gdctrl->currFrame - gdctrl->dragStartFrame < 10) {
            gdctrl->AbtnPressWait = TRUE;
        }
    }

    if (gdctrl->dragging) {
        gdctrl->dragStartFrame = gdctrl->currFrame;
    }
    gdctrl->currFrame++;

    if (currInputs->button & START_BUTTON && !(prevInputs->button & START_BUTTON)) {
        gdctrl->newStartPress ^= 1;
    }

    if (currInputs->button & Z_TRIG && !(prevInputs->button & Z_TRIG)) {
        sCurrDebugViewIndex++;
    }

    if (sCurrDebugViewIndex > sDebugViewsCount) {
        sCurrDebugViewIndex = 0;
    } else if (sCurrDebugViewIndex < 0) {
        sCurrDebugViewIndex = sDebugViewsCount;
    }

    if (sCurrDebugViewIndex) {
        deactivate_timing();
    } else {
        activate_timing();
    }

    for (i = 0; ((s32) i) < sDebugViewsCount; i++) {
        sDebugViews[i]->flags &= ~VIEW_UPDATE;
    }

    if (sCurrDebugViewIndex) {
        sDebugViews[sCurrDebugViewIndex - 1]->flags |= VIEW_UPDATE;
    }

    // deadzone checks
    if (ABS(gdctrl->stickX) >= 6) {
        gdctrl->cursorX += gdctrl->stickX * 0.1;
    }
    if (ABS(gdctrl->stickY) >= 6) {
        gdctrl->cursorY -= gdctrl->stickY * 0.1;
    }

    // clamp cursor position within screen view bounds
    if (gdctrl->cursorX < sScreenView->parent->upperLeft.x + 16.0f) {
        gdctrl->cursorX = sScreenView->parent->upperLeft.x + 16.0f;
    }
    if (gdctrl->cursorX > sScreenView->parent->upperLeft.x + sScreenView->parent->lowerRight.x - 48.0f) {
        gdctrl->cursorX = sScreenView->parent->upperLeft.x + sScreenView->parent->lowerRight.x - 48.0f;
    }
    if (gdctrl->cursorY < sScreenView->parent->upperLeft.y + 16.0f) {
        gdctrl->cursorY = sScreenView->parent->upperLeft.y + 16.0f;
    }
    if (gdctrl->cursorY > sScreenView->parent->upperLeft.y + sScreenView->parent->lowerRight.y - 32.0f) {
        gdctrl->cursorY = sScreenView->parent->upperLeft.y + sScreenView->parent->lowerRight.y - 32.0f;
    }

    for (i = 0; i < sizeof(OSContPadEx); i++) {
        ((u8 *) prevInputs)[i] = ((u8 *) currInputs)[i];
    }
}

/* 251AF4 -> 251B40 */
void func_801A3324(f32 x, f32 y, f32 z) {
    D_801BD768.x = x;
    D_801BD768.y = y;
    D_801BD768.z = z;
    D_801BD758.x = x;
    D_801BD758.y = y;
    D_801BD758.z = z;
}

/* 251B40 -> 251BC8 */
void func_801A3370(f32 x, f32 y, f32 z) {
    gdDisplayListDisplacement(x, y, z);
    D_801BD768.x += x;
    D_801BD768.y += y;
    D_801BD768.z += z;
}

void border_active_view(void) {
    if (sActiveView->flags & VIEW_BORDERED) {
        gdDisplayListSetFillColor(gd_get_colour(1));
        gd_draw_border_rect(0.0f, 0.0f, (sActiveView->lowerRight.x - 1.0f),
                            (sActiveView->lowerRight.y - 1.0f));
    }
}

/* 251D44 -> 251DAC */
void gd_shading(s32 model) {
    switch (model) {
        case 9:
            break;
        case 10:
            break;
        default:
            fatal_printf("gd_shading(): Unknown shading model");
    }
}

/* 251DAC -> 251E18 */
s32 gd_getproperty(s32 prop, UNUSED void *arg1) {
    s32 got = FALSE;

    switch (prop) {
        case 3:
            got = TRUE;
            break;
        default:
            fatal_printf("gd_getproperty(): Unkown property");
    }

    return got;
}

/* 251E18 -> 2522B0 */
void gd_setproperty(enum GdProperty prop, f32 f1, f32 f2, f32 f3) {
    UNUSED f32 sp3C = 1.0f;
    s32 parm;

    switch (prop) {
        case GD_PROP_LIGHTING:
            parm = (s32) f1;
            switch (parm) {
                case 1:
                    gSPSetGeometryMode(next_gfx(), G_LIGHTING);
                    break;
                case 0:
                    gSPClearGeometryMode(next_gfx(), G_LIGHTING);
                    break;
            }
            break;
        case GD_PROP_AMB_COLOUR:
            sAmbScaleColour.r = f1;
            sAmbScaleColour.g = f2;
            sAmbScaleColour.b = f3;
            break;
        case GD_PROP_LIGHT_DIR:
            sLightDirections[sLightId].x = (s32)(f1 * 120.f);
            sLightDirections[sLightId].y = (s32)(f2 * 120.f);
            sLightDirections[sLightId].z = (s32)(f3 * 120.f);
            break;
        case GD_PROP_DIFUSE_COLOUR:
            sLightScaleColours[sLightId].r = f1;
            sLightScaleColours[sLightId].g = f2;
            sLightScaleColours[sLightId].b = f3;
            break;
        case GD_PROP_CULLING:
            parm = (s32) f1;
            switch (parm) {
                case 1:
                    gSPSetGeometryMode(next_gfx(), G_CULL_BACK);
                    break;
                case 0:
                    gSPClearGeometryMode(next_gfx(), G_CULL_BACK);
                    break;
            }
            break;
        case GD_PROP_OVERLAY:
            parm = (s32) f1;
            switch (parm) {
                case 1:
                    break;
                case 0:
                    break;
                default:
                    fatal_printf("Bad GD_OVERLAY parm");
            }
            break;
        case GD_PROP_ZBUF_FN:
            parm = (s32) f1;
            switch (parm) {
                case 23:
                    break;
                case 24:
                    break;
                case 25:
                    break;
                default:
                    fatal_printf("Bad zbuf function");
            }
            //? no break?
        case GD_PROP_STUB17:
            break;
        case GD_PROP_STUB18:
            break;
        case GD_PROP_STUB19:
            break;
        case GD_PROP_STUB20:
            break;
        case GD_PROP_STUB21:
            break;
        default:
            fatal_printf("gd_setproperty(): Unkown property");
    }
}

/* 2522C0 -> 25245C */
void gd_create_ortho_matrix(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    uintptr_t orthoMtx;
    uintptr_t rotMtx;

    // Should produce G_RDPHALF_1 in Fast3D
    gSPPerspNormalize(next_gfx(), 0xFFFF);

    guOrtho(&DL_CURRENT_MTX(sCurrentGdDl), l, r, b, t, n, f, 1.0f);
    orthoMtx = GD_LOWER_29(&DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), orthoMtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);

    next_mtx();
    guRotate(&DL_CURRENT_MTX(sCurrentGdDl), 0.0f, 0.0f, 0.0f, 1.0f);
    rotMtx = GD_LOWER_29(&DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), rotMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    func_801A3324(0.0f, 0.0f, 0.0f);
    next_mtx();
}

/* 25245C -> 25262C */
void gd_create_perspective_matrix(f32 fovy, f32 aspect, f32 near, f32 far) {
    u16 perspNorm;
    uintptr_t perspecMtx;
    uintptr_t rotMtx;
    UNUSED f32 unused = 0.0625f;

    sGdPerspTimer += 0.1;
    guPerspective(&DL_CURRENT_MTX(sCurrentGdDl), &perspNorm, fovy, aspect, near, far, 1.0f);

    gSPPerspNormalize(next_gfx(), perspNorm);

    perspecMtx = GD_LOWER_29(&DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), perspecMtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    next_mtx();

    guRotate(&DL_CURRENT_MTX(sCurrentGdDl), 0.0f, 0.0f, 0.0f, 1.0f);
    rotMtx = GD_LOWER_29(&DL_CURRENT_MTX(sCurrentGdDl));
    gSPMatrix(next_gfx(), rotMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    func_801A3324(0.0f, 0.0f, 0.0f);
    next_mtx();
}

/* 25262C -> 252AF8 */
s32 setup_view_buffers(const char *name, struct ObjView *view, UNUSED s32 ulx, UNUSED s32 uly,
                       UNUSED s32 lrx, UNUSED s32 lry) {
    char memtrackerName[0x100];

    if (view->flags & (VIEW_Z_BUF | VIEW_COLOUR_BUF) && !(view->flags & VIEW_UNK_1000)) {
        if (view->flags & VIEW_COLOUR_BUF) {
            sprintf(memtrackerName, "%s CBuf", name);
            start_memtracker(memtrackerName);
            view->colourBufs[0] =
                gd_malloc((u32)(2.0f * view->lowerRight.x * view->lowerRight.y + 64.0f), 0x20);

            if (view->flags & VIEW_2_COL_BUF) {
                view->colourBufs[1] =
                    gd_malloc((u32)(2.0f * view->lowerRight.x * view->lowerRight.y + 64.0f), 0x20);
            } else {
                view->colourBufs[1] = view->colourBufs[0];
            }

            view->colourBufs[0] = (void *) ALIGN64((uintptr_t) view->colourBufs[0]);
            view->colourBufs[1] = (void *) ALIGN64((uintptr_t) view->colourBufs[1]);
            stop_memtracker(memtrackerName);

            if (view->colourBufs[0] == NULL || view->colourBufs[1] == NULL) {
                fatal_printf("Not enough DRAM for colour buffers\n");
            }
            view->parent = view;
        } else {
            view->parent = sScreenView;
        }

        if (view->flags & VIEW_Z_BUF) {
            sprintf(memtrackerName, "%s ZBuf", name);
            start_memtracker(memtrackerName);
            if (view->flags & VIEW_ALLOC_ZBUF) {
                view->zbuf =
                    gd_malloc((u32)(2.0f * view->lowerRight.x * view->lowerRight.y + 64.0f), 0x40);
                if (view->zbuf == NULL) {
                    fatal_printf("Not enough DRAM for Z buffer\n");
                }
                view->zbuf = (void *) ALIGN64((uintptr_t) view->zbuf);
            }
            stop_memtracker(memtrackerName);
        } else {
            view->zbuf = sScreenView->zbuf;
        }
    } else {
        view->parent = sScreenView;
    }

    view->gdDlNum = 0;
    view->unk74 = 0;

    if (view->flags & VIEW_DEFAULT_PARENT) {
        view->parent = D_801A86E0;
    }

    return 0;
}

void gd_init_controllers(void) {
    OSContPadEx *p1cont = &sPrevFrameCont[0]; // 1c
    u32 i;                                  // 18

    osCreateMesgQueue(&D_801BE830, D_801BE848, ARRAY_COUNT(D_801BE848));
    osSetEventMesg(OS_EVENT_SI, &D_801BE830, (OSMesg) OS_MESG_SI_COMPLETE);
    osContInit(&D_801BE830, &D_801BAEA0, D_801BAE60);
    osContStartReadDataEx(&D_801BE830);

    for (i = 0; i < sizeof(OSContPadEx); i++) {
        ((u8 *) p1cont)[i] = 0;
    }
}

/* 252C08 -> 252C70 */
void func_801A4438(f32 x, f32 y, f32 z) {
    sTextDrawPos.x = x - (sActiveView->lowerRight.x / 2.0f);
    sTextDrawPos.y = (sActiveView->lowerRight.y / 2.0f) - y;
    sTextDrawPos.z = z;
}

/* 252C70 -> 252DB4 */
s32 gd_gentexture(void *texture, s32 fmt, s32 size, UNUSED u32 arg3, UNUSED u32 arg4) {
    UNUSED s32 sp2C;
    UNUSED s32 sp28 = 1;
    s32 dl; // 24

    switch (fmt) {
        case 29:
            fmt = 0;
            break;
        case 31:
            fmt = 3;
            break;
        default:
            fatal_printf("gd_gentexture(): bad format");
    }

    switch (size) {
        case 33:
            size = 2;
            sp2C = 16;
            break;
        default:
            fatal_printf("gd_gentexture(): bad size");
    }

    sLoadedTextures[++sTextureCount] = texture;
    dl = gd_startdisplist(7);

    if (dl == 0) {
        fatal_printf("Cant generate DL for texture");
    }
    gd_enddlsplist_parent();
    sTextureDisplayLists[sTextureCount] = dl;

    return dl;
}

/**
 * Unused (not called)
 */
void *load_texture_from_file(const char *file, s32 fmt, s32 size, u32 arg3, u32 arg4) {
    struct GdFile *txFile; // 3c
    void *texture;         // 38
    u32 txSize;            // 34
    u32 i;                 // 30
    u16 *txHalf;           // 2C
    u8 buf[3];             // 28
    u8 alpha;              // 27
    s32 dl;                // 20

    txFile = gd_fopen(file, "r");
    if (txFile == NULL) {
        fatal_print("Cant load texture");
    }
    txSize = gd_get_file_size(txFile);
    texture = gd_malloc_perm(txSize / 3 * 2);
    if (texture == NULL) {
        fatal_printf("Cant allocate memory for texture");
    }
    txHalf = (u16 *) texture;
    for (i = 0; i < txSize / 3; i++) {
        gd_fread((s8 *) buf, 3, 1, txFile);
        alpha = 0xFF;
        *txHalf = ((buf[2] >> 3) << 11) | ((buf[1] >> 3) << 6) | ((buf[0] >> 3) << 1) | (alpha >> 7);
        txHalf++;
    }
    gd_printf("Loaded texture '%s' (%d bytes)\n", file, txSize);
    gd_fclose(txFile);
    dl = gd_gentexture(texture, fmt, size, arg3, arg4);
    gd_printf("Generated '%s' (%d) display list ok.\n", file, dl);

    return texture;
}

/* 252F88 -> 252FAC */
void Unknown801A47B8(struct ObjView *v) {
    if (v->flags & VIEW_SAVE_TO_GLOBAL) {
        D_801BE994 = v;
    }
}

/**
 * Unused - called by func_801A520C and Unknown801A5344
 */
void func_801A4808(void) {
    while (D_801A8674 != 0) {
        ;
    }

    return;
}

/* 253018 -> 253084 */
void func_801A4848(s32 linkDl) {
    struct GdDisplayList *curDl;

    curDl = sCurrentGdDl;
    sCurrentGdDl = sMHeadMainDls[gGdFrameBufNum];
    branch_cur_dl_to_num(linkDl);
    sCurrentGdDl = curDl;
}

/* 2530A8 -> 2530C0 */
void stub_draw_label_text(char *s) {
    gd_printf("stub_draw_label_text: '%s'\n", s);
}

void set_active_view(struct ObjView *v) {
    sActiveView = v;
}

/**
 * Unused - called by func_801A520C
 */
void func_801A4918(void) {
    f32 x;     // c
    f32 y;     // 8
    u32 ydiff; // 4

    if (sHandView == NULL || sMenuView == NULL) {
        return;
    }

    x = sHandView->upperLeft.x;
    y = sHandView->upperLeft.y;

    if (!(x > sMenuView->upperLeft.x && x < sMenuView->upperLeft.x + sMenuView->lowerRight.x
          && y > sMenuView->upperLeft.y && y < sMenuView->upperLeft.y + sMenuView->lowerRight.y)) {
        return;
    }
    ydiff = (y - sMenuView->upperLeft.y) / 25.0f;

    if (ydiff < sItemsInMenu) {
        sMenuGadgets[ydiff]->drawFlags |= OBJ_HIGHLIGHTED;
    }
}

/* 2532D4 -> 2533DC */
void Unknown801A4B04(void) {
    if (D_801A86AC != NULL) {
        D_801A86AC->prevScaledTotal = 20.0f;
    }
    if (D_801A86A4 != NULL) {
        D_801A86A4->prevScaledTotal = (f32)((sDLGenTime * 50.0f) + 20.0f);
    }
    if (D_801A86A8 != NULL) {
        D_801A86A8->prevScaledTotal = (f32)((sDLGenTime * 50.0f) + 20.0f);
    }
    sDLGenTime = get_scaled_timer_total("dlgen");
    sRCPTime = get_scaled_timer_total("rcp");
    sDynamicsTime = get_scaled_timer_total("dynamics");
}

void update_cursor(void) {
    if (sHandView == NULL)
        return;

    if (gdControllerInfo.currFrame - gdControllerInfo.dragStartFrame < 300) {
        sHandView->flags |= VIEW_UPDATE;
        // by playing the sfx every frame, it will only play once as it
        // never leaves the "sfx played last frame" buffer
        gd_play_sfx(GD_SFX_HAND_APPEAR);
    } else {
        sHandView->flags &= ~VIEW_UPDATE;
        gd_play_sfx(GD_SFX_HAND_DISAPPEAR);
    }

    sHandView->upperLeft.x = (f32) gdControllerInfo.cursorX;
    sHandView->upperLeft.y = (f32) gdControllerInfo.cursorY;

    // Make hand display list
    begin_gddl(sHandShape->dlNums[gGdFrameBufNum]);
    if (gdControllerInfo.dragging) {
        gd_put_sprite((u16 *) gd_texture_hand_closed, sHandView->upperLeft.x, sHandView->upperLeft.y, 0x20, 0x20);
    } else {
        gd_put_sprite((u16 *) gd_texture_hand_open, sHandView->upperLeft.x, sHandView->upperLeft.y, 0x20, 0x20);
    }
    gd_enddlsplist_parent();

    if (sHandView->upperLeft.x < sHandView->parent->upperLeft.x) {
        sHandView->upperLeft.x = sHandView->parent->upperLeft.x;
    }
    if (sHandView->upperLeft.x > (sHandView->parent->upperLeft.x + sHandView->parent->lowerRight.x)) {
        sHandView->upperLeft.x = sHandView->parent->upperLeft.x + sHandView->parent->lowerRight.x;
    }

    if (sHandView->upperLeft.y < sHandView->parent->upperLeft.y) {
        sHandView->upperLeft.y = sHandView->parent->upperLeft.y;
    }
    if (sHandView->upperLeft.y > (sHandView->parent->upperLeft.y + sHandView->parent->lowerRight.y)) {
        sHandView->upperLeft.y = sHandView->parent->upperLeft.y + sHandView->parent->lowerRight.y;
    }
}

/* 253728 -> 2538E0 */
void Unknown801A4F58(void) {
    register s16 *cbufOff; // a0
    register s16 *cbufOn;  // a1
    register u16 *zbuf;    // a2
    register s16 colour;   // a3
    register s16 r;        // t0
    register s16 g;        // t1
    register s16 b;        // t2
    register s32 i;        // t3

    cbufOff = sScreenView->colourBufs[gGdFrameBufNum ^ 1];
    cbufOn = sScreenView->colourBufs[gGdFrameBufNum];
    zbuf = sScreenView->zbuf;

    for (i = 0; i < (320 * 240); i++) { // L801A4FCC
        colour = cbufOff[i];
        if (colour) {
            r = (s16)(colour >> 11 & 0x1F);
            g = (s16)(colour >> 6 & 0x1F);
            b = (s16)(colour >> 1 & 0x1F);
            if (--r < 0) {
                r = 0;
            }
            if (--g < 0) {
                g = 0;
            }
            if (--b < 0) {
                b = 0;
            }

            colour = (s16)(r << 11 | g << 6 | b << 1);
            cbufOff[i] = colour;
            cbufOn[i] = colour;
        } else { // L801A50D8
            zbuf[i] = 0xFFFC;
        }
    }
}

/* 2538E0 -> 253938 */
void Proc801A5110(struct ObjView *view) {
    if (view->flags & VIEW_UPDATE) {
        apply_to_obj_types_in_group(OBJ_TYPE_NETS, (applyproc_t) convert_net_verts, view->components);
    }
}

void update_view_and_dl(struct ObjView *view) {
    s32 prevFlags; // 18

    prevFlags = view->flags;
    update_view(view);
    if (prevFlags & VIEW_UPDATE) {
        sCurrentGdDl = sMHeadMainDls[gGdFrameBufNum];
        if (view->gdDlNum != 0) {
            func_801A4848(view->gdDlNum);
        }
    }
}

/* 253BC8 -> 2540E0 */
void gdInitSystem(void) {
    s32 i; // 34
    s8 *data; // 2c

    imin("gdInitSystem");
    i = (u32)(sMemBlockPoolSize - DOUBLE_SIZE_ON_64_BIT(0x3E800));
    data = gd_allocblock(i);
    gd_add_mem_to_heap(i, data, 0x10);
    sAlpha = (u16) 0xff;
    D_801A867C = 0;
    D_801A8680 = 0;
    sTextureCount = 0;
    gGdFrameBufNum = 0;
    D_801A86BC = 1;
    sItemsInMenu = 0;
    sDebugViewsCount = 0;
    sCurrDebugViewIndex = 0;
    sGdDlCount = 0;
    D_801A8674 = 0;
    sLightId = 0;
    sAmbScaleColour.r = 0.0f;
    sAmbScaleColour.g = 0.0f;
    sAmbScaleColour.b = 0.0f;

    for (i = 0; i < ARRAY_COUNT(sLightScaleColours); i++) {
        sLightScaleColours[i].r = 1.0f;
        sLightScaleColours[i].g = 0.0f;
        sLightScaleColours[i].b = 0.0f;
        sLightDirections[i].x = 0;
        sLightDirections[i].y = 120;
        sLightDirections[i].z = 0;
    }

    sNumLights = NUMLIGHTS_2;
    gd_set_identity_mat4(&gdIdentityMatrix);
    mat4_to_mtx(&gdIdentityMatrix, &gdRCPIdentityMatrix);
    remove_all_memtrackers();
    null_obj_lists();
    start_memtracker("total");
    remove_all_timers();

    start_memtracker("Static DL");
    sStaticDl = new_gd_dl(0, GODDARD_STATIC_DL_CMD_COUNT, 8000, 1, 300, 8);
    stop_memtracker("Static DL");

    start_memtracker("Dynamic DLs");
    sDynamicMainDls[0] = new_gd_dl(1, GODDARD_DYNAMIC_DL_CMD_COUNT, 10, 200, 10, 3);
    sDynamicMainDls[1] = new_gd_dl(1, GODDARD_DYNAMIC_DL_CMD_COUNT, 10, 200, 10, 3);
    stop_memtracker("Dynamic DLs");

    sMHeadMainDls[0] = new_gd_dl(1, GODDARD_FACE_DL_CMD_COUNT, 0, 0, 0, 0);
    sMHeadMainDls[1] = new_gd_dl(1, GODDARD_FACE_DL_CMD_COUNT, 0, 0, 0, 0);

    for (i = 0; i < ARRAY_COUNT(sViewDls); i++) {
        sViewDls[i][0] = create_child_gdl(1, sDynamicMainDls[0]);
        sViewDls[i][1] = create_child_gdl(1, sDynamicMainDls[1]);
    }

    sScreenView =
        make_view("screenview2", (VIEW_2_COL_BUF | VIEW_UNK_1000 | VIEW_COLOUR_BUF | VIEW_Z_BUF), 0, 0,
                  0, 320, 240, NULL);
    sScreenView->colour.r = 0.0f;
    sScreenView->colour.g = 0.0f;
    sScreenView->colour.b = 0.0f;
    sScreenView->parent = sScreenView;
    sScreenView->flags &= ~VIEW_UPDATE;
    sActiveView = sScreenView;

    // Zero out controller inputs
    data = (s8 *) &gdControllerInfo;
    for (i = 0; (u32) i < sizeof(struct GdControl); i++) {
        *data++ = 0;
    }

    // 801A5868
    gdControllerInfo.unk88 = 1.0f;
    gdControllerInfo.unkA0 = -45.0f;
    gdControllerInfo.unkAC = 45.0f;
    gdControllerInfo.unk00 = 2;
    gdControllerInfo.newStartPress = FALSE;
    gdControllerInfo.prevFrame = &gdControllerInfoPrevFrame;
    gdControllerInfo.cursorX = 160;
    gdControllerInfo.cursorY = 120;
    gdControllerInfo.dragStartFrame = -1000;
    // unusedDl801BB0AC = create_mtl_gddl(4);
    imout();
}

/**
 * Initializes the pick buffer. This functions like the `pick` or `gselect`
 * functions from IRIS GL.
 * @param buf  pointer to an array of 16-bit values
 * @param len  maximum number of values to store
 */
void init_pick_buf(s16 *buf, s32 len) {
    buf[0] = 0;
    buf[1] = 0;
    sPickBufLen = len;
    sPickBuf = buf;
    sPickBufPosition = 0;
}

/**
 * Stores a 16-bit value into the pick buffer. This functions like the
 * `pushname` function from IRIS GL.
 */
void store_in_pickbuf(s16 data) {
    sPickBuf[sPickBufPosition++] = data;
}

/** Divides by 3, since in the final game, only thing stored
** in the pick buf is a tupple of three halves: (datasize, objtype, objnumber)
** (datasize is always 2) */
s32 get_cur_pickbuf_offset(UNUSED s16 *arg0) {
    return sPickBufPosition / 3;
}

/* 254288 -> 2542B0 */
void *Unknown801A5AB8(s32 texnum) {
    return sLoadedTextures[texnum];
}

/* 2542B0 -> 254328 */
void Unknown801A5AE0(s32 arg0) {
    D_801BB018 = arg0;
    if (D_801BB01C != D_801BB018) {
        branch_cur_dl_to_num(sTextureDisplayLists[arg0]);
        D_801BB01C = D_801BB018;
    }
}

void set_vtx_tc_buf(f32 tcS, f32 tcT) {
    sVtxCvrtTCBuf[0] = (s16)(tcS * 512.0f);
    sVtxCvrtTCBuf[1] = (s16)(tcT * 512.0f);
}

/* 2543B8 -> 2543F4 */
void add_debug_view(struct ObjView *view) {
    sDebugViews[sDebugViewsCount++] = view;
}

union ObjVarVal *cvrt_val_to_kb(union ObjVarVal *dst, union ObjVarVal src) {
    union ObjVarVal temp;

    temp.f = src.f / 1024.0; //? 1024.0f
    return (*dst = temp, dst);
}

/* 254450 -> 254560 */
UNUSED void Unknown801A5C80(struct ObjGroup *parentGroup) {
    struct ObjLabel *label;      // 3c
    struct ObjGroup *debugGroup; // 38

    dStartGroup("debugg");
    label = (struct ObjLabel *) dMakeObject(D_LABEL, 0);
    dSetRelativePosition(10.0f, 230.0f, 0.0f);
    dSetParmPointer(PARM_PTR_CHAR, gd_strdup("FT %2.2f"));
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTracked1FrameTime);
    label->unk30 = 3;
    dEndGroup("debugg");

    debugGroup = (struct ObjGroup *) dUseObject("debugg");
    make_view("debugview", (VIEW_2_COL_BUF | VIEW_ALLOC_ZBUF | VIEW_1_CYCLE | VIEW_DRAW), 2, 0, 0, 320,
              240, debugGroup);

    if (parentGroup != NULL) {
        addto_group(parentGroup, &debugGroup->header);
    }
}

void gd_put_sprite(u16 *sprite, s32 x, s32 y, s32 wx, s32 wy) {
    s32 column; // 5c
    s32 row; // 58

    gSPDisplayList(next_gfx(), osVirtualToPhysical(gd_dl_sprite_start_tex_block));
    for (row = 0; row < wy; row += 32) {
        for (column = 0; column < wx; column += 32) {
             gDPLoadTextureBlock(next_gfx(), (row * 32) + sprite + column, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
                G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD)
             gSPTextureRectangle(next_gfx(), x << 2, (y + row) << 2, (x + 32) << 2, (y + row + 32) << 2,
                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }
    }

    gDPPipeSync(next_gfx());
    gDPSetCycleType(next_gfx(), G_CYC_1CYCLE);
    gDPSetRenderMode(next_gfx(), G_RM_AA_ZB_OPA_INTER, G_RM_NOOP2);
    gSPTexture(next_gfx(), 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
}

void gdInitMouse(struct ObjGroup *parentgrp) {
    struct ObjView *mouseview; // 34
    struct ObjGroup *mousegrp; // 30

    sHandShape = make_shape(0, "mouse");
    sHandShape->dlNums[0] = gd_startdisplist(7);
    gd_put_sprite((u16 *) gd_texture_hand_open, 100, 100, 32, 32);
    gd_enddlsplist_parent();
    sHandShape->dlNums[1] = gd_startdisplist(7);
    gd_put_sprite((u16 *) gd_texture_hand_open, 100, 100, 32, 32);
    gd_enddlsplist_parent();

    dStartGroup("mouseg");
    dMakeObject(D_NET, AsDynName(0));
    dSetInitPos(0.0f, 0.0f, 0.0f);
    dSetType(3);
    dSetShapePointerFromPointer(&sHandShape);
    dEndGroup("mouseg");

    mousegrp = (struct ObjGroup *) dUseObject("mouseg");
    mouseview = make_view("mouseview",
                          (VIEW_2_COL_BUF | VIEW_ALLOC_ZBUF | VIEW_1_CYCLE | VIEW_MOVEMENT | VIEW_DRAW),
                          2, 0, 0, 32, 32, mousegrp);
    mouseview->flags &= ~VIEW_UPDATE;
    sHandView = mouseview;
    if (parentgrp != NULL) {
        addto_group(parentgrp, &mousegrp->header);
    }
}

/**
 * This prints all timers if the view was not updated for a frame
 **/
void view_proc_print_timers(struct ObjView *self) {
    if (self->flags & VIEW_WAS_UPDATED) {
        return;
    }

    print_all_timers();
}

void make_timer_gadgets(void) {
    struct ObjLabel *timerLabel;
    struct ObjGroup *timerg;
    struct ObjView *timersview;
    struct ObjGadget *bar1;
    struct ObjGadget *bar2;
    struct ObjGadget *bar3;
    struct ObjGadget *bar4;
    struct ObjGadget *bar5;
    struct ObjGadget *bar6;
    struct GdTimer *timer;
    s32 i;
    char timerNameBuf[0x20];

    dStartGroup("timerg");
    dMakeObject(D_GADGET, "bar1");
    dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
    dSetWorldPos(20.0f, 5.0f, 0.0f);
    dSetScale(50.0f, 5.0f, 0.0f);
    dSetType(4);
    dSetParmF(PARM_F_RANGE_MIN, 0);
    dSetParmF(PARM_F_RANGE_MAX, sTimeScaleFactor);
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTimeScaleFactor);
    bar1 = (struct ObjGadget *) dUseObject("bar1");
    bar1->colourNum = COLOUR_WHITE;

    dMakeObject(D_GADGET, "bar2");
    dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
    dSetWorldPos(70.0f, 5.0f, 0.0f);
    dSetScale(50.0f, 5.0f, 0.0f);
    dSetType(4);
    dSetParmF(PARM_F_RANGE_MIN, 0);
    dSetParmF(PARM_F_RANGE_MAX, sTimeScaleFactor);
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTimeScaleFactor);
    bar2 = (struct ObjGadget *) dUseObject("bar2");
    bar2->colourNum = COLOUR_PINK;

    dMakeObject(D_GADGET, "bar3");
    dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
    dSetWorldPos(120.0f, 5.0f, 0.0f);
    dSetScale(50.0f, 5.0f, 0.0f);
    dSetType(4);
    dSetParmF(PARM_F_RANGE_MIN, 0);
    dSetParmF(PARM_F_RANGE_MAX, sTimeScaleFactor);
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTimeScaleFactor);
    bar3 = (struct ObjGadget *) dUseObject("bar3");
    bar3->colourNum = COLOUR_WHITE;

    dMakeObject(D_GADGET, "bar4");
    dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
    dSetWorldPos(170.0f, 5.0f, 0.0f);
    dSetScale(50.0f, 5.0f, 0.0f);
    dSetType(4);
    dSetParmF(PARM_F_RANGE_MIN, 0);
    dSetParmF(PARM_F_RANGE_MAX, sTimeScaleFactor);
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTimeScaleFactor);
    bar4 = (struct ObjGadget *) dUseObject("bar4");
    bar4->colourNum = COLOUR_PINK;

    dMakeObject(D_GADGET, "bar5");
    dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
    dSetWorldPos(220.0f, 5.0f, 0.0f);
    dSetScale(50.0f, 5.0f, 0.0f);
    dSetType(4);
    dSetParmF(PARM_F_RANGE_MIN, 0);
    dSetParmF(PARM_F_RANGE_MAX, sTimeScaleFactor);
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTimeScaleFactor);
    bar5 = (struct ObjGadget *) dUseObject("bar5");
    bar5->colourNum = COLOUR_WHITE;

    dMakeObject(D_GADGET, "bar6");
    dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
    dSetWorldPos(270.0f, 5.0f, 0.0f);
    dSetScale(50.0f, 5.0f, 0.0f);
    dSetType(4);
    dSetParmF(PARM_F_RANGE_MIN, 0);
    dSetParmF(PARM_F_RANGE_MAX, sTimeScaleFactor);
    dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &sTimeScaleFactor);
    bar6 = (struct ObjGadget *) dUseObject("bar6");
    bar6->colourNum = COLOUR_PINK;

    for (i = 0; i < GD_NUM_TIMERS; i++) {
        sprintf(timerNameBuf, "tim%d\n", i);

        timer = get_timernum(i);

        dMakeObject(D_GADGET, timerNameBuf);
        dSetObjectDrawFlag(OBJ_IS_GRABBABLE);
        dSetWorldPos(20.0f, (f32)((i * 15) + 15), 0.0f);
        dSetScale(50.0f, 14.0f, 0);
        dSetType(4);
        dSetParmF(PARM_F_RANGE_MIN, 0.0f);
        dSetParmF(PARM_F_RANGE_MAX, 1.0f);
        dAddValuePointer(NULL, 0, OBJ_VALUE_FLOAT, (uintptr_t) &timer->prevScaledTotal);
        sTimerGadgets[i] = (struct ObjGadget *) dUseObject(timerNameBuf);
        sTimerGadgets[i]->colourNum = timer->gadgetColourNum;

        timerLabel = (struct ObjLabel *) dMakeObject(D_LABEL, AsDynName(0));
        dSetRelativePosition(5.0f, 14.0f, 0);
        dSetParmPointer(PARM_PTR_CHAR, (void *) timer->name);
        dAddValuePointer(timerNameBuf, 0x40000, 0, (uintptr_t) NULL);
        timerLabel->unk30 = 3;
    }

    dEndGroup("timerg");
    timerg = (struct ObjGroup *) dUseObject("timerg");
    timersview = make_view(
        "timersview", (VIEW_2_COL_BUF | VIEW_ALLOC_ZBUF | VIEW_1_CYCLE | VIEW_MOVEMENT | VIEW_DRAW), 2,
        0, 10, 320, 270, timerg);
    timersview->colour.r = 0.0f;
    timersview->colour.g = 0.0f;
    timersview->colour.b = 0.0f;
    timersview->flags &= ~VIEW_UPDATE;
    timersview->proc = view_proc_print_timers;
    add_debug_view(timersview);

    return;
}

#ifndef NO_SEGMENTED_MEMORY
/**
 * Copies `size` bytes of data from ROM address `romAddr` to RAM address `vAddr`.
 */
static void gd_block_dma(u32 romAddr, void *vAddr, s32 size) {
    s32 blockSize;

    do {
        if ((blockSize = size) > 0x1000) {
            blockSize = 0x1000;
        }

        osPiStartDma(&sGdDMAReqMesg, OS_MESG_PRI_NORMAL, OS_READ, romAddr, vAddr, blockSize, &sGdDMAQueue);
        osRecvMesg(&sGdDMAQueue, &sGdDMACompleteMsg, OS_MESG_BLOCK);
        romAddr += blockSize;
        vAddr = (void *) ((uintptr_t) vAddr + blockSize);
        size -= 0x1000;
    } while (size > 0);
}

/**
 * Loads the specified DynList from ROM and processes it.
 */
struct GdObj *load_dynlist(struct DynList *dynlist) {
    u32 segSize;
    u8 *allocSegSpace;
    void *allocPtr;
    uintptr_t dynlistSegStart;
    uintptr_t dynlistSegEnd;
    s32 i;
    s32 tlbEntries;
    struct GdObj *loadedList;

    i = -1;

    // Make sure the dynlist exists
    while (sDynLists[++i].list != NULL) {
        if (sDynLists[i].list == dynlist) {
            break;
        }
    }
    if (sDynLists[i].list == NULL) {
        fatal_printf("load_dynlist() ptr not found in any banks");
    }

    switch (sDynLists[i].flag) {
        case STD_LIST_BANK:
            dynlistSegStart = (uintptr_t) _faceDataSegmentRomStart;
            dynlistSegEnd = (uintptr_t) _faceDataSegmentRomEnd;
            break;
        default:
            fatal_printf("load_dynlist() unkown bank");
    }

    segSize = dynlistSegEnd - dynlistSegStart;
    allocSegSpace = gd_malloc_temp(segSize + PAGE_SIZE);

    if ((allocPtr = (void *) allocSegSpace) == NULL) {
        fatal_printf("Not enough DRAM for DATA segment \n");
    }

    allocSegSpace = (u8 *) (((uintptr_t) allocSegSpace + PAGE_SIZE) & 0xFFFF0000);

    // Copy the dynlist data from ROM
    gd_block_dma(dynlistSegStart, (void *) allocSegSpace, segSize);

    osUnmapTLBAll();

    tlbEntries = (segSize / PAGE_SIZE) / 2 + 1;
    if (tlbEntries >= 31) {
        fatal_printf("load_dynlist() too many TLBs");
    }

    // Map virtual address 0x04000000 to `allocSegSpace`
    for (i = 0; i < tlbEntries; i++) {
        osMapTLB(i, OS_PM_64K,
            (void *) (uintptr_t) (0x04000000 + (i * 2 * PAGE_SIZE)),  // virtual address to map
            GD_LOWER_24(((uintptr_t) allocSegSpace) + (i * 2 * PAGE_SIZE) + 0),  // even page address
            GD_LOWER_24(((uintptr_t) allocSegSpace) + (i * 2 * PAGE_SIZE) + PAGE_SIZE),  // odd page address
            -1);
    }

    // process the dynlist
    loadedList = gdProcessDynList(dynlist);

    gd_free(allocPtr);
    osUnmapTLBAll();

    return loadedList;
}
#else
struct GdObj *load_dynlist(struct DynList *dynlist) {
    return gdProcessDynList(dynlist);
}
#endif
