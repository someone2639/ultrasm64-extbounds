#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvGrandStar[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_STAR_OR_KEY),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_GRAND_STAR),
    SET_HITBOX(/*Radius*/ 160, /*Height*/ 100),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_grand_star_loop),
    END_LOOP(),
};
