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

typedef enum {
    GDCMD_BEGIN_LIST = 53716,
    GDCMD_USE_INTEGER_NAMES = 0,
    GDCMD_SET_INITIAL_POSITION = 1,
    GDCMD_SET_RELATIVE_POSITION = 2,
    GDCMD_SET_WORLD_POSITION = 3,
    GDCMD_SET_NORMAL = 4,
    GDCMD_SET_SCALE = 5,
    GDCMD_SET_ROTATION = 6,
    GDCMD_SET_DRAW_FLAG = 7,
    GDCMD_SET_FLAG = 8,
    GDCMD_CLEAR_FLAG = 9,
    GDCMD_SET_FRICTION = 10,
    GDCMD_SET_SPRING = 11,
    GDCMD_CALL_LIST = 12,
    GDCMD_SET_COLOUR_NUM = 13,
    GDCMD_MAKE_DYN_OBJ = 15,
    GDCMD_START_GROUP = 16,
    GDCMD_END_GROUP = 17,
    GDCMD_ADD_TO_GROUP = 18,
    GDCMD_SET_TYPE = 19,
    GDCMD_SET_MATERIAL_GROUP = 20,
    GDCMD_SET_NODE_GROUP = 21,
    GDCMD_SET_SKIN_SHAPE = 22,
    GDCMD_SET_PLANE_GROUP = 23,
    GDCMD_SET_SHAPE_PTR_PTR = 24,
    GDCMD_SET_SHAPE_PTR = 25,
    GDCMD_SET_SHAPE_OFFSET = 26,
    GDCMD_SET_CENTER_OF_GRAVITY = 27,
    GDCMD_LINK_WITH = 28,
    GDCMD_LINK_WITH_PTR = 29,
    GDCMD_USE_OBJECT = 30,
    GDCMD_SET_CONTROL_TYPE = 31,
    GDCMD_SET_SKIN_WEIGHT = 32,
    GDCMD_SET_AMBIENT = 33,
    GDCMD_SET_DIFFUSE = 34,
    GDCMD_SET_ID = 35,
    GDCMD_SET_MATERIAL = 36,
    GDCMD_MAP_MATERIALS = 37,
    GDCMD_MAP_VERTICES = 38,
    GDCMD_ATTACH = 39,
    GDCMD_ATTACH_TO = 40,
    GDCMD_SET_ATTACH_OFFSET = 41,
    GDCMD_SET_NAME_SUFFIX = 43,
    GDCMD_SET_PARAM_F = 44,
    GDCMD_SET_PARAM_PTR = 45,
    GDCMD_MAKE_NET_WITH_SUB_GROUP = 46,
    GDCMD_MAKE_ATTACHED_JOINT = 47,
    GDCMD_END_NET_WITH_SUB_GROUP = 48,
    GDCMD_MAKE_VERTEX = 49,
    GDCMD_MAKE_VAL_PTR = 50,
    GDCMD_USE_TEXTURE = 52,
    GDCMD_SET_TEXTURE_S_T = 53,
    GDCMD_MAKE_NET_FROM_SHAPE = 54,
    GDCMD_MAKE_NET_FROM_SHAPE_PTR_PTR = 55,
    GDCMD_END_LIST = 58,
} DynlistCMD;

// flags for dAttachTo and friends
#define ATTACH_FLAG_RELATIVE_TO_PARENT 0x9
#define ATTACH_FLAG_RELATIVE (0x4 | ATTACH_FLAG_RELATIVE_TO_PARENT)

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
void dSetID(s32 id);
void gdResetDynListProcessor(void);
struct GdObj* gdProcessDynList(struct DynList *dylist);
void dSetNameSuffix(char *str);
struct GdObj* dMakeObject(enum DObjTypes type, DynObjName name);
void dSetShapePointerFromPointer(struct ObjShape **shpPtrptr);
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
