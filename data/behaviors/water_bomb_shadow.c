#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvWaterBombShadow[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SCALE(/*Unused*/ 0, /*Field*/ 150),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_water_bomb_shadow_update),
    END_LOOP(),
};
