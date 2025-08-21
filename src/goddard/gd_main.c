#include <PR/ultratypes.h>

#include "debug_utils.h"
#include "gd_main.h"
#include "gd_memory.h"
#include "macros.h"
#include "objects.h"
#include "renderer.h"

/* This file was spilt out of debug_memory.asm based on rodata.
 * The rodata for functions after this "__main__" function have string literals following
 * f32 literal, which implies that this is its own file
 */

s32 gGdMoveScene = TRUE;    // Whether objects should move at all
s32 gdUseSmoothShading = TRUE; // Set to FALSE to use face normals

struct GdControl gdControllerInfo;
struct GdControl gdControllerInfoPrevFrame;

