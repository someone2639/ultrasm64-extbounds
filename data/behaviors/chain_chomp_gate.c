#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/bob/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvChainChompGate[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(bob_seg7_collision_chain_chomp_gate),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    CALL_NATIVE(bhv_chain_chomp_gate_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_chain_chomp_gate_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
