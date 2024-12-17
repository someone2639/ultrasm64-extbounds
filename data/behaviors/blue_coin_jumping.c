#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBlueCoinJumping[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    CALL_NATIVE(bhv_blue_coin_sliding_jumping_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_coin_jumping_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};
