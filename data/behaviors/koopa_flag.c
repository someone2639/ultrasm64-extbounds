#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group14.h"
#include "behavior_commands.h"

const BehaviorScript bhvKoopaFlag[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    SET_INTERACT_TYPE(INTERACT_POLE),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 700),
    SET_INT(oIntangibleTimer, 0),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, koopa_flag_seg6_anims_06001028),
    ANIMATE(KOOPA_FLAG_ANIM_WAVE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_pole_base_loop),
    END_LOOP(),
};
