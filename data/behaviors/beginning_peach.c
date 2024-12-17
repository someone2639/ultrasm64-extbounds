#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group10.h"
#include "behavior_commands.h"

const BehaviorScript bhvBeginningPeach[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_ANIMATIONS(oAnimations, peach_seg5_anims_0501C41C),
    ANIMATE(PEACH_ANIM_0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_intro_peach_loop),
    END_LOOP(),
};
