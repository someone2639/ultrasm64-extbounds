#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvInvisibleObjectsUnderBridge[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    CALL_NATIVE(bhv_invisible_objects_under_bridge_init),
    BREAK(),
};
