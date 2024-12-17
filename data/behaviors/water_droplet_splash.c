#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvWaterDropletSplash[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oFaceAnglePitch, 0),
    SET_INT(oFaceAngleYaw, 0),
    SET_INT(oFaceAngleRoll, 0),
    CALL_NATIVE(bhv_water_droplet_splash_init),
    ADD_FLOAT(oPosY, 5),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    BEGIN_REPEAT(6),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};
