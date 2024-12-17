#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/hmc/header.h"
#include "levels/hmc/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvControllablePlatformSub[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(hmc_seg7_collision_controllable_platform_sub),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_controllable_platform_sub_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
