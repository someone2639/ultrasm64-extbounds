#include <PR/ultratypes.h>
#include <stdio.h>

#include "debug_utils.h"
#include "dynlist_proc.h"
#include "gd_types.h"
#include "macros.h"
#include "objects.h"
#include "gadgets.h"
#include "renderer.h"

/**
 * @file gadgets.c
 *
 * This file contains remnants of code for rendering what appears to be a GUI
 * that used the IRIX Graphics Library, from when this program was a standalone demo.
 * It also contains code for creating labels and gadget, which are `GdObj`s that
 * allow for displaying text and memory values on screen. Those `GdObj`s are not
 * created in-game, but there are some functions in `renderer.c` that use
 * them, and those functions may still work if called.
 */

/* 239EC0 -> 239F78 */
void get_objvalue(union ObjVarVal *dst, enum ValPtrType type, void *base, size_t offset) {
    union ObjVarVal *objAddr = (void *) ((u8 *) base + offset);

    switch (type) {
        case OBJ_VALUE_INT:
            dst->i = objAddr->i;
            break;
        case OBJ_VALUE_FLOAT:
            dst->f = objAddr->f;
            break;
        default:
            fatal_printf("%s: Undefined ValueType", "get_objvalue");
    }
}

/* 23A190 -> 23A250 */
struct ObjLabel *make_label(struct ObjValPtr *ptr, char *str, s32 a2, f32 x, f32 y, f32 z) {
    struct ObjLabel *label = (struct ObjLabel *) make_object(OBJ_TYPE_LABELS);
    label->valfn = NULL;
    label->valptr = ptr;
    label->fmtstr = str;
    label->unk24 = a2;
    label->unk30 = 4;
    label->position.x = x;
    label->position.y = y;
    label->position.z = z;

    return label;
}

/* 23A250 -> 23A32C */
struct ObjGadget *make_gadget(UNUSED s32 a0, s32 a1) {
    struct ObjGadget *gdgt = (struct ObjGadget *) make_object(OBJ_TYPE_GADGETS);
    gdgt->valueGrp = NULL;
    gdgt->rangeMax = 1.0f;
    gdgt->rangeMin = 0.0f;
    gdgt->unk20 = a1;
    gdgt->colourNum = 0;
    gdgt->sliderPos = 1.0f;
    gdgt->size.x = 100.0f;
    gdgt->size.y = 10.0f;
    gdgt->size.z = 10.0f;  // how is this useful?

    return gdgt;
}

/* 23A784 -> 23A940; orig name: Unknown8018BFB4 */
void reset_gadget(struct ObjGadget *gdgt) {
    f32 range;
    struct ObjValPtr *vp;

    if (gdgt->rangeMax - gdgt->rangeMin == 0.0f) {
        fatal_printf("gadget has zero range (%f -> %f)\n", gdgt->rangeMin, gdgt->rangeMax);
    }

    range = (f32)(1.0 / (gdgt->rangeMax - gdgt->rangeMin));

    if (gdgt->valueGrp != NULL) {
        vp = (struct ObjValPtr *) gdgt->valueGrp->firstMember->obj;

        switch (vp->datatype) {
            case OBJ_VALUE_FLOAT:
                get_objvalue(&gdgt->varval, OBJ_VALUE_FLOAT, vp->obj, vp->offset);
                gdgt->sliderPos = (gdgt->varval.f - gdgt->rangeMin) * range;
                break;
            case OBJ_VALUE_INT:
                get_objvalue(&gdgt->varval, OBJ_VALUE_INT, vp->obj, vp->offset);
                gdgt->sliderPos = (gdgt->varval.i - gdgt->rangeMin) * range;
                break;
            default:
                fatal_printf("%s: Undefined ValueType", "reset_gadget");
        }
    }
}

/* 23A940 -> 23A980 */
void reset_gadgets_in_grp(struct ObjGroup *grp) {
    apply_to_obj_types_in_group(OBJ_TYPE_GADGETS, (applyproc_t) reset_gadget, grp);
}
