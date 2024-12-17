#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvShallowWaterSplash[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DISABLE_RENDERING(),
    BEGIN_REPEAT(18),
        SPAWN_WATER_DROPLET(&gShallowWaterSplashDropletParams),
    END_REPEAT_CONTINUE(),
    CALL_NATIVE(bhv_shallow_water_splash_init),
    DELAY(1),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_SHALLOW_WATER_SPLASH),
    DEACTIVATE(),
};
