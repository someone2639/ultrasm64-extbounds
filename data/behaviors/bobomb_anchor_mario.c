#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBobombAnchorMario[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_FLOAT(oParentRelativePosX, 100),
    SET_FLOAT(oParentRelativePosZ, 150),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bobomb_anchor_mario_loop),
    END_LOOP(),
};
