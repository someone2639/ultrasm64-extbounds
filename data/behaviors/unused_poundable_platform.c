#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/sl/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvUnusedPoundablePlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(sl_seg7_collision_pound_explodes),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_unused_poundable_platform),
    END_LOOP(),
};
