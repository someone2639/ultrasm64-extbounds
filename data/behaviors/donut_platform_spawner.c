#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvDonutPlatformSpawner[] = {
    BEGIN(OBJ_LIST_SPAWNER),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_donut_platform_spawner_update),
    END_LOOP(),
};
