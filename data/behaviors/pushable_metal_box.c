#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvPushableMetalBox[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(metal_box_seg8_collision_08024C28),
    SET_FLOAT(oCollisionDistance, 500),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_pushable_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
