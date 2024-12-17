#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group3.h"
#include "behavior_commands.h"

const BehaviorScript bhvKingBobomb[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, king_bobomb_seg5_anims_0500FE30),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 100),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_INT(oIntangibleTimer, 0),
    DROP_TO_FLOOR(),
    SET_HOME(),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvBobombAnchorMario),
    SET_INT(oHealth, 3),
    SET_INT(oDamageOrCoinValue, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_king_bobomb_loop),
    END_LOOP(),
};
