#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvMistParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_DUST),
    DISABLE_RENDERING(),
    SPAWN_CHILD(/*Model*/ MODEL_MIST, /*Behavior*/ bhvWhitePuff1),
    SPAWN_CHILD(/*Model*/ MODEL_SMOKE, /*Behavior*/ bhvWhitePuff2),
    DELAY(1),
    DEACTIVATE(),
};
