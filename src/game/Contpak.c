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

extern void *castle_grounds_seg7_texture_07002000;

void *cpr_textureArray[LEVEL_COUNT] = {
    [LEVEL_CASTLE_GROUNDS] = &castle_grounds_seg7_texture_07002000,
};



void cpr_copytex(u32 *p) {
    // extern u32 castle_grounds_seg7_dl_0700B9E8;
    // u32 *p = segmented_to_virtual(&castle_grounds_seg7_dl_0700B9E8);
    // p[1] = ((u32)cpr_Texture) & 0x00FFFFFF;

    void *dst = segmented_to_virtual(p);
    memcpy(dst, cpr_Texture, sizeof(cpr_Texture));
}
void cpr_apply() {

    if (cpr_textureArray[gCurrLevelNum] != 0) {
        cpr_copytex(cpr_textureArray[gCurrLevelNum]);
    }
}