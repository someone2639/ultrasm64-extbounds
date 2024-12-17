#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvSmallWaterWave[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    CALL_NATIVE(bhv_bubble_wave_init),
    SET_RANDOM_FLOAT(oWaterObjScaleXAngle, /*Minimum*/ -50, /*Range*/ 100),
    SET_RANDOM_FLOAT(oWaterObjScaleYAngle, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosX, /*Value 1*/ oPosX, /*Value 2*/ oWaterObjScaleXAngle),
    SUM_FLOAT(/*Dest*/ oPosZ, /*Value 1*/ oPosZ, /*Value 2*/ oWaterObjScaleYAngle),
    SET_RANDOM_FLOAT(oWaterObjScaleXAngleVel, /*Minimum*/ 0, /*Range*/ 50),
    SUM_FLOAT(/*Dest*/ oPosY, /*Value 1*/ oPosY, /*Value 2*/ oWaterObjScaleXAngleVel),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    CALL_NATIVE(bhvSmallWaterWave398),
    BEGIN_REPEAT(60),
        CALL_NATIVE(bhvSmallWaterWave398),
        CALL_NATIVE(bhv_small_water_wave_loop),
    END_REPEAT(),
    DEACTIVATE(),
};
