#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvHorStarParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    DISABLE_RENDERING(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_H_STAR),
    CALL_NATIVE(bhv_pound_tiny_star_particle_init),
    DELAY(1),
    DEACTIVATE(),
};
