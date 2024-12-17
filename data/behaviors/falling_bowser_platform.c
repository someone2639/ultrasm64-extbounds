#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "behavior_commands.h"

const BehaviorScript bhvFallingBowserPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_LONG(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_DONT_CALC_COLL_DIST)),
    SET_FLOAT(oDrawingDistance, 20000),
    SET_FLOAT(oCollisionDistance, 20000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_falling_bowser_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
