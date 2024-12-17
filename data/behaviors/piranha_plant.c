#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvPiranhaPlant[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, piranha_plant_seg6_anims_0601C31C),
    ANIMATE(PIRANHA_PLANT_ANIM_BITE),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 200),
    SET_HURTBOX(/*Radius*/ 50, /*Height*/ 200),
    SET_INT(oIntangibleTimer,   0),
    SET_INT(oDamageOrCoinValue, 3),
    SET_INT(oNumLootCoins,      5),
    SPAWN_CHILD(/*Model*/ MODEL_BUBBLE, /*Behavior*/ bhvPiranhaPlantBubble),
    SET_FLOAT(oDrawingDistance, 2000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_piranha_plant_loop),
    END_LOOP(),
};
