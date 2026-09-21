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

        MakeDynObj(D_LIGHT, DYNOBJ_SILVER_STAR_LIGHT),
            SetId(1),
            SetDiffuse(1.0, 1.0, 1.0),
            SetFlag(0x20),
            SetShapePtrPtr(&gShapeSilverStar),

        MakeDynObj(D_LIGHT, DYNOBJ_RED_STAR_LIGHT),
            SetId(0),
            SetDiffuse(1.0, 0.0, 0.0),
            SetShapePtrPtr(&gShapeRedStar),

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
