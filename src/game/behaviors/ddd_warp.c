#include <ultra64.h>
#include "actors/vanilla_actors/castle_inside/header.h"
#include "global_object_fields.h"
#include "game/object_helpers.h"

void bhv_ddd_warp_loop(void) {
#ifndef UNLOCK_ALL
    if (gDddPaintingStatus & DDD_FLAG_BOWSERS_SUB_BEATEN) {
#endif
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp_2);
#ifndef UNLOCK_ALL
    } else {
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp);
    }
#endif
}
