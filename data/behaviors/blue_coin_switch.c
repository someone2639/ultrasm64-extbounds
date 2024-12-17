#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvBlueCoinSwitch[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(blue_coin_switch_seg8_collision_08000E98),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_coin_switch_loop),
    END_LOOP(),
};
