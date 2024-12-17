#define OBJECT_FIELDS_INDEX_DIRECTLY
#include "sm64.h"
#include "types.h"

#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/behavior_actions.h"

#include "behavior_commands.h"

const BehaviorScript bhvSparkleParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_SPARKLES),
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oGraphYOffset, 25),
    SET_RANDOM_FLOAT(oMarioParticleFlags, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosX, /*Value 1*/ oPosX, /*Value 2*/ oMarioParticleFlags),
    SET_RANDOM_FLOAT(oMarioParticleFlags, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosZ, /*Value 1*/ oPosZ, /*Value 2*/ oMarioParticleFlags),
    SET_RANDOM_FLOAT(oMarioParticleFlags, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosY, /*Value 1*/ oPosY, /*Value 2*/ oMarioParticleFlags),
    SET_INT(oAnimState, OBJ_ANIM_STATE_INIT_ANIM),
    BEGIN_REPEAT(12),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};
