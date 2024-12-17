#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"
#include "game/interaction.h"

#include "behavior_commands.h"

const BehaviorScript bhvKoopaShellFlame[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_koopa_shell_flame_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};
