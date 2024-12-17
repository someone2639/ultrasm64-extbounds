#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvIgloo[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_IGLOO_BARRIER),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 200),
    SET_INT(oIntangibleTimer, 0),
    SET_HOME(),
    BEGIN_LOOP(),
        SET_INT(oInteractStatus, INT_STATUS_NONE),
    END_LOOP(),
};
