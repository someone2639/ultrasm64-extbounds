#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvPurpleParticle[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(10),
        CALL_NATIVE(bhv_piranha_particle_loop),
    END_REPEAT(),
    DEACTIVATE(),
};
