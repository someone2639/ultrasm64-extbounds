#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group5.h"
#include "behavior_commands.h"

const BehaviorScript bhvEyerokHand[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, eyerok_seg5_anims_050116E4),
    ANIMATE(EYEROK_HAND_ANIM_SLEEPING),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 150, /*Gravity*/ 0, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    SET_INT(oAnimState, EYEROK_HAND_ANIM_STATE_EYE_CLOSED),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_eyerok_hand_loop),
    END_LOOP(),
};
