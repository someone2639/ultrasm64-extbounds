#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvDecorativePendulum[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_decorative_pendulum_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_decorative_pendulum_loop),
    END_LOOP(),
};
