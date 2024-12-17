#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "actors/group7.h"
#include "behavior_commands.h"

const BehaviorScript bhvSpindrift[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, spindrift_seg5_anims_05002D68),
    ANIMATE(SPINDRIFT_ANIM_DEFAULT),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_TWIRL_BOUNCE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_spindrift_loop),
    END_LOOP(),
};
