#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group4.h"
#include "behavior_commands.h"

const BehaviorScript bhvSushiShark[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, sushi_seg5_anims_0500AE54),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 100, /*Height*/ 50, /*Downwards offset*/ 50),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_INT(oDamageOrCoinValue, 3),
    SET_HOME(),
    ANIMATE(SUSHI_ANIM_SWIM),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_sushi_shark_loop),
    END_LOOP(),
};
