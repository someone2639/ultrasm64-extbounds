#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvTreasureChestBottom[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DROP_TO_FLOOR(),
    CALL_NATIVE(bhv_treasure_chest_bottom_init),
    SET_INT(oIntangibleTimer, -1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_treasure_chest_bottom_loop),
    END_LOOP(),
};
