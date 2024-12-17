#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/wf/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvLargeBomp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_large_bomp),
    CALL_NATIVE(bhv_large_bomp_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_large_bomp_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
