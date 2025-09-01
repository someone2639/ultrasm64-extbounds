#ifndef GD_RENDERER_H
#define GD_RENDERER_H

#include <PR/ultratypes.h>
#include <PR/os_cont.h>

#include "gd_types.h"
#include "macros.h"

// size of a 64K TLB page
#define PAGE_SIZE 65536

#define MAX_GD_DLS 1000
#define OS_MESG_SI_COMPLETE 0x33333333

#ifndef NO_SEGMENTED_MEMORY
#define GD_VIRTUAL_TO_PHYSICAL(addr) ((uintptr_t)(addr) &0x0FFFFFFF)
#define GD_LOWER_24(addr) ((uintptr_t)(addr) &0x00FFFFFF)
#define GD_LOWER_29(addr) (((uintptr_t)(addr)) & 0x1FFFFFFF)
#else
#define GD_VIRTUAL_TO_PHYSICAL(addr) (addr)
#define GD_LOWER_24(addr) ((uintptr_t)(addr))
#define GD_LOWER_29(addr) (((uintptr_t)(addr)))
#endif

#define MTX_INTPART_PACK(w1, w2) (((w1) &0xFFFF0000) | (((w2) >> 16) & 0xFFFF))
#define MTX_FRACPART_PACK(w1, w2) ((((w1) << 16) & 0xFFFF0000) | ((w2) &0xFFFF))
#define LOOKAT_PACK(c) ((s32) MIN(((c) * (128.0)), 127.0) & 0xff)


/// Properties types used in [gd_setproperty](@ref gd_setproperty); most are stubbed out.
enum GdProperty {
    GD_PROP_OVERLAY       = 4,
    GD_PROP_LIGHTING      = 11,
    GD_PROP_AMB_COLOUR    = 12,
    GD_PROP_DIFUSE_COLOUR = 13,
    GD_PROP_LIGHT_DIR     = 15,
    GD_PROP_CULLING       = 16,
    GD_PROP_STUB17        = 17,
    GD_PROP_STUB18        = 18,
    GD_PROP_STUB19        = 19,
    GD_PROP_STUB20        = 20,
    GD_PROP_STUB21        = 21,
    GD_PROP_ZBUF_FN       = 22
};

enum GdSceneId {
    GD_SCENE_YOSHI,  // create yoshi?
    GD_SCENE_YOSHI1,  // destroy yoshi?
    GD_SCENE_REGULAR_MARIO,
    GD_SCENE_DIZZY_MARIO,
    GD_SCENE_CAR,   // create car?
    GD_SCENE_CAR5  // destroy car?
};

// structs
struct GdDisplayList {
    /* Vertices */
    /*0x00*/ s32 curVtxIdx;
    /*0x04*/ s32 totalVtx;
    /*0x08*/ Vtx *vtx;
    /* Matrices */
    /*0x0C*/ s32 curMtxIdx;
    /*0x10*/ s32 totalMtx;
    /*0x14*/ Mtx *mtx;
    /* Lights */
    /*0x18*/ s32 curLightIdx;
    /*0x1C*/ s32 totalLights;
    /*0x20*/ Lights4 *light;
    /* Gfx-es */
    /*0x24*/ s32 curGfxIdx;
    /*0x28*/ s32 totalGfx;
    /*0x2C*/ Gfx *gfx;    // active position in DL
    /*0x30*/ Gfx **dlptr; // pointer to list/array of display lists for each frame?
                          /* Viewports */
    /*0x34*/ s32 curVpIdx;
    /*0x38*/ s32 totalVp;
    /*0x3C*/ Vp *vp;
    /* GD DL Info */
    /*0x40*/ u32 id;     // user specified
    /*0x44*/ u32 number; // count
    /*0x4C*/ struct GdDisplayList *parent; // not quite sure?
};                                         /* sizeof = 0x50 */
// accessor macros for gd dl
#define DL_CURRENT_VTX(dl)   ((dl)->vtx[(dl)->curVtxIdx])
#define DL_CURRENT_MTX(dl)   ((dl)->mtx[(dl)->curMtxIdx])
#define DL_CURRENT_LIGHT(dl) ((dl)->light[(dl)->curLightIdx])
#define DL_CURRENT_GFX(dl)   ((dl)->gfx[(dl)->curGfxIdx])
#define DL_CURRENT_VP(dl)    ((dl)->vp[(dl)->curVpIdx])

struct LightDirVec {
    s32 x, y, z;
};

enum DynListBankFlag { TABLE_END = -1, STD_LIST_BANK = 3 };

struct DynListBankInfo {
    /* 0x00 */ enum DynListBankFlag flag;
    /* 0x04 */ struct DynList *list;
};

// data
extern s32 gGdFrameBufNum;
extern struct ObjGroup *sMarioSceneGrp;
extern struct ObjView *sMSceneView;
extern struct ObjView *sHandView;
extern struct GdDisplayList *sCurrentGdDl;
extern struct GdDisplayList *sMHeadMainDls[2];

