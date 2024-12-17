#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/ttc/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvTTC2DRotator[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_clock_main_rotation),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oCollisionDistance, 1800),
    CALL_NATIVE(bhv_ttc_2d_rotator_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_2d_rotator_update),
    END_LOOP(),
};
