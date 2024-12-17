#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group7.h"
#include "behavior_commands.h"

const BehaviorScript bhvMrBlizzard[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, snowman_seg5_anims_0500D118),
    ANIMATE(MR_BLIZZARD_ANIM_SPAWN_SNOWBALL),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_mr_blizzard_init),
    SET_FLOAT(oMrBlizzardScale, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_mr_blizzard_update),
    END_LOOP(),
};
