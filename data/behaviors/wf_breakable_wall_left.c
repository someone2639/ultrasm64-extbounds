#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/wf/header.h"
#include "levels/wf/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvWfBreakableWallLeft[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(wf_seg7_collision_breakable_wall_2),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HITBOX(/*Radius*/ 300, /*Height*/ 400),
    SET_FLOAT(oDrawingDistance, 4500),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wf_breakable_wall_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
