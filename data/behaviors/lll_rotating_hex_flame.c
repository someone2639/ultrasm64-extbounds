#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvLllRotatingHexFlame[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 100, /*Downwards offset*/ 50),
    SET_INT(oIntangibleTimer, 0),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_rotating_hex_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};
