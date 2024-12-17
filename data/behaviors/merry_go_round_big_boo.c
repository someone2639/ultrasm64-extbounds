#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvMerryGoRoundBigBoo[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    SET_INT(oBehParams2ndByte, BIG_BOO_BP_MERRY_GO_ROUND),
    // Set number of minion boos killed to 10, which is greater than 5 so that the boo always loads without needing to kill any boos.
    SET_INT(oBigBooNumMinionBoosKilled, 10),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_boo_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_big_boo_loop),
    END_LOOP(),
};
