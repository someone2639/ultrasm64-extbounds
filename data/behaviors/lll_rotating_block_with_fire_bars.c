#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/lll/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvLllRotatingBlockWithFireBars[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_rotating_fire_bars),
    SET_FLOAT(oCollisionDistance, 4000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_rotating_block_fire_bars_loop),
    END_LOOP(),
};
