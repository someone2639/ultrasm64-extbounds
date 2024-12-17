#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/ttc/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvTTCElevator[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_clock_platform),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 400),
    CALL_NATIVE(bhv_ttc_elevator_init),
    SET_FLOAT(oTTCElevatorDir, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_elevator_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
