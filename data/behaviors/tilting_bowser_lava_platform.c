#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/bowser_2/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvTiltingBowserLavaPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_LONG(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_DONT_CALC_COLL_DIST)),
    LOAD_COLLISION_DATA(bowser_2_seg7_collision_tilting_platform),
    SET_FLOAT(oDrawingDistance, 20000),
    SET_FLOAT(oCollisionDistance, 20000),
    SET_INT(oFaceAngleYaw, 0x0),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(cur_obj_rotate_face_angle_using_vel),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
