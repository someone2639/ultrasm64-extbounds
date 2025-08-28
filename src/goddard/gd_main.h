#ifndef GD_MAIN_H
#define GD_MAIN_H

#include <PR/ultratypes.h>

#include "gd_types.h"

#define printf(...)                                       \
    osSyncPrintf(__VA_ARGS__)

// structs
struct GdControl { // gdControllerInfo
    u8 dleft        : 1;
    u8 dright       : 1;
    u8 dup          : 1;
    u8 ddown        : 1;
    u8 cleft        : 1;
    u8 cright       : 1;
    u8 cup          : 1;
    u8 cdown        : 1;

    void * unk28;     // null-checked ptr? symbol not deref-ed in extant code?
    void * unk2C;     // some sort of old texture ptr? symbol not deref-ed in extant code?
    void * unk30;     // null-checked ptr? symbol not deref-ed in extant code?

    s32 btnA; // bool A button
    s32 btnB; // bool B button
    s32 trgL; // bool L trigger pressed
    s32 trgR; // bool R trigger pressed
    s32 unk4C;
    s32 unk50;
    s32 newStartPress; // toggle bit? start pressed?
    f32 stickXf;
    f32 stickYf;
    f32 unk88;  // set but never used
    f32 unkA0;  // set but never used
    f32 unkAC;
    s32 dragStartX; // cursor x position when there was a new (A) press?
    s32 dragStartY; // cursor y position when there was a new (A) press?
    s32 stickDeltaX;
    s32 stickDeltaY;
    s32 stickX;
    s32 stickY;
    s32 cursorX; // bounded by screen view
    s32 cursorY; // bounded by screen view
    /* hand/cursor state bitfield? */
    u8 dragging;  // bool (A) pressed
    u8 startedDragging;  // bool new (A) press
    u8 AbtnPressWait;  // bool 10 frames between (A) presses (cursor cool down?)
    u32 dragStartFrame; // first frame of new a press
    u32 currFrame; // frame count?
    struct GdControl *prevFrame; // previous frame data
};

// linker-generated symbols
extern u8 _faceDataSegmentRomStart[];
extern u8 _faceDataSegmentRomEnd[];
extern struct DynList GODDARD_MAIN_FACE_SCENE[];

// data
extern s32 gGdMoveScene;
extern s32 gdUseSmoothShading;
extern OSContPadEx sGdContPads[4];

// bss
extern struct GdControl gdControllerInfo;
extern struct GdControl gdControllerInfoPrevFrame;

void gd_copy_p1_contpad(OSContPadEx *p1cont);
s32 gd_sfx_to_play(void);
void gdInit(s16 sceneID);
void gdSceneTick(void);
Gfx *gdDrawScene(s32 sceneID);

#endif // GD_MAIN_H
