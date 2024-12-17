#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group7.h"
#include "behavior_commands.h"

const BehaviorScript bhvRacingPenguin[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, penguin_seg5_anims_05008B74),
    ANIMATE(PENGUIN_ANIM_IDLE),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 200, /*Gravity*/ -800, /*Bounciness*/ -5, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    SCALE(/*Unused*/ 0, /*Field*/ 400),
    CALL_NATIVE(bhv_racing_penguin_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_racing_penguin_update),
    END_LOOP(),
};
