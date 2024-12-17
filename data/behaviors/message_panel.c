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

const BehaviorScript bhvMessagePanel[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wooden_signpost_seg3_collision_0302DD80),
    SET_INTERACT_TYPE(INTERACT_TEXT),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_SIGN),
    DROP_TO_FLOOR(),
    SET_HITBOX(/*Radius*/ 150, /*Height*/ 80),
    SET_INT(oWoodenPostTotalMarioAngle, 0),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(load_object_static_model),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        SET_INT(oInteractStatus, INT_STATUS_NONE),
    END_LOOP(),
};
