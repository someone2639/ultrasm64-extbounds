#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvChainChomp[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, chain_chomp_seg6_anims_06025178),
    ANIMATE(CHAIN_CHOMP_ANIM_CHOMPING),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    HIDE(),
    SET_HOME(),
    SET_FLOAT(oGraphYOffset, 240),
    SCALE(/*Unused*/ 0, /*Field*/ 200),
    SPAWN_CHILD_WITH_PARAM(/*Bhv param*/ 0, /*Model*/ MODEL_WOODEN_POST, /*Behavior*/ bhvWoodenPost),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_chain_chomp_update),
    END_LOOP(),
};
