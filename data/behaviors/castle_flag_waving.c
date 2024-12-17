#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/castle_grounds/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvCastleFlagWaving[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_ANIMATIONS(oAnimations, castle_grounds_seg7_anims_flags),
    ANIMATE(CASTLE_FLAG_ANIM_WAVE),
    CALL_NATIVE(bhv_castle_flag_init),
    BEGIN_LOOP(),
    END_LOOP(),
};
