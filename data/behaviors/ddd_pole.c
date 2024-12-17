#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvDDDPole[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    SET_INTERACT_TYPE(INTERACT_POLE),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 800),
    SET_INT(oIntangibleTimer, 0),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_HOME(),
    CALL_NATIVE(bhv_ddd_pole_init),
    SET_FLOAT(oDDDPoleVel, 10),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ddd_pole_update),
        CALL_NATIVE(bhv_pole_base_loop),
    END_LOOP(),
};
