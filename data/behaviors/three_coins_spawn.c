#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvThreeCoinsSpawn[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(3),
        SPAWN_CHILD(/*Model*/ MODEL_YELLOW_COIN, /*Behavior*/ bhvSingleCoinGetsSpawned),
    END_REPEAT(),
    DEACTIVATE(),
};
