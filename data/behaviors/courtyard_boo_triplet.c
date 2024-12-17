#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvCourtyardBooTriplet[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    DISABLE_RENDERING(),
    CALL_NATIVE(bhv_courtyard_boo_triplet_init),
    DEACTIVATE(),
};
