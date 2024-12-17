#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvGhostHuntBoo[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oIntangibleTimer, 0),
    SET_HOME(),
    SET_INT(oDamageOrCoinValue, 2),
    SET_HITBOX(/*Radius*/ 140, /*Height*/ 80),
    SET_HURTBOX(/*Radius*/ 40, /*Height*/ 60),
    SET_FLOAT(oGraphYOffset, 30),
    CALL_NATIVE(bhv_init_room),
    SPAWN_CHILD(/*Model*/ MODEL_YELLOW_COIN, /*Behavior*/ bhvCoinInsideBoo),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_boo_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_boo_loop),
    END_LOOP(),
};
