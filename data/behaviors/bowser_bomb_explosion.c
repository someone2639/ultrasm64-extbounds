#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBowserBombExplosion[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, -288),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_bomb_explosion_loop),
    END_LOOP(),
};
