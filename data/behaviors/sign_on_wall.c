#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvSignOnWall[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_TEXT),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_SIGN),
    SET_HITBOX(/*Radius*/ 150, /*Height*/ 80),
    SET_INT(oWoodenPostTotalMarioAngle, 0),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        SET_INT(oInteractStatus, INT_STATUS_NONE),
    END_LOOP(),
};
