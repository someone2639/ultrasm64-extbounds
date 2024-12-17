#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvFirePiranhaPlant[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, piranha_plant_seg6_anims_0601C31C),
    ANIMATE(FIRE_PIRANHA_PLANT_ANIM_SHRINK),
    SET_HOME(),
    HIDE(),
    CALL_NATIVE(bhv_fire_piranha_plant_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fire_piranha_plant_update),
    END_LOOP(),
};
