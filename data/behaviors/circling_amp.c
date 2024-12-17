#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/common0.h"
#include "behavior_commands.h"

const BehaviorScript bhvCirclingAmp[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, dAmpAnimsList),
    ANIMATE(AMP_ANIM_DEFAULT),
    SET_FLOAT(oGraphYOffset, 40),
    SET_INT(oIntangibleTimer, 0),
    CALL_NATIVE(bhv_circling_amp_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_circling_amp_loop),
    END_LOOP(),
};
