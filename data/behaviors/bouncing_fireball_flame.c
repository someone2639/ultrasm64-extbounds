#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvBouncingFireballFlame[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    SET_FLOAT(oGraphYOffset, 30),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 25, /*Downwards offset*/ 25),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bouncing_fireball_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};
