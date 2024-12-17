#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group9.h"
#include "behavior_commands.h"

const BehaviorScript bhvFlyingBookend[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, bookend_seg5_anims_05002540),
    ANIMATE(FLYING_BOOKEND_ANIM_SPAWN),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 60, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_INT(oMoveFlags, OBJ_MOVE_NONE),
    SCALE(/*Unused*/ 0, /*Field*/ 70),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flying_bookend_loop),
    END_LOOP(),
};
