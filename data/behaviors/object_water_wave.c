#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvObjectWaterWave[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oFaceAnglePitch, 0),
    SET_INT(oFaceAngleYaw, 0),
    SET_INT(oFaceAngleRoll, 0),
    CALL_NATIVE(bhv_object_water_wave_init),
    DELAY(6),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_object_water_wave_loop),
        ADD_INT(oAnimState, 1),
    BEGIN_REPEAT(6),
        CALL_NATIVE(bhv_object_water_wave_loop),
    END_REPEAT(),
    END_LOOP(),
};
