#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvMrIBlueCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    SET_INT(oInteractType, INTERACT_COIN),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oIntangibleTimer, 0),
    SET_FLOAT(oCoinBaseYVel, 20),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_coin_init),
    SET_INT(oDamageOrCoinValue, 5),
    SET_HITBOX(/*Radius*/ 120, /*Height*/ 64),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_coin_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};
