#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvOneCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    SET_INT(oBehParams2ndByte, YELLOW_COIN_BP_ONE_COIN),
    BILLBOARD(),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_yellow_coin_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_yellow_coin_loop),
    END_LOOP(),
};
