#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBowserTailAnchor[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 100, /*Height*/ 50, /*Downwards offset*/ -50),
    SET_INT(oIntangibleTimer, 0),
    DISABLE_RENDERING(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_tail_anchor_loop),
    END_LOOP(),
};
