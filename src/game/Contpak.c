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

typedef struct contpak_t {
    void *texture;
    void *palette;
} contpak_t;

// INSTRUCTIONS
//  Step 1: Go to levels/(your level)/model.inc.c and find your texture/palette
//  Step 2: Extern that symbol here and add it to the texture array
//           [LEVEL_XYZ] = {&your_texture, &your_palette},
// IMPORTANT:
//  The Palette symbol will look like 'u16 myPalette[] = ...'
//  MAKE SURE TO SET THE LENGTH TO 16!!

extern void *castle_grounds_seg7_texture_07002000;
extern void *castle_grounds_seg7_texture_07000000;

contpak_t cpr_textureArray_Initial[LEVEL_COUNT] = {
    [LEVEL_CASTLE_GROUNDS] = {
        .texture = &castle_grounds_seg7_texture_07002000,
        .palette = &castle_grounds_seg7_texture_07002000
    },
};
contpak_t cpr_textureArray_Payoff[LEVEL_COUNT] = {
    [LEVEL_CASTLE_GROUNDS] = {
        .texture = &castle_grounds_seg7_texture_07002000,
        .palette = &castle_grounds_seg7_texture_07002000
    },
};

void cpr_copytex(u32 *p) {
    void *dst = segmented_to_virtual(p);
    memcpy(dst, cpr_Texture, 2048);
}
void cpr_copypal(u32 *p) {
    void *dst = segmented_to_virtual(p);
    memcpy(dst, cpr_Texture + 2048, 32);
}

void cpr_apply_payoff() {
    if (cpr_textureArray_Payoff[gCurrLevelNum].texture != 0) {
        cpr_copytex(cpr_textureArray_Payoff[gCurrLevelNum].texture);
        cpr_copypal(cpr_textureArray_Payoff[gCurrLevelNum].palette);
    }
}
void cpr_apply_initial() {
    if (cpr_textureArray_Initial[gCurrLevelNum].texture != 0) {
        cpr_copytex(cpr_textureArray_Initial[gCurrLevelNum].texture);
        cpr_copypal(cpr_textureArray_Initial[gCurrLevelNum].palette);
    }
}
