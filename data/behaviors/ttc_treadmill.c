#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "behavior_commands.h"

const BehaviorScript bhvTTCTreadmill[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_NO_AUTO_DISPLACEMENT)),
    SET_FLOAT(oCollisionDistance, 750),
    CALL_NATIVE(bhv_ttc_treadmill_init),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_treadmill_update),
        CALL_NATIVE(cur_obj_compute_vel_xz),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
