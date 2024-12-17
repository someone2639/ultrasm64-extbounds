#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "behavior_commands.h"

const BehaviorScript bhvFerrisWheelAxle[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    ADD_INT(oMoveAngleYaw, 0x4000),
    CALL_NATIVE(bhv_ferris_wheel_axle_init),
    BEGIN_LOOP(),
        ADD_INT(oFaceAngleRoll, 400),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
