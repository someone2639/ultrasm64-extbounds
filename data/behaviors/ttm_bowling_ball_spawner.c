#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvTtmBowlingBallSpawner[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oBBallSpawnerPeriodMinus1, 63),
    CALL_NATIVE(bhv_generic_bowling_ball_spawner_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_generic_bowling_ball_spawner_loop),
    END_LOOP(),
};
