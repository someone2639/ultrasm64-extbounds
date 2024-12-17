#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvKoopa[] = {
    BEGIN(OBJ_LIST_PUSHABLE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, koopa_seg6_anims_06011364),
    ANIMATE(KOOPA_ANIM_WALK),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 50, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SCALE(/*Unused*/ 0, /*Field*/ 150),
    SET_FLOAT(oKoopaAgility, 1),
    CALL_NATIVE(bhv_koopa_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_koopa_update),
    END_LOOP(),
};
