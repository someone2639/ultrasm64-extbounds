#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group15.h"
#include "behavior_commands.h"

const BehaviorScript bhvToadMessage[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_PERSISTENT_RESPAWN | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, toad_seg6_anims_0600FB58),
    ANIMATE(TOAD_ANIM_WEST_WAVING_BOTH_ARMS),
    SET_INTERACT_TYPE(INTERACT_TEXT),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 100),
    SET_INT(oIntangibleTimer, 0),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_toad_message_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_toad_message_loop),
    END_LOOP(),
};
