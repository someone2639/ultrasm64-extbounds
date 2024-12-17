#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBbhTumblingBridge[] = {
    BEGIN(OBJ_LIST_SPAWNER),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_INT(oBehParams2ndByte, TUMBLING_BRIDGE_BP_BBH),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tumbling_bridge_loop),
    END_LOOP(),
};
