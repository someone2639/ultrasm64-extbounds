#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"
#include "engine/surface_load.h"

#include "actors/common1.h"
#include "behavior_commands.h"

const BehaviorScript bhvWarpPipe[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_WARP),
    LOAD_COLLISION_DATA(warp_pipe_seg3_collision_03009AC8),
    SET_FLOAT(oDrawingDistance, 16000),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX(/*Radius*/ 70, /*Height*/ 50),
    CALL_NATIVE(load_object_static_model),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_warp_loop),
    END_LOOP(),
};
