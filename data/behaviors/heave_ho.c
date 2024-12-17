#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group1.h"
#include "behavior_commands.h"

const BehaviorScript bhvHeaveHo[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, heave_ho_seg5_anims_0501534C),
    ANIMATE(HEAVE_HO_ANIM_MOVING),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 200, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 600, /*Unused*/ 0, 0),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvHeaveHoThrowMario),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_INT(oInteractionSubtype, (INT_SUBTYPE_NOT_GRABBABLE | INT_SUBTYPE_GRABS_MARIO)),
    SET_HITBOX(/*Radius*/ 120, /*Height*/ 100),
    SET_HOME(),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_heave_ho_loop),
    END_LOOP(),
};
