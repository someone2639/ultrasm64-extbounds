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

const BehaviorScript bhvHorizontalGrindel[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ssl_seg7_collision_grindel),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DROP_TO_FLOOR(),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 40, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SCALE(/*Unused*/ 0, /*Field*/ 90),
    CALL_NATIVE(bhv_horizontal_grindel_init),
    BEGIN_LOOP(),
        CALL_NATIVE(cur_obj_update_floor_and_walls),
        CALL_NATIVE(bhv_horizontal_grindel_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
