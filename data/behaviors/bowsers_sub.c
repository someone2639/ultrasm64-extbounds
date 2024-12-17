#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/ddd/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvBowsersSub[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oDrawingDistance, 20000),
    LOAD_COLLISION_DATA(ddd_seg7_collision_submarine),
    CALL_NATIVE(bhv_bowsers_sub_init),
    BREAK(),
};
