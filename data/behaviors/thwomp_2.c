#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "actors/group1.h"
#include "behavior_commands.h"

const BehaviorScript bhvThwomp2[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(thwomp_seg5_collision_0500B92C),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    ADD_FLOAT(oPosY, 1),
    SET_HOME(),
    SCALE(/*Unused*/ 0, /*Field*/ 140),
    SET_FLOAT(oDrawingDistance, 4000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_grindel_thwomp_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
