#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvCheckerboardPlatformSub[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(checkerboard_platform_seg8_collision_platform),
    CALL_NATIVE(bhv_checkerboard_platform_init),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_checkerboard_platform_loop),
    END_LOOP(),
};
