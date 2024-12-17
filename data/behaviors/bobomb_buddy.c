#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvBobombBuddy[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oBobombBuddyRole, 0),
    LOAD_ANIMATIONS(oAnimations, bobomb_seg8_anims_0802396C),
    SET_INTERACT_TYPE(INTERACT_TEXT),
    DROP_TO_FLOOR(),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 60),
    ANIMATE(BOBOMB_ANIM_WALKING),
    SET_HOME(),
    CALL_NATIVE(bhv_bobomb_buddy_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_bobomb_buddy_loop),
    END_LOOP(),
};
