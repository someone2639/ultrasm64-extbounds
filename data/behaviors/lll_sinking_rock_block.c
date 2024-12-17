#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/lll/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvLllSinkingRockBlock[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_floating_block),
    ADD_FLOAT(oPosY, -50),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_sinking_rock_block_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
