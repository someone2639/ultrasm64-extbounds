#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/hmc/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvControllablePlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_THROW_MATRIX_FROM_TRANSFORM | OBJ_FLAG_UPDATE_TRANSFORM_FOR_THROW_MATRIX | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(hmc_seg7_collision_controllable_platform),
    SET_HOME(),
    CALL_NATIVE(bhv_controllable_platform_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_controllable_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
