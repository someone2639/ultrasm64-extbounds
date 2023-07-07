#include <ultra64.h>
#include <PR/gs2dex.h>
#include "game_init.h"
#include "engine/math_util.h"
#include "contpakreal.h"

#include "troll_sprites/bg.h"
#include "troll_sprites/cursor.h"


#define qs510(n) ((s16)((n)*0x0400))
#define qs1516(n) ((s32)((n)*0x00010000))
#define qs102(n) ((s16)((n)*0x0004))

typedef struct BB {
    u16 ulx; u16 uly;
    u16 lrx; u16 lry;
} Bound;
int BB_inside(Bound *b, u16 x, u16 y) {
    return ((x <= b->lrx) && (x >= b->ulx))
        && ((y <= b->uly) && (y >= b->lry));
}



enum CPR_GAMESTATE {
    CPR_INIT = 0,
    CPR_ZOOMIN,
    CPR_GAME,
    CPR_ZOOMOUT,
    CPR_FINISH,
};
u32 cpr_GameState = CPR_INIT;
u32 cpr_NextState = CPR_INIT;
u32 cpr_Timer = 0;


void cpr_drawscreenzoom() {
    static float scale = 0.1f;
    uObjBg *b = segmented_to_virtual(&bg_bg);

    b->s.scaleW = b->s.scaleH = qs510(scale);

    scale = approach_f32_asymptotic(scale, 1.0f, 2.0f);
    if (scale > 1.0f) {
        scale = 1.0f;
        cpr_NextState = CPR_GAME;
    }

    gSPDisplayList(gDisplayListHead++, bg_bg_dl);
}
void cpr_drawscreen() {
    gSPDisplayList(gDisplayListHead++, bg_bg_dl);
}


void cpr_drawsprites() {
    // for paint and colors
    // maybe also cursors
}

void cpr_drawtexture() {
    extern u8 cursor_init_dl[], tex_obj[];

    extern uObjTxtrBlock_t texParms;

    uObjTxtrBlock_t *parm = segmented_to_virtual(&texParms);

    parm->image = cpr_Texture;
    parm->flag = cpr_Texture;


    gDPPipeSync(gDisplayListHead++);
    gSPDisplayList(gDisplayListHead++, cursor_init_dl);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SPRITE, G_RM_XLU_SPRITE2);
    gSPObjRenderMode(gDisplayListHead++, G_OBJRM_XLU | G_OBJRM_BILERP);
    gSPObjLoadTxtr(gDisplayListHead++, &texParms);
    gSPObjSprite(gDisplayListHead++, &tex_obj);
}

void cpr_point(u16 on, u16 x, u16 y) {
    u16 (*toDraw)[64] = cpr_Texture;
    #define PIXEL (((32 - y) * 64) + x)
    // #define PIXEL ((x * 32) + (64 - y))

    if (on) {
        toDraw[32 - y][x] = 0x0001;
    } else {
        toDraw[32 - y][x] = 0xFFFF;
    }
}

void cpr_drawcursor() {
    static s16 curX = 32;
    static s16 curY = 16;
    static uObjMtx *posMtx;
    posMtx = segmented_to_virtual(&cursor_mtx);

    static uObjSprite *posSprite;
    posSprite = segmented_to_virtual(&cursor_obj);

    if (gPlayer1Controller->stickX < -14) {
        curX--;
        if (curX < 0) curX = 0;
    }
    if (gPlayer1Controller->stickX > 14) {
        curX++;
        if (curX > 63) curX = 63;
    }
    if (gPlayer1Controller->stickY < -14) {
        curY--;
        if (curY < 1) curY = 1;
    }
    if (gPlayer1Controller->stickY > 14) {
        curY++;
        if (curY > 32) curY = 32;
    }

    if (gPlayer1Controller->buttonDown & A_BUTTON) {
        cpr_point(TRUE, curX, curY);
    }
    if (gPlayer1Controller->buttonDown & B_BUTTON) {
        cpr_point(FALSE, curX, curY);
    }

    posSprite->s.objX = qs102((16 + 70 + (curX * 3)));
    posSprite->s.objY = qs102(240 - (100 + (curY * 3)));

    gSPDisplayList(gDisplayListHead++, cursor_sprite_dl);
}


int cpr_minigame() {
    cpr_drawscreen();
    cpr_drawsprites();
    cpr_drawtexture();
    cpr_drawcursor();

    cpr_Timer++;
    if (cpr_NextState != cpr_GameState) {
        cpr_GameState = cpr_NextState;
        cpr_Timer = 0;
    }

    if (gPlayer1Controller->buttonPressed & L_CBUTTONS) {
        if (cpr_filegone()) {
            cpr_makefile();
        }
        if (cpr_filesize() == 0) {
            cpr_deletefile();
            cpr_makefile();   
        }
        cpr_write();
        cpr_close();
        return 1;
    }

    return 0;
}

