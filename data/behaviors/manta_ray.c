#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group4.h"
#include "behavior_commands.h"

const BehaviorScript bhvMantaRay[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, manta_seg5_anims_05008EB4),
    ANIMATE(MANTA_ANIM_SWIM),
    CALL_NATIVE(bhv_manta_ray_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_manta_ray_loop),
    END_LOOP(),
};
