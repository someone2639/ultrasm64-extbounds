#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvCannon[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SPAWN_CHILD(/*Model*/ MODEL_CANNON_BARREL, /*Behavior*/ bhvCannonBarrel),
    SET_INT(oInteractType, INTERACT_CANNON_BASE),
    ADD_FLOAT(oPosY, -340),
    SET_HOME(),
    SET_HITBOX(/*Radius*/ 150, /*Height*/ 150),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_cannon_base_loop),
    END_LOOP(),
};
