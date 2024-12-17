#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common1.h"
#include "behavior_commands.h"

const BehaviorScript bhvBowserKeyCourseExit[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    LOAD_ANIMATIONS(oAnimations, bowser_key_seg3_anims_list),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_key_course_exit_loop),
    END_LOOP(),
};
