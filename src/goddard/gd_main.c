/**
 * All functions referenced externally to the face driver
 */

#include <PR/ultratypes.h>

#include "debug_utils.h"
#include "gd_main.h"
#include "gd_memory.h"
#include "macros.h"
#include "objects.h"
#include "renderer.h"
#include "sfx.h"
#include "skin.h"

#include "buffers/zbuffer.h"
#include "buffers/framebuffers.h"
#include "game/memory.h"

static s32 sUpdateMarioScene; // update dl Vtx from ObjVertex?
s32 gGdMoveScene = TRUE;    // Whether objects should move at all
s32 gdUseSmoothShading = TRUE; // Set to FALSE to use face normals
OSContPadEx sGdContPads[4];
struct GdControl gdControllerInfo;
struct GdControl gdControllerInfoPrevFrame;

/**
 * Copies the player1 controller data from p1cont to sGdContPads[0].
 */
void gd_copy_p1_contpad(OSContPadEx *p1cont) {
    u32 i;                                    // 24
    u8 *src = (u8 *) p1cont;             // 20
    u8 *dest = (u8 *) &sGdContPads[0]; // 1c

    for (i = 0; i < sizeof(OSContPadEx); i++) {
        dest[i] = src[i];
    }

    if (p1cont->button & Z_TRIG) {
        print_all_timers();
    }
}

s32 gd_sfx_to_play(void) {
    return gd_new_sfx_to_play();
}

// Singular entrypoint to the Goddard system
void gdInit(s16 sceneID) {
    imin(__FUNCTION__);

    void *addr = main_pool_alloc(DOUBLE_SIZE_ON_64_BIT(GODDARD_TOTAL_HEAP_SIZE), MEMORY_POOL_LEFT);
    if (addr != NULL) {
        gdInitMemory(addr, DOUBLE_SIZE_ON_64_BIT(GODDARD_TOTAL_HEAP_SIZE));
        gdAddMemoryToHeap(gZBuffer, sizeof(gZBuffer)); // 0x25800
        gdAddMemoryToHeap(gFramebuffers, sizeof(gFramebuffers)); // 0x70800
        sUpdateMarioScene = FALSE;
        gdSetupFace();
        gdLoadScene(sceneID);
    }

    imout();
}

/**
 * Runs every frame at V-blank. Handles input and updates state.
 */
void gdSceneTick(void) {
    gd_sfx_update();
    if (sUpdateMarioScene) {
        apply_to_obj_types_in_group(OBJ_TYPE_NETS, (applyproc_t) convert_net_verts, sMarioSceneGrp);
    }
    sUpdateMarioScene = FALSE;
    gGdFrameBufNum ^= 1;
    reset_cur_dl_indices();
    parse_p1_controller();
    update_cursor();
}

// Draws the scene
Gfx *gdDrawScene(s32 sceneID) {
    struct GdDisplayList *gddl;

    start_timer("dlgen");
    gddl = NULL;

    switch (sceneID) {
        case GD_SCENE_REGULAR_MARIO:
        case GD_SCENE_DIZZY_MARIO:
            setup_timers();
            update_view_and_dl(sMSceneView);
            if (sHandView != NULL) {
                update_view_and_dl(sHandView);
            }
            sCurrentGdDl = sMHeadMainDls[gGdFrameBufNum];
            gdFinishDrawing();
            gddl = sCurrentGdDl;
            sUpdateMarioScene = TRUE;
            break;
        default:
            fatal_printf("gdDrawScene(%d): No associated Scene!", sceneID);
    }

    if (gddl == NULL) {
        fatal_printf("no display list");
    }
    stop_timer("dlgen");
    return (void *) osVirtualToPhysical(gddl->gfx);
}


