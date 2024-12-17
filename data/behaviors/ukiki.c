#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group6.h"
#include "behavior_commands.h"

const BehaviorScript bhvUkiki[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_HOLDABLE_NPC),
    SET_HITBOX(/*Radius*/ 40, /*Height*/ 40),
    SET_INT(oIntangibleTimer, 0),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, ukiki_seg5_anims_05015784),
    ANIMATE(UKIKI_ANIM_RUN),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_ukiki_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ukiki_loop),
    END_LOOP(),
};
