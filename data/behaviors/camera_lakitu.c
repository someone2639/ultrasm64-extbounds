#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group15.h"
#include "behavior_commands.h"

const BehaviorScript bhvCameraLakitu[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, lakitu_seg6_anims_060058F8),
    ANIMATE(CAMERA_LAKITU_ANIM_DEFAULT),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_camera_lakitu_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_camera_lakitu_update),
    END_LOOP(),
};
