#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group13.h"
#include "behavior_commands.h"

const BehaviorScript bhvMantaRayWaterRing[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_ANIMATIONS(oAnimations, water_ring_seg6_anims_06013F7C),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 75, /*Height*/ 20, /*Downwards offset*/ 20),
    SET_INTERACT_TYPE(INTERACT_WATER_RING),
    SET_INT(oDamageOrCoinValue, 2),
    SET_INT(oIntangibleTimer, 0),
    CALL_NATIVE(bhv_manta_ray_water_ring_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_manta_ray_water_ring_loop),
    END_LOOP(),
};
