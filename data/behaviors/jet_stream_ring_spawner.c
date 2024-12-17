#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvJetStreamRingSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    HIDE(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_jet_stream_ring_spawner_loop),
    END_LOOP(),
};
