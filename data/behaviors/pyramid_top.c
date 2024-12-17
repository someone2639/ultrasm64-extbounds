#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/ssl/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvPyramidTop[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(ssl_seg7_collision_pyramid_top),
    SET_HOME(),
    SET_FLOAT(oDrawingDistance, 20000),
    SET_FLOAT(oCollisionDistance, 20000),
    CALL_NATIVE(bhv_pyramid_top_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_pyramid_top_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
