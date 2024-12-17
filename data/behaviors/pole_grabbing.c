#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvPoleGrabbing[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oInteractType, INTERACT_POLE),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 1500),
    CALL_NATIVE(bhv_pole_init),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_pole_base_loop),
    END_LOOP(),
};
