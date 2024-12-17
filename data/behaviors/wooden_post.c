#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvWoodenPost[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(poundable_pole_collision_wooden_post),
    OR_LONG(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_DONT_CALC_COLL_DIST)),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_INT(oNumLootCoins, 5),
    DROP_TO_FLOOR(),
    SET_HOME(),
    SCALE(/*Unused*/ 0, /*Field*/ 50),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wooden_post_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
