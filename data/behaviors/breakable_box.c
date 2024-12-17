#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvBreakableBox[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_LONG(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_DONT_CALC_COLL_DIST)),
    LOAD_COLLISION_DATA(breakable_box_seg8_collision),
    SET_FLOAT(oCollisionDistance, 1000),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_breakable_box_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
    BREAK(),
};
