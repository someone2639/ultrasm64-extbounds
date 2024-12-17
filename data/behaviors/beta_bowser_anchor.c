#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBetaBowserAnchor[] = {
    BEGIN(OBJ_LIST_DESTRUCTIVE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_HOME(),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 300),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_beta_bowser_anchor_loop),
    END_LOOP(),
};
