#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "behavior_data.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "levels/ttm/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvUkikiCage[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_LONG(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_DONT_CALC_COLL_DIST)),
    SET_HOME(),
    LOAD_COLLISION_DATA(ttm_seg7_collision_ukiki_cage),
    SPAWN_CHILD(/*Model*/ MODEL_STAR, /*Behavior*/ bhvUkikiCageStar),
    SPAWN_CHILD(/*Model*/ MODEL_NONE, /*Behavior*/ bhvUkikiCageChild),
    SET_FLOAT(oCollisionDistance, 20000),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ukiki_cage_loop),
    END_LOOP(),
};
