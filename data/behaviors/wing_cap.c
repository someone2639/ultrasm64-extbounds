#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvWingCap[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_LONG(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_SILHOUETTE)),
    CALL_NATIVE(bhv_wing_cap_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wing_vanish_cap_loop),
    END_LOOP(),
};
