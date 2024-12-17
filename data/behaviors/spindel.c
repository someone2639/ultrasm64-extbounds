#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/ssl/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvSpindel[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(ssl_seg7_collision_spindel),
    CALL_NATIVE(bhv_spindel_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_spindel_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
