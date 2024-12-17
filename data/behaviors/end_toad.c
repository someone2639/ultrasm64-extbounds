#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group15.h"
#include "behavior_commands.h"

const BehaviorScript bhvEndToad[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_ANIMATIONS(oAnimations, toad_seg6_anims_0600FB58),
    ANIMATE(TOAD_ANIM_WEST_WAVE_THEN_TURN),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_end_toad_loop),
    END_LOOP(),
};
