#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"
#include "engine/surface_load.h"

#include "levels/castle_inside/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvStarDoor[] = {
    BEGIN(OBJ_LIST_SURFACE),
    SET_INT(oInteractType, INTERACT_DOOR),
    LOAD_COLLISION_DATA(inside_castle_seg7_collision_star_door),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_STAR_DOOR),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 100),
    SET_HOME(),
    SET_FLOAT(oDrawingDistance, 20000),
    CALL_NATIVE(bhv_door_init),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_star_door_loop),
        CALL_NATIVE(load_object_collision_model),
        CALL_NATIVE(bhv_door_rendering_loop),
    END_LOOP(),
};
