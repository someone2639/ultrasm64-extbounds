#include <ultra64.h>
#include <PR/gs2dex.h>
#include "game_init.h"
#include "engine/math_util.h"
#include "contpakreal.h"

#include "troll_sprites/bg.h"
#include "troll_sprites/cursor.h"
#include "troll_sprites/painttool.h"
#include "troll_sprites/linetool.h"
#include "troll_sprites/airbrush.h"
#include "troll_sprites/toolcur.h"

#define cpr_CYC G_CYC_1CYCLE

#if cpr_CYC == G_CYC_1CYCLE
#define cpr_dsdx 1
#else
#define cpr_dsdx 4
#endif

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

Bound drawBoxBounds = {
    .ulx = 0, .uly = 32,
    .lrx = 63, .lry = 1,
};

void BB_constrain(Bound *b, s16 *x, s16 *y) {
    if (*x < b->ulx) *x = b->ulx;
    if (*x > b->lrx) *x = b->lrx;
    if (*y < b->lry) *y = b->lry;
    if (*y > b->uly) *y = b->uly;
}


typedef enum CPR_DRAWTOOL {
    CPR_MINTOOL = 0,
    CPR_POINT,
    CPR_LINE,
    CPR_AIRBRUSH,
    // CPR_FILL,
    CPR_MAXTOOL,
} DrawTool;

DrawTool cpr_Tool = CPR_POINT;
u16 cpr_ColorOn = 0x0001;

void cpr_drawscreen() {
    // gSPDisplayList(gDisplayListHead++, bg_bg_dl);
    render_multi_image(bg_tex_0, 0, 0, 320, 240, 1, 1, cpr_CYC);
}

enum colors {
    CPR_MINCOL,
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_TEAL,
    COLOR_PURBLE,
    COLOR_BLUE,
    COLOR_YELLOW,
    CPR_MAXCOL,
};
static enum colors cpr_colorIdx = COLOR_BLACK;

u16 cpr_colorArray[CPR_MAXCOL] = {
    [COLOR_BLACK] = GPACK_RGBA5551(0, 0, 0, 255),
    [COLOR_RED] = GPACK_RGBA5551(247, 70, 57, 255),
    [COLOR_GREEN] = GPACK_RGBA5551(173, 209, 10, 255),
    [COLOR_TEAL] = GPACK_RGBA5551(33, 209, 192, 255),
    [COLOR_PURBLE] = GPACK_RGBA5551(167, 66, 235, 255),
    [COLOR_BLUE] = GPACK_RGBA5551(42, 42, 247, 255),
    [COLOR_YELLOW] = GPACK_RGBA5551(0xFF,0xFF,00, 255),
};

void cpr_drawsprites() {
    // for paint and colors
    // maybe also cursors
    render_multi_image(painttool_tex_0, 32, 69, 32, 32, 1, 1, cpr_CYC);
    render_multi_image(linetool_tex_0, 32, 100, 32, 32, 1, 1, cpr_CYC);
    render_multi_image(airbrush_tex_0, 32, 132, 32, 32, 1, 1, cpr_CYC);
    // gSPDisplayList(gDisplayListHead++, painttool_sprite_dl);
    // gSPDisplayList(gDisplayListHead++, linetool_sprite_dl);
    // gSPDisplayList(gDisplayListHead++, airbrush_sprite_dl);

    render_multi_image(toolcursor_tex_0, 32, 70 + (32 * (cpr_Tool - 1)), 32, 32, 1, 1, cpr_CYC);

    // uObjMtx *mm = segmented_to_virtual(&toolcursor_mtx);
    // mm->m.Y = qs102(70 + (32 * (cpr_Tool - 1)));
    // gSPDisplayList(gDisplayListHead++, toolcursor_sprite_dl);
}

