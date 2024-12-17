#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "actors/group7.h"
#include "actors/group7.h"
#include "behavior_commands.h"

const BehaviorScript bhvSLWalkingPenguin[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(penguin_seg5_collision_05008B88),
    LOAD_ANIMATIONS(oAnimations, penguin_seg5_anims_05008B74),
    ANIMATE(PENGUIN_ANIM_WALK),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SCALE(/*Unused*/ 0, /*Field*/ 600),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_sl_walking_penguin_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
