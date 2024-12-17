#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "actors/group11.h"
#include "behavior_commands.h"

const BehaviorScript bhvEnemyLakitu[] = {
    BEGIN(OBJ_LIST_PUSHABLE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, lakitu_enemy_seg5_anims_050144D4),
    ANIMATE(ENEMY_LAKITU_ANIM_SPAWN),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 40, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_enemy_lakitu_update),
    END_LOOP(),
};
