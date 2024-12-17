#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group17.h"
#include "behavior_commands.h"

const BehaviorScript bhvSwoop[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, swoop_seg6_anims_060070D0),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 50, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_init_room),
    SCALE(/*Unused*/ 0, /*Field*/ 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_swoop_update),
    END_LOOP(),
};
