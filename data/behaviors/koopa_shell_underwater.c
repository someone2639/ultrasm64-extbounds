#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvKoopaShellUnderwater[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_LONG(oFlags, (OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_SILHOUETTE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_koopa_shell_underwater_loop),
    END_LOOP(),
};
