#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common1.h"
#include "behavior_commands.h"

const BehaviorScript bhvBlueFish[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    LOAD_ANIMATIONS(oAnimations, blue_fish_seg3_anims_0301C2B0),
    ANIMATE(FISH_ANIM_DEFAULT),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_fish_movement_loop),
    END_LOOP(),
};
