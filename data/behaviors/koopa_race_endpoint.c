#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvKoopaRaceEndpoint[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    SPAWN_CHILD_WITH_PARAM(/*Bhv param*/ 0, /*Model*/ MODEL_KOOPA_FLAG, /*Behavior*/ bhvKoopaFlag),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_koopa_race_endpoint_update),
    END_LOOP(),
};
