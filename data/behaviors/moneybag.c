#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group16.h"
#include "behavior_commands.h"

const BehaviorScript bhvMoneybag[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, moneybag_seg6_anims_06005E5C),
    DROP_TO_FLOOR(),
    SET_HOME(),
    SET_INT(oIntangibleTimer, -1),
    CALL_NATIVE(bhv_moneybag_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_moneybag_loop),
    END_LOOP(),
};
