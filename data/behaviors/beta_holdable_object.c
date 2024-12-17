#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvBetaHoldableObject[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_HOLDABLE | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    DROP_TO_FLOOR(),
    SET_HITBOX(/*Radius*/ 40, /*Height*/ 50),
    CALL_NATIVE(bhv_beta_holdable_object_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_beta_holdable_object_loop),
    END_LOOP(),
};
