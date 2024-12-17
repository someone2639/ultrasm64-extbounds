#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/ttm/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvTtmRollingLog[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(ttm_seg7_collision_pitoune_2),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 2000),
    CALL_NATIVE(bhv_ttm_rolling_log_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_rolling_log_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
