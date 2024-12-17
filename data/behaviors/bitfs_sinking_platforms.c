#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/bitfs/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvBitfsSinkingPlatforms[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(bitfs_seg7_collision_sinking_platform),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bitfs_sinking_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
