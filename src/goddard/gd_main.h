#ifndef GD_MAIN_H
#define GD_MAIN_H

#include <PR/ultratypes.h>

#include "gd_types.h"

// In various files of the Goddard subsystem, there are miscellaneous
// unused rodata strings. These are likely byproducts of a printf macro
// that was stubbed out as "#define printf", letting printf calls expand
// to no-op comma expressions. (IDO doesn't support variadic macros, so
// "#define printf(...) /* nothing */" wasn't an option.)
// This macro is separate from the gd_printf function; one probably
// forwarded to the other, but it is hard to tell in which direction.

#define printf(...)                                       \
    _Pragma ("GCC diagnostic push")                       \
    _Pragma ("GCC diagnostic ignored \"-Wunused-value\"") \
    (__VA_ARGS__);                                        \
    _Pragma ("GCC diagnostic pop")

// structs
struct GdControl { // gdControllerInfo
    s32 unk00;  // set but never used
    s32 dleft; // Dpad-left (mask)
    s32 dright; // Dpad-right (mask)
    s32 dup; // Dpad-up (mask)
    s32 ddown; // Dpad-down (mask)
    s32 cleft; // bool C-left
    s32 cright; // bool C-right
    s32 cup; // bool C-up
    s32 cdown; // bool C-down
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
