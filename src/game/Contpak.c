#include <ultra64.h>
#include <PR/gs2dex.h>
#include "game_init.h"
#include "engine/math_util.h"
#include "contpakreal.h"
#include "level_table.h"
#include "object_list_processor.h"
#include "main.h"
#include "debug.h"
#include "game_init.h"

// INSTRUCTIONS
//  Step 1: Go to levels/(your level)/model.inc.c and find your texture
//  Step 2: Extern that symbol here and add it to the texture array
//           [LEVEL_XYZ] = &your_texture,

extern void *castle_grounds_seg7_texture_07002000;

void *cpr_textureArray[LEVEL_COUNT] = {
    [LEVEL_CASTLE_GROUNDS] = &castle_grounds_seg7_texture_07002000,
};







void cpr_copytex(u32 *p) {
    void *dst = segmented_to_virtual(p);
    memcpy(dst, cpr_Texture, sizeof(cpr_Texture));
}
void cpr_apply() {
    if (cpr_textureArray[gCurrLevelNum] != 0) {
        cpr_copytex(cpr_textureArray[gCurrLevelNum]);
    }
}