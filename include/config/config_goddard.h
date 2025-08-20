#pragma once

/********************
 * GODDARD SETTINGS *
 ********************/

/**
 * Enables Goddard (the Mario head).
 */
#define KEEP_MARIO_HEAD

/**
 * Enables the Goddard easter egg from Shindou (has no effect if KEEP_MARIO_HEAD is disabled).
 */
#define GODDARD_EASTER_EGG

/**
 * Disables the demo that plays when idle on the start screen (has no effect if KEEP_MARIO_HEAD is disabled).
 */
#define DISABLE_DEMO

/**************************
 * Configure Face Settings
 **************************/

#define GODDARD_TOTAL_HEAP_SIZE 0x5000000

#define GODDARD_MAIN_FACE_SCENE dynlist_mario

// default 1900
#define GODDARD_STATIC_DL_CMD_COUNT 5000

// default 600
#define GODDARD_DYNAMIC_DL_CMD_COUNT 5000

// default 100
#define GODDARD_FACE_DL_CMD_COUNT 5000
