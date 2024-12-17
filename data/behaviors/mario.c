#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvMario[] = {
    BEGIN(OBJ_LIST_PLAYER),
    SET_INT(oIntangibleTimer, 0),
    OR_LONG(oFlags, (OBJ_FLAG_PLAYER | OBJ_FLAG_SILHOUETTE)),
    OR_INT(oUnk94, 0x0001),
    SET_HITBOX(/*Radius*/ 37, /*Height*/ 160),
    BEGIN_LOOP(),
#ifdef VANILLA_DEBUG
        CALL_NATIVE(try_print_debug_mario_level_info),
#endif
        CALL_NATIVE(bhv_mario_update),
#ifdef VANILLA_DEBUG
        CALL_NATIVE(try_do_mario_debug_object_spawn),
#endif
    END_LOOP(),
};
