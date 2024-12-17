#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group9.h"
#include "behavior_commands.h"

const BehaviorScript bhvHauntedChair[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, chair_seg5_anims_05005784),
    ANIMATE(HAUNTED_CHAIR_ANIM_DEFAULT),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 40, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_haunted_chair_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_haunted_chair_loop),
    END_LOOP(),
};
