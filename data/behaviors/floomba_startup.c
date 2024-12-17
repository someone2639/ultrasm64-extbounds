#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvFloombaStartup[] = {
    BEGIN(OBJ_LIST_PUSHABLE),
    SET_INT(oIsFloomba, TRUE),
    SET_INT(oAction, FLOOMBA_ACT_STARTUP),
    OR_INT(oFlags, OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_ANIMATIONS(oAnimations, goomba_seg8_anims_0801DA4C),
    SET_HOME(),
    CALL_NATIVE(bhv_goomba_init),
    SET_FLOAT(oDrawingDistance, 30000),
    SET_INT(oIntangibleTimer, -1),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ 0, /*Bounciness*/ 0, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_goomba_update),
    END_LOOP(),
};
