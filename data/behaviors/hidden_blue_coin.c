#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvHiddenBlueCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    SET_INT(oInteractType, INTERACT_COIN),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 64),
    SET_INT(oDamageOrCoinValue, 5),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_hidden_blue_coin_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};
