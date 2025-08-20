#ifndef GD_DYNLIST_PROCESSOR_H
#define GD_DYNLIST_PROCESSOR_H

#include <PR/ultratypes.h>

#include "gd_types.h"

// types
/// @name DynObjName Type
/// @{
/// A new type for identification of `GdObj`s in the dynamic object list.
typedef void *DynObjName;
/// Macros for casting between types of names,
/// as the name can be either a number or a string.
/// @{
#define DynNameAsStr(name) ((char *)(name))
#define DynNameAsInt(name) ((u32)(uintptr_t)(name))
#define AsDynName(unk)   ((DynObjName)(unk))
/// @}
/// @}

/// parameters types for `dSetParmPointer()`
enum DParmPtr {
    PARM_PTR_OBJ_VTX = 1, ///< parameter is the index of a vertex to add to an `ObjFace`
    PARM_PTR_CHAR    = 5  ///< parameter is a `char *`
};

/// parameters for `dSetParmF()`
enum DParmF {
    PARM_F_ALPHA = 1,       ///< Set the alpha value for an `ObjShape` or `ObjVertex`
    PARM_F_RANGE_MIN = 2,  ///< Set the left range for an `ObjGadget`
    PARM_F_RANGE_MAX = 3, ///< Set the right range for an `ObjGadget`
    PARM_F_VARVAL = 6       ///< Set the float variable value union in an `ObjGadget`
};

/// `dMakeObject()` object types
enum DObjTypes {
    D_CAR_DYNAMICS  = 0,
    D_NET           = 1,
    D_JOINT         = 2,
    D_ANOTHER_JOINT = 3,
    D_CAMERA        = 4,
    D_VERTEX        = 5,
    D_FACE          = 6,
    D_PLANE         = 7,
    D_BONE          = 8,
    D_MATERIAL      = 9,
    D_SHAPE         = 10,
    D_GADGET        = 11,
    D_LABEL         = 12,
    D_VIEW          = 13,
    D_ANIMATOR      = 14,
    D_DATA_GRP      = 15, ///< An `ObjGroup` that links to raw vertex or face data
    D_PARTICLE      = 16,
    D_LIGHT         = 17,
    D_GROUP         = 18
};

// functions
void dStashDynobj(void);
void dUnstashDynobj(void);
void gdResetDynListProcessor(void);
struct GdObj* gdProcessDynList(struct DynList *dylist);
void dSetNameSuffix(char *str);
struct GdObj* dMakeObject(enum DObjTypes type, DynObjName name);
void dSetShapePtrPtr(struct ObjShape **shpPtrptr);
struct GdObj* dUseObject(DynObjName name);
void set_cur_dynobj(struct GdObj *obj);
void dStartGroup(DynObjName name);
void dEndGroup(DynObjName name);
void dUseIntegerNames(s32 isIntBool);
void dSetInitPos(f32 x, f32 y, f32 z);
void dGetInitPos(struct GdVec3f *dst);
void dGetInitRot(struct GdVec3f *dst);
void dSetRelativePosition(f32 x, f32 y, f32 z);
void dGetRelPos(struct GdVec3f *dst);
struct ObjGroup * dGetAttachedObjGroup(void);
void dGetScale(struct GdVec3f *dst);
void dSetWorldPos(f32 x, f32 y, f32 z);
void dGetWorldPosition(struct GdVec3f *dst);
void dSetScale(f32 x, f32 y, f32 z);
void dAddValuePointer(DynObjName name, u32 vflags, enum ValPtrType type, size_t offset);
void dAddValproc(union ObjVarVal * (*)(union ObjVarVal *, union ObjVarVal));
void dSetFlags(s32 flags);
void dSetParmF(enum DParmF param, f32 val);
void dSetParmPointer(enum DParmPtr param, void *ptr);
void dSetObjectDrawFlag(enum ObjDrawingFlags flag);
void dSetType(s32 type);
void dSetColourNum(s32 colornum);
void dSetDiffuse(f32 r, f32 g, f32 b);
struct GdBoundingBox* dGetBoundingBox(void);
void dGetMatrix(Mat4f *dst);
Mat4f * dGetRotationMatrixPointer(void);
void dSetIMatrix(Mat4f *src);
Mat4f * dGetMatrixPointer(void);
Mat4f * dGetIMatrixPointer(void);
f32 dCalcWorldDistBtwn(struct GdObj *obj1, struct GdObj *obj2);

#endif // GD_DYNLIST_PROCESSOR_H
