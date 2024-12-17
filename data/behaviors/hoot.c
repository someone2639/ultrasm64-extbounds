#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group1.h"
#include "behavior_commands.h"

const BehaviorScript bhvHoot[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, hoot_seg5_anims_05005768),
    SET_INT(oInteractType, INTERACT_HOOT),
    SET_HITBOX(/*Radius*/ 75, /*Height*/ 75),
    CALL_NATIVE(bhv_hoot_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_hoot_loop),
    END_LOOP(),
};
