#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvRotatingExclamationMark[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SCALE(/*Unused*/ 0, /*Field*/ 200),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_rotating_exclamation_mark_loop),
        ADD_INT(oMoveAngleYaw, 0x800),
    END_LOOP(),
};