void cpr_drawcolors() {
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetRenderMode(gDisplayListHead++, G_RM_NOOP, G_RM_NOOP2);
    for (int i = 1; i < CPR_MAXCOL; i++) {
        gDPPipeSync(gDisplayListHead++);
        gDPSetFillColor(gDisplayListHead++, (cpr_colorArray[i] << 16) | cpr_colorArray[i]);
        gDPFillRectangle(gDisplayListHead++, 
            (i * 32) + 5, 5 + 180,
            (i * 32) + 27, 180 + 27
        );
    }

    extern u8 colorcursor_sprite_dl[], colorcursor_mtx[];

    render_multi_image(
        toolcursor_tex_0,
        32 + (cpr_colorIdx * 32),
        180,
        32, 32,
        1, 1,
        cpr_CYC
    );

    // uObjMtx *m = segmented_to_virtual(colorcursor_mtx);
    // m->m.X = qs102((cpr_colorIdx * 32) + 0.5f);
    // m->m.Y = qs102(180 + 0.5f);

    // gSPDisplayList(gDisplayListHead++, colorcursor_sprite_dl);
}

void cpr_drawtexture() {
    extern u8 cursor_init_dl[], tex_obj[], tex_mtx[];

    extern uObjTxtrBlock_t texParms;

    uObjTxtrBlock_t *parm = segmented_to_virtual(&texParms);

    parm->image = cpr_Texture;
    parm->flag = cpr_Texture;

    extern u8 dl_hud_img_load_tex_block[];

    u16 x = 100;
    u16 y = 70;

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, cpr_CYC);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, cpr_Texture);
    gSPDisplayList(gDisplayListHead++, &dl_hud_img_load_tex_block);
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 63) << 2, (y + 31) << 2,
                        G_TX_RENDERTILE, 0, 0, cpr_dsdx << 10, 1 << 10);
    // gSPDisplayList(gDisplayListHead++, cursor_init_dl);
    // gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    // gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SPRITE, G_RM_XLU_SPRITE2);
    // gSPObjRenderMode(gDisplayListHead++, G_OBJRM_XLU | G_OBJRM_BILERP);
    // gSPObjLoadTxtr(gDisplayListHead++, &texParms);
    // gSPObjMatrix(gDisplayListHead++, &tex_mtx);
    // gSPObjSprite(gDisplayListHead++, &tex_obj);
}

void cpr_point(u16 on, u16 x, u16 y) {
    u16 (*toDraw)[64] = cpr_Texture;
    #define PIXEL (((32 - y) * 64) + x)
    // #define PIXEL ((x * 32) + (64 - y))

    if (on) {
        toDraw[32 - y][x] = cpr_ColorOn;
    } else {
        toDraw[32 - y][x] = 0xFFFF;
    }
}

void cpr_line(u16 on, u16 x, u16 y) {
    u16 (*toDraw)[64] = cpr_Texture;
    #define PIXEL (((32 - y) * 64) + x)
    #define LEFTPAD(c) ((c) < 0 ? 0 : (c))
    #define RIGHTPAD(c) ((c) > 63 ? 63 : (c))

    s16 xPos[5];
        xPos[0] = LEFTPAD(x - 2);
        xPos[1] = LEFTPAD(x - 1);
        xPos[2] = x;
        xPos[3] = RIGHTPAD(x + 1);
        xPos[4] = RIGHTPAD(x + 2);


    for (int i = 0; i < 5; i++) {
        if (on) {
            toDraw[32 - y][xPos[i]] = cpr_ColorOn;
        } else {
            toDraw[32 - y][xPos[i]] = 0xFFFF;
        }
    }
}

void cpr_airbrush(u16 on, u16 x, u16 y) {
    u16 (*toDraw)[64] = cpr_Texture;
    #define LEFTPAD(c) ((c) < 0 ? 0 : (c))
    #define RIGHTPAD(c) ((c) > 63 ? 63 : (c))
    #define DOWNPAD(c) ((c) < 1 ? 1 : (c))
    #define UP_PAD(c) ((c) > 32 ? 32 : (c))

    #define AIRBRUSH_RADIUS 5

    for (int i = 0; i < 5; i++) {
        s16 newY = (32 - y);
        s16 newX = x;

        if (random_u16() & 1) {
            newY = DOWNPAD(newY - random_float() * AIRBRUSH_RADIUS);
        } else {
            newY = UP_PAD(newY + random_float() * AIRBRUSH_RADIUS);
        }

        if (random_u16() & 1) {
            newX = LEFTPAD(newX - random_float() * AIRBRUSH_RADIUS);
        } else {
            newX = RIGHTPAD(newX + random_float() * AIRBRUSH_RADIUS);
        }


        if (on) {
            toDraw[newY][newX] = cpr_ColorOn;
        } else {
            toDraw[newY][newX] = 0xFFFF;
        }
    }
}

