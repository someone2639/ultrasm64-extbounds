#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBetaMovingFlamesSpawn[] = {
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_beta_moving_flames_spawn_loop),
    END_LOOP(),
};
