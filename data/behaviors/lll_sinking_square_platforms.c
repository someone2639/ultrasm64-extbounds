#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/lll/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvLllSinkingSquarePlatforms[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_sinking_pyramids),
    ADD_FLOAT(oPosY, 5),
    SET_FLOAT(oCollisionDistance, 2000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_sinking_square_platforms_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