void cpr_fill(u16 startColor, u16 on, s16 x, s16 y) {
    u16 (*toDraw)[64] = cpr_Texture;
    // trivial cases
    if (x < 0) return;
    if (y < 1) return;
    if (x > 63) return;
    if (y < 32) return;
    // color cases
    if (toDraw[32 - y][x] != startColor) return;

    if (on) {
        toDraw[32 - y][x] = cpr_ColorOn;
    } else {
        toDraw[32 - y][x] = 0xFFFF;
    }

    cpr_fill(startColor, on, x - 1, y);
    cpr_fill(startColor, on, x + 1, y);
    cpr_fill(startColor, on, x, y - 1);
    cpr_fill(startColor, on, x, y + 1);
}

void cpr_draw(u16 on, u16 x, u16 y) {
    switch (cpr_Tool) {
        case CPR_POINT: cpr_point(on, x, y); break;
        case CPR_LINE: cpr_line(on, x, y); break;
        case CPR_AIRBRUSH: cpr_airbrush(on, x, y); break;
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
    }
    if (gPlayer1Controller->stickX > 14) {
        curX++;
    }
    if (gPlayer1Controller->stickY < -14) {
        curY--;
    }
    if (gPlayer1Controller->stickY > 14) {
        curY++;
    }
    BB_constrain(&drawBoxBounds, &curX, &curY);

    if (gPlayer1Controller->buttonDown & A_BUTTON) {
        cpr_draw(TRUE, curX, curY);
    }
    if (gPlayer1Controller->buttonDown & B_BUTTON) {
        cpr_draw(FALSE, curX, curY);
    }

    // u16 (*toDraw)[64] = cpr_Texture;
    // u16 currColor = toDraw[32 - curY][curX];

    // if (gPlayer1Controller->buttonPressed & A_BUTTON) {
    //     cpr_fill(currColor, TRUE, curX, curY);
    // }
    // if (gPlayer1Controller->buttonPressed & B_BUTTON) {
    //     cpr_fill(currColor, FALSE, curX, curY);
    // }

    posMtx->m.X = qs102((70 + 44 + (curX * 3)));
    posMtx->m.Y = qs102(240 - (65 + (curY * 3)));

    // gSPDisplayList(gDisplayListHead++, cursor_sprite_dl);
    render_multi_image(
        cursor_tex_0,
        (70 + 44 + (curX * 3)),
        240 - (65 + (curY * 3)),
        32, 32,
        1, 1,
        cpr_CYC
    );
}

void cpr_updatetool() {
    if (gPlayer1Controller->buttonPressed & U_JPAD) cpr_Tool--;
    if (gPlayer1Controller->buttonPressed & D_JPAD) cpr_Tool++;

    if (cpr_Tool <= CPR_MINTOOL) cpr_Tool = CPR_MINTOOL + 1;
    if (cpr_Tool >= CPR_MAXTOOL) cpr_Tool = CPR_MAXTOOL - 1;
}
void cpr_updatecolor() {

    if (gPlayer1Controller->buttonPressed & L_JPAD) cpr_colorIdx--;
    if (gPlayer1Controller->buttonPressed & R_JPAD) cpr_colorIdx++;

    if (cpr_colorIdx <= CPR_MINCOL) cpr_colorIdx = CPR_MINCOL + 1;
    if (cpr_colorIdx >= CPR_MAXCOL) cpr_colorIdx = CPR_MAXCOL - 1;


    cpr_ColorOn = cpr_colorArray[cpr_colorIdx];
}

int cpr_minigame() {
    cpr_drawscreen();
    cpr_drawsprites();
    cpr_drawtexture();
    cpr_drawcursor();

    cpr_drawcolors();

    cpr_updatetool();
    cpr_updatecolor();

    if (gPlayer1Controller->buttonPressed & START_BUTTON) {
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

