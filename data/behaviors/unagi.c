#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group4.h"
#include "behavior_commands.h"

const BehaviorScript bhvUnagi[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, unagi_seg5_anims_05012824),
    ANIMATE(UNAGI_ANIM_IDLE_2),
    SET_HOME(),
    SCALE(/*Unused*/ 0, /*Field*/ 300),
    SET_FLOAT(oDrawingDistance, 6000),
    CALL_NATIVE(bhv_unagi_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_unagi_loop),
    END_LOOP(),
};
