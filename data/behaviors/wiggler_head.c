#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group11.h"
#include "behavior_commands.h"

const BehaviorScript bhvWigglerHead[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, wiggler_seg5_anims_0500EC8C),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 60, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    HIDE(),
    SCALE(/*Unused*/ 0, /*Field*/ 400),
    SET_FLOAT(oWigglerFallThroughFloorsHeight, 5000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wiggler_update),
    END_LOOP(),
};
