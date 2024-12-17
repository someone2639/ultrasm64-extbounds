#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/castle_grounds/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvMoatGrills[] = {
    BEGIN(OBJ_LIST_SURFACE),
#ifdef UNLOCK_ALL
    DEACTIVATE(),
#else
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(castle_grounds_seg7_collision_moat_grills),
    SET_FLOAT(oCollisionDistance, 30000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_moat_grills_loop),
    END_LOOP(),
#endif
};
