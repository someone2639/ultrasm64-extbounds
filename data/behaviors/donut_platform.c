#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/rr/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvDonutPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(rr_seg7_collision_donut_platform),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_donut_platform_update),
    END_LOOP(),
};
