#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/castle_inside/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvFloorTrapInCastle[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(inside_castle_seg7_collision_floor_trap),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_floor_trap_in_castle_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
