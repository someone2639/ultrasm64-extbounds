#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group14.h"
#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvSmallWhomp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    SET_INT(oNumLootCoins, 5),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, whomp_seg6_anims_06020A04),
    LOAD_COLLISION_DATA(whomp_seg6_collision_06020A0C),
    ANIMATE(WHOMP_ANIM_WALK),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_whomp_loop),
    END_LOOP(),
};
