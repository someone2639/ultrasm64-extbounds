#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBlackSmokeBowser[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, 0),
    BEGIN_REPEAT(8),
        CALL_NATIVE(bhv_black_smoke_bowser_loop),
        ANIMATE_TEXTURE(oAnimState, 4),
    END_REPEAT(),
    DEACTIVATE(),
};
