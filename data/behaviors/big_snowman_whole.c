#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvBigSnowmanWhole[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oGraphYOffset, 180),
    SET_INTERACT_TYPE(INTERACT_TEXT),
    SET_HITBOX(/*Radius*/ 210, /*Height*/ 550),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
    END_LOOP(),
};
