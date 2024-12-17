#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group5.h"
#include "behavior_commands.h"

const BehaviorScript bhvKlepto[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, klepto_seg5_anims_05008CFC),
    ANIMATE(KLEPTO_ANIM_DIVE_0),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 100, /*Gravity*/ 0, /*Bounciness*/ -20, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_klepto_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_klepto_update),
    END_LOOP(),
};
