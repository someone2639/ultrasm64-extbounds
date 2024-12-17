#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvObjectBubble[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    CALL_NATIVE(bhv_object_bubble_init),
    SET_RANDOM_FLOAT(oVelY, /*Minimum*/ 3, /*Range*/ 6),
    SET_INT_RAND_RSHIFT(oMoveAngleYaw, /*Minimum*/ 0, /*Right shift*/ 0),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_object_bubble_loop),
    END_LOOP(),
};
