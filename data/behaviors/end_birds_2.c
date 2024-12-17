#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group10.h"
#include "behavior_commands.h"

const BehaviorScript bhvEndBirds2[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, birds_seg5_anims_050009E8),
    ANIMATE(END_BIRDS_ANIM_FLY),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_end_birds_2_loop),
    END_LOOP(),
};
