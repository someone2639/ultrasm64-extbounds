#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvFlyGuy[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, flyguy_seg8_anims_08011A64),
    ANIMATE(FLY_GUY_ANIM_FLYING),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 50, /*Gravity*/ 0, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 1000, /*Buoyancy*/ 600, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_init_room),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_TWIRL_BOUNCE),
    SET_FLOAT(oGraphYOffset, 30),
    SCALE(/*Unused*/ 0, /*Field*/ 150),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fly_guy_update),
    END_LOOP(),
};