// functions
u32 get_alloc_mem_amt(void);
s32 gd_get_ostime(void);
f32 get_time_scale(void);
f64 gdSine(f64 x);
f64 gdCosine(f64 x);
f64 gdSqrt(f64 x);

#if defined(ISVPRINT) || defined(UNF)
#define gdPrintf osSyncPrintf
#else
void gdPrintf(const char *format, ...);
#endif
void gdExit(UNUSED s32 code) NORETURN;
void gdFree(void *ptr);
void *gdAllocateMemoryBlock(u32 size);
void *gdMalloc(u32 size, u8 perm);
void *gdMallocPermanent(u32 size);
void *gdMallocTemporary(u32 size);
void draw_indexed_dl(s32 dlNum, s32 gfxIdx);
void gdAddMemoryToHeap(void *addr, u32 size);
void gdInitMemory(void *blockpool, u32 size);
void gdSetupFace(void);
void gdLoadScene(s32 id);
void gdFinishDrawing();
void gdFillRectangle(f32 ulx, f32 uly, f32 lrx, f32 lry);
void gdFillRectangleBorder(f32 ulx, f32 uly, f32 lrx, f32 lry);
void gdDisplayListSetFillColor(struct GdColour *colour);
void gdDisplayListStash(void);
void gdDisplayListStashPop(void);
s32 gdInitDisplayList(s32 memarea);
s32 gd_enddlsplist_parent(void);
void gdLoadMatrix(Mat4f *mtx);
void gdDisplayListPushMatrix(void);
void gdDisplayListPopMatrix(void);
void gdDisplayListDisplacement(f32 x, f32 y, f32 z);
void gdDisplayListTranslate(f32 x, f32 y, f32 z);
void gd_dl_scale(f32 x, f32 y, f32 z);
void func_8019F2C4(f32 arg0, s8 arg1);
void gd_dl_lookat(struct ObjCamera *cam, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);
void gdVerifyTriangle(s32 vtxcount);
Vtx *gdMakeVertex(f32 x, f32 y, f32 z, f32 alpha);
void gdTriangleRegistered(void);
void gdMakeTriangle(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3);
void gdFlushVtxBuffer(void);
void gdDrawVtxTriBuffers(void);
void set_render_alpha(f32 arg0);
void set_light_id(s32 index);
void set_light_num(s32 n);
s32 create_mtl_gddl(s32 mtlType);
void branch_to_gddl(s32 dlNum);
void gd_dl_hilite(s32, struct ObjCamera *, struct GdVec3f *, struct GdVec3f *, struct GdVec3f *, struct GdColour *);
void gd_dl_hilite(s32 idx, struct ObjCamera *cam, UNUSED struct GdVec3f *arg2, UNUSED struct GdVec3f *arg3,
                   struct GdVec3f *arg4, struct GdColour *colour);
s32 gd_dl_material_lighting(s32 id, struct GdColour *colour, s32 material);
void gdSetNormalToFace(struct GdVec3f *norm);
void gdSetNormalToVertex(struct GdVec3f *norm);
void set_gd_mtx_parameters(s32 params);
void gd_set_one_cycle(void);
void gddl_is_loading_stub_dl(UNUSED s32 dlLoad);
void start_view_dl(struct ObjView *view);
void border_active_view(void);
void gd_shading(s32 model);
s32 gd_getproperty(s32 prop, UNUSED void *arg1);
void gd_setproperty(enum GdProperty prop, f32 f1, f32 f2, f32 f3);
void gd_create_ortho_matrix(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);
void gd_create_perspective_matrix(f32 fovy, f32 aspect, f32 near, f32 far);
s32 setup_view_buffers(const char *name, struct ObjView *view, UNUSED s32 ulx, UNUSED s32 uly,
                       UNUSED s32 lrx, UNUSED s32 lry);
void gd_init_controllers(void);
long defpup(UNUSED const char *menufmt, ...);
void menu_cb_control_type(u32);
void menu_cb_recalibrate_controller(u32);
void func_801A4438(f32 x, f32 y, f32 z);
void stub_draw_label_text(UNUSED char *s);
void gdSetActiveView(struct ObjView *v);
void func_801A520C(void);
void gdInitSystem(void);
void init_pick_buf(s16 *buf, s32 len);
void store_in_pickbuf(s16 data);
s32 get_cur_pickbuf_offset(UNUSED s16 *arg0);
void set_vtx_tc_buf(f32 tcS, f32 tcT);
struct GdObj *load_dynlist(struct DynList *dynlist);

u32 new_gddl_from(Gfx *, s32);
void gdInitMouse(struct ObjGroup *);
void parse_p1_controller(void);
void update_cursor(void);
void update_view_and_dl(struct ObjView *);
void gdSetSpecular(s32);
void func_801A3370(f32, f32, f32);
void gd_put_sprite(u16 *, s32, s32, s32, s32);
void reset_cur_dl_indices(void);

#endif // GD_RENDERER_H
