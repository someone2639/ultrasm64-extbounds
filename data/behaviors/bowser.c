#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group12.h"
#include "behavior_commands.h"

const BehaviorScript bhvBowser[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_HITBOX(/*Radius*/ 400, /*Height*/ 400),
    DROP_TO_FLOOR(),
    SET_HOME(),
    LOAD_ANIMATIONS(oAnimations, bowser_seg6_anims_06057690),
    SPAWN_CHILD(/*Model*/ MODEL_NONE, /*Behavior*/ bhvBowserBodyAnchor),
    SPAWN_CHILD(/*Model*/ MODEL_BOWSER_BOMB_CHILD_OBJ, /*Behavior*/ bhvBowserFlameSpawn),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvBowserTailAnchor),
    // Beta leftover that spawn 50 coins when Bowser is defeated
    SET_INT(oNumLootCoins, 50),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_bowser_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_loop),
    END_LOOP(),
};
