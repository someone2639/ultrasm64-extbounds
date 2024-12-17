#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvBobombExplosionBubble[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_bobomb_explosion_bubble_init),
    ADD_RANDOM_FLOAT(oPosX, /*Minimum*/ -50, /*Range*/ 100),
    ADD_RANDOM_FLOAT(oPosY, /*Minimum*/ -50, /*Range*/ 100),
    ADD_RANDOM_FLOAT(oPosZ, /*Minimum*/ -50, /*Range*/ 100),
    CALL_NATIVE(bhvBobombExplosionBubble3600),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhvBobombExplosionBubble3600),
        CALL_NATIVE(bhv_bobomb_explosion_bubble_loop),
    END_LOOP(),
};
