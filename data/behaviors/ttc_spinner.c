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

const BehaviorScript bhvTTCSpinner[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_rotating_clock_platform2),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oCollisionDistance, 450),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_spinner_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
