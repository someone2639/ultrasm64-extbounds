#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/rr/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvSwingPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(rr_seg7_collision_pendulum),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oCollisionDistance, 2000),
    CALL_NATIVE(bhv_swing_platform_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_swing_platform_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
