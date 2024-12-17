#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"
#include "engine/surface_load.h"

#include "levels/ttm/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvExitPodiumWarp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_WARP),
    DROP_TO_FLOOR(),
    SET_FLOAT(oDrawingDistance, 8000),
    SET_FLOAT(oCollisionDistance, 8000),
    LOAD_COLLISION_DATA(ttm_seg7_collision_podium_warp),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX(/*Radius*/ 50, /*Height*/ 50),
    BEGIN_LOOP(),
        CALL_NATIVE(load_object_collision_model),
        SET_INT(oInteractStatus, INT_STATUS_NONE),
    END_LOOP(),
};
