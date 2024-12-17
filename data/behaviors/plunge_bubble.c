#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvPlungeBubble[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_PLUNGE_BUBBLE),
    DISABLE_RENDERING(),
    CALL_NATIVE(bhv_water_waves_init),
    DEACTIVATE(),
};
