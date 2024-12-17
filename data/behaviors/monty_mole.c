#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group6.h"
#include "behavior_commands.h"

const BehaviorScript bhvMontyMole[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, monty_mole_seg5_anims_05007248),
    ANIMATE(MONTY_MOLE_ANIM_BEGIN_JUMP_INTO_HOLE),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    HIDE(),
    SET_INT(oIntangibleTimer, -1),
    SET_FLOAT(oGraphYOffset, -60),
    SCALE(/*Unused*/ 0, /*Field*/ 150),
    DELAY(1),
    CALL_NATIVE(bhv_monty_mole_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_monty_mole_update),
    END_LOOP(),
};
