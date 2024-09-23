#pragma once
#include <PR/os_cont.h>

/* Demo Macros */

.macro set_rng seed
    .byte 0, 0
    .half \seed
.endm

.macro stick x, y
    .byte \x, \y
.endm
.macro yaw y
    .half \y
.endm

.macro end_demo
    .word 0, 0, 0
.endm

.macro for holdcount_frames
    .byte \holdcount_frames
.endm

/* purely for legibility */
#define frames

.macro mag m
    .float \m
.endm

.macro press b
    .half \b
.endm

/* Pretty names since pressing every button on a frame might overrun a buffer*/
#define A A_BUTTON
#define B B_BUTTON
#define Z Z_TRIG
#define Start START_BUTTON
#define R R_TRIG

#define C_Up U_CBUTTONS
#define C_Down D_CBUTTONS
#define C_Left L_CBUTTONS
#define C_Right R_CBUTTONS

/* Macro for no button */
#define _ 0
