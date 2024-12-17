#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "engine/surface_load.h"

#include "levels/lll/header.h"
#include "behavior_commands.h"

const BehaviorScript bhvLllBowserPuzzlePiece[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_LONG(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_DONT_CALC_COLL_DIST)),
    LOAD_COLLISION_DATA(lll_seg7_collision_puzzle_piece),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 3000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_bowser_puzzle_piece_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
