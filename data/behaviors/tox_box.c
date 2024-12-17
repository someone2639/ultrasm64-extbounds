#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/ssl/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvToxBox[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(ssl_seg7_collision_tox_box),
    ADD_FLOAT(oPosY, 256),
    SET_FLOAT(oDrawingDistance, 8000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tox_box_loop),
    END_LOOP(),
};
