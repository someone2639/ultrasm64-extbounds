#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "actors/group8.h"
#include "behavior_commands.h"

const BehaviorScript bhvBetaTrampolineTop[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(springboard_collision_05001A28),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_beta_trampoline_top_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
