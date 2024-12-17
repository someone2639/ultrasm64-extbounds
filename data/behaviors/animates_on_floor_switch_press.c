#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "behavior_commands.h"

const BehaviorScript bhvAnimatesOnFloorSwitchPress[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oDrawingDistance, 8000),
    SET_FLOAT(oCollisionDistance, 8000),
    CALL_NATIVE(bhv_animates_on_floor_switch_press_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_animates_on_floor_switch_press_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
