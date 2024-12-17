#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBobombFuseSmoke[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    CALL_NATIVE(bhv_bobomb_fuse_smoke_init),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_dust_smoke_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};
