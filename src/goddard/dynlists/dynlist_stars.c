#include <PR/ultratypes.h>

#include "dynlist_macros.h"
#include "dynlists.h"
#include "animdata.h"
#include "../dynlist_proc.h"
#include "../shape_helper.h"

struct DynList dynlist_stars[] = {
    BeginList(),
    UseIntegerNames(TRUE),

    StartGroup(234),

        // silver star animator
        MakeDynObj(D_DATA_GRP, DYNOBJ_SILVER_STAR_ANIMDATA_GROUP),
            LinkWithPtr(&anim_silver_star),
            MakeDynObj(D_ANIMATOR, DYNOBJ_SILVER_STAR_ANIMATOR),
                AttachTo(0x0, DYNOBJ_MARIO_MAIN_ANIMATOR),
                SetNodeGroup(DYNOBJ_SILVER_STAR_ANIMDATA_GROUP),
                LinkWith(DYNOBJ_SILVER_STAR_LIGHT),

        // red star animator
        MakeDynObj(D_DATA_GRP, DYNOBJ_RED_STAR_ANIMDATA_GROUP),
            LinkWithPtr(&anim_red_star),
        MakeDynObj(D_ANIMATOR, DYNOBJ_RED_STAR_ANIMATOR),
            AttachTo(0x0, DYNOBJ_MARIO_MAIN_ANIMATOR),
            SetNodeGroup(DYNOBJ_RED_STAR_ANIMDATA_GROUP),
            LinkWith(DYNOBJ_RED_STAR_LIGHT),

    EndGroup(234),

    UseObj(234),

    UseIntegerNames(FALSE),
    EndList(),
};