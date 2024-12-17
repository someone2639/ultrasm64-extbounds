#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvBobomb[] = {
    BEGIN(OBJ_LIST_DESTRUCTIVE),
    OR_INT(oFlags, (OBJ_FLAG_PERSISTENT_RESPAWN | OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, bobomb_seg8_anims_0802396C),
    DROP_TO_FLOOR(),
    ANIMATE(BOBOMB_ANIM_WALKING),
    SET_INT(oIntangibleTimer, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_bobomb_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bobomb_loop),
    END_LOOP(),
};
