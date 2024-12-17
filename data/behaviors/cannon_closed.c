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

const BehaviorScript bhvCannonClosed[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_PERSISTENT_RESPAWN | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(cannon_lid_seg8_collision_08004950),
    SET_HOME(),
    CALL_NATIVE(bhv_cannon_closed_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_cannon_closed_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
