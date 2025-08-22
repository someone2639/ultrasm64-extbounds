#ifndef GD_TYPES_H
#define GD_TYPES_H

#include <ultra64.h>

/* Vector Types */
struct GdVec3f {
    f32 x, y, z;
};

struct GdBoundingBox {
    f32 minX, minY, minZ;
    f32 maxX, maxY, maxZ;
};

struct GdTriangleF {
    struct GdVec3f p0, p1, p2;
};

struct GdAnimTransform {
    struct GdVec3f scale;
    struct GdVec3f rotate;  // each component specifies the degrees to rotate about that axis
    struct GdVec3f pos;
};

typedef f32 Mat4f[4][4];

struct GdColour {
    f32 r, g, b;
};

/* dynlist entries and types */
union DynUnion {
    void *ptr;
    char *str;
    s32 word;
};

struct DynList {
    s32 cmd;
    union DynUnion w1;
    union DynUnion w2;
    struct GdVec3f vec;
};

/* Goddard Code Object Structs */
/* Object Type Flags */
enum ObjTypeFlag {
    OBJ_TYPE_GROUPS    = 0x00000001,
    OBJ_TYPE_BONES     = 0x00000002,
    OBJ_TYPE_JOINTS    = 0x00000004,
    OBJ_TYPE_PARTICLES = 0x00000008,
    OBJ_TYPE_SHAPES    = 0x00000010,
    OBJ_TYPE_NETS      = 0x00000020,
    OBJ_TYPE_PLANES    = 0x00000040,
    OBJ_TYPE_FACES     = 0x00000080,
    OBJ_TYPE_VERTICES  = 0x00000100,
    OBJ_TYPE_CAMERAS   = 0x00000200,
    // 0x400 was not used
    OBJ_TYPE_MATERIALS = 0x00000800,
    OBJ_TYPE_WEIGHTS   = 0x00001000,
    OBJ_TYPE_GADGETS   = 0x00002000,
    OBJ_TYPE_VIEWS     = 0x00004000,
    OBJ_TYPE_LABELS    = 0x00008000,
    OBJ_TYPE_ANIMATORS = 0x00010000,
    OBJ_TYPE_VALPTRS   = 0x00020000,
    // 0x40000 was not used
    OBJ_TYPE_LIGHTS    = 0x00080000,
    OBJ_TYPE_ZONES     = 0x00100000,
    OBJ_TYPE_UNK200000 = 0x00200000
};
/* This constant seems to be used to indicate the type of any or all objects */
#define OBJ_TYPE_ALL 0x00FFFFFF


/// Function pointer for a `GdObj`'s drawing routine
typedef void (*drawmethod_t)(void *);
/// Flags for the drawFlags field of an GdObj
enum ObjDrawingFlags {
    OBJ_DRAW_UNK01   = 0x01,
    OBJ_INVISIBLE    = 0x02, ///< This `GdObj` shouldn't be drawn when updating a scene
    OBJ_PICKED       = 0x04, ///< This `GdObj` is held by the cursor
    OBJ_IS_GRABBABLE = 0x08, ///< This `GdObj` can be grabbed/picked by the cursor
    OBJ_HIGHLIGHTED  = 0x10
};

/**
 * The base of structure of all of Goddard's objects. It is present as a "header"
 * at the beginning of all `ObjX` structures, and as such, this type is used
 * when we need to generalize code to take different `ObjX`es.
 * Every GdObj created is connected together in a linked list with `prev` and `next` pointers.
 */
struct GdObj {
    struct GdObj *prev;  // previous node in gGdObjectList linked list
    struct GdObj *next;  // next node in gGdObjectList linked list
    drawmethod_t objDrawFn;
    enum ObjTypeFlag type;
    s16 index;    ///< the index of this `GdObj` in the linked list
    u16 drawFlags; ///< enumerated in `::ObjDrawingFlags`
    /* 0x14 Specific object data starts here  */
};

/* Used to create a linked list of objects (or data)
** within an ObjGroup */
struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
    struct GdObj *obj;
};

// I think this is actually the same type as ListNode, and data is just a generic "void *" pointer.
struct VtxLink {
    struct VtxLink *prev;
    struct VtxLink *next;
    Vtx *data;
};

/* These are the compressed versions of ObjFace or ObjVertex that are
** pointed to by ListNode in the faceGroup and vtxGroup, if Group.linkType
** is set to 0x01. See `chk_shapegen` */
struct GdFaceData {
    u32 count;
    s32 type;
    u16 (*data)[4]; ///< (mtl id, vtx ids[3])
};

struct GdVtxData {
    u32 count;
    s32 type;
    s16 (*data)[3]; ///< [x, y, z]
};


/**
 * Used to group related objects together. Seems to mainly be used for `apply_to_obj_types_in_group`.
 */
struct ObjGroup {
    struct GdObj header;
    struct ObjGroup *prev;
    struct ObjGroup *next;
    struct ListNode *firstMember; ///< Head of a linked list for objects contained in this group
    struct ListNode *lastMember; ///< Tail of a linked list for objects contained in this group
    s32 memberTypes;   ///< OR'd collection of type flags for all objects in this group
    s32 memberCount;  // number of objects in this group
    s32 debugPrint;  // might also be a type?
    s32 linkType;
    char name[0x40]; ///< possibly, only referenced in old code
    s32 id;
};

/* Known linkTypes
 * 0x00 : Normal (link to GdObj)
 * 0x01 : Compressed (vtx or face data)
 */

struct ObjBone {
    struct GdObj header;
    struct GdVec3f worldPos;   // "position"?? from dead code in draw_bone
    struct ObjBone *prev;   // maybe, based on make_bone
    struct ObjBone *next;   // maybe, based on make_bone
    struct GdVec3f unk28;   // "rotation"?? from dead code in draw_bone
    struct GdVec3f unk40;
    struct GdVec3f unk58;  // orientation?
    struct GdVec3f unk64;
    Mat4f mat70;
    Mat4f matB0;
    struct ObjShape *shapePtr; // from dead code in draw_bone
    f32 unkF4;              // also length?
    f32 unkF8;              // length?
    f32 unkFC;              // also length?
    s32 colourNum;             // "colour"
    s32 unk104;             // "flags"
    s32 id;
    struct ObjGroup *unk10C; // group of joints?
    f32 spring;
    f32 unk114;
    f32 unk118;
};

struct ObjJoint {
    struct GdObj header;
    struct GdVec3f worldPos;    // position in world space
    struct ObjShape *shapePtr;
    struct ObjJoint *prevjoint; // prev joint? linked joint?
    struct ObjJoint *nextjoint;
    void (*updateFunc)(struct ObjJoint*);  // seems to update attached objects? see grabbable_joint_update_func
    struct GdVec3f unk30;   // huge array of vecs? another matrix?
    struct GdVec3f unk3C;   // relative position?
    struct GdVec3f unk48;
    struct GdVec3f initPos;   // attached offset? (with +200 as well)
    struct GdVec3f unk6C;   // initial rotation vec
    struct GdVec3f velocity;
    struct GdVec3f unk84;
    struct GdVec3f unk90;
    struct GdVec3f scale;
    struct GdVec3f unkA8;
    struct GdVec3f unkB4;
    struct GdVec3f shapeOffset;
    struct GdVec3f unkCC;
    struct GdVec3f friction;
    Mat4f matE8;     //matrix4x4
    Mat4f mat128;    // "rot matrix"
    Mat4f mat168;    // "id matrix"
    struct GdVec3f unk1A8;
    s32 id;
    s32 flags;     // "flags" - 0x2000 = grabbed
    s32 unk1C0;
    struct ObjGroup *unk1C4;    // bone group?
    s32 colourNum;
    s32 type;     // 0 = normal joint, 5 = grabbable joint. seems to be set, but never used
    struct ObjAnimator *rootAnimator;  // root animator? used by eye_joint_update_func
    struct ObjGroup *weightGrp;    //Group of ObjWeights, only? skin weights?
    struct ObjGroup *attachedObjsGrp;    //attach object group
    s32 attachFlags;                 //d_attach_to arg 0; "AttFlag"
    struct GdVec3f attachOffset;
    struct GdObj *attachedToObj;  // object that this object is attached to
    f32 unk228;
};

/* Particle Types (+60)
   3 = Has groups of other particles in 6C?
*/

struct ObjParticle {
    struct GdObj header;
    struct ObjShape *shapePtr;     // looks like a shape...
    struct GdVec3f pos;    // some kind of position - relative? world?
    f32 unk30;
    struct GdVec3f unk38;
    f32 unk44; //not vec?
    f32 unk48; //not vec?
    s32 id;
    u32 flags;   // "dflags"?
    s32 colourNum;
    s32 timeout;  // when this reaches zero, the particle disappears
    s32 unk60;   //type?
    s32 unk64;   //type? (1 = has 50 sub-particles, 2,3 = has 30 sub-particles
    struct ObjGroup *subParticlesGrp;   // group of other Particles ?
    s32 unk74;
    u8 unk78[4];
    struct ObjAnimator *unk7C;   // guessing on type; doesn't seem to be used in final code
    struct ObjLight *unk80;  // could be a Net or Light; not seen as non-null in running code
    s32 unkB0;   //state?
    struct ObjGroup *attachedObjsGrp;  // attach group? unused group of particles
    s32 attachFlags;   //attached arg0; "AttFlag"
    struct GdObj *attachedToObj; // object that this object is attached to. looks like can be a Light or Camera
};

/**
 * An object that represents the visual portion of a 3D model.
 */
struct ObjShape {
    struct GdObj header;
    struct ObjShape *prevShape;
    struct ObjShape *nextShape;
    struct ObjGroup *faceGroup;  /* face group; based on get_3DG1_shape */
    struct ObjGroup *vtxGroup;  /* vtx group; based on get_3DG1_shape */
    struct ObjGroup *scaledVtxGroup; /* group for type 2 shapenets only ? vertices whose scaleFactor is not 1 get put into this group. */
    struct ObjGroup *mtlGroup;  /* what does this group do? materials? */
    s32 unk30;
    s32 faceCount;   /* face count? based on get_3DG1_shape */
    s32 vtxCount;   /* vtx count? based on get_3DG1_shape */
    s32 unk3C; // bool? if FALSE, then draw_shape_faces(shape)
    u32 id;
    s32 flag; // what are the flag values? only from dynlists?
    s32 dlNums[2];  // gd dl number for each frame buffer (??) [0, 1]
               s32 unk50;      // frame number (index into dlNums)?
    f32 alpha;       // paramF? opacitiy? something with rendertype
    char name[0x40];
};

/* 0x44 Flag Values
 * 0x01 -
 * 0x10 - Use vtx position as vtx normal? (`chk_shapegen`)
 */

/* netTypes
 * 0 - default?
 * 1 - shape net
 * 2 - something about the shape unk24 group having vertices too?
 * 3 - joints?
 * 4 - dynamic net? bone net?
 * 5 - particle net?
 * 6 - stub
 * 7 -
 */

struct ObjNet {
    struct GdObj header;
    struct GdVec3f worldPos;   // position? d_set_initpos + dSetWorldPos; print_net says world
    struct GdVec3f initPos;   // position? d_set_initpos? attached offset? dynamic? scratch?
    s32 flags;       // "dflags"?
    u32 id;      // some sort of id? from move_net
    s32 unk3C;      // state flags? vertex info flags?
    s32 colourNum;
    struct GdVec3f unusedForce;   // "force" (unused)
    struct GdVec3f velocity;
    struct GdVec3f rotation;
    struct GdVec3f unk68;   //initial rotation?
    struct GdVec3f collDisp;   // what is this?
    struct GdVec3f collTorque;   // what is this?
    struct GdVec3f unusedCollTorqueL;   // unused
    struct GdVec3f unusedCollTorqueD;   // unused
    struct GdVec3f torque;   // torque
    struct GdVec3f centerOfGravity;   // "CofG" center of gravity?
    struct GdBoundingBox boundingBox;
    struct GdVec3f unusedCollDispOff;   // unused
    f32 unusedCollMaxD;              // unused
    f32 maxRadius;
    Mat4f matE8;
    Mat4f mat128;
    Mat4f mat168;             // "rotation matrix"
    struct ObjShape *shapePtr;
    struct GdVec3f scale;
    f32 unusedMass;              // unused
    s32 numModes;              // unused
    struct ObjGroup *unk1C0;    // group of `ObjVertex` or `ObjParticle`
    struct ObjGroup *skinGrp;   // SkinGroup (from reset_weight) (joints and bones)
    struct ObjGroup *unk1C8;    // "node group" (joints, weights?)
    struct ObjGroup *unk1CC;    // plane group (only type 1?)
    struct ObjGroup *unk1D0;    // vertex group
    struct ObjGroup *attachedObjsGrp;
    struct GdVec3f attachOffset;
    s32 attachFlags;                 // d_attach_to arg 0; "AttFlag"
    struct GdObj *attachedToObj;   // object that this object is attached to
    s32 netType;    // from move_net
    struct ObjNet *unk1F0;  // or joint. guess from Unknown80192AD0
    struct GdVec3f unk1F4;
    struct GdVec3f unk200;
    struct ObjGroup *unk20C;
    s32 ctrlType;     // has no purpose
    struct ObjGroup *unk21C;
};

struct ObjPlane {
    struct GdObj header;
    u32 id;
    s32 unk18; //bool;  contained within zone? (from its parent Net?)
    f32 unk1C;
    s32 unk20;
    s32 unk24;
    struct GdBoundingBox boundingBox;
    struct ObjFace* unk40;
};

struct ObjVertex {
    struct GdObj header;
    struct GdVec3f initPos;
    struct GdVec3f pos;     // new position after being moved by joints?
    struct GdVec3f normal;  // normal? also color (like gbi?)
    s16 id;
    f32 scaleFactor;
    f32 alpha;
    struct VtxLink *gbiVerts;
};

/**
 * An object that represents a face in an `ObjShape`. It connects 3 or 4 vertices.
 */
struct ObjFace {
    struct GdObj header;
    struct GdColour colour;
    s32 colourNum;                     // "colour" index
    struct GdVec3f normal;
    s32 vtxCount;
    struct ObjVertex *vertices[4];  // these can also be s32 indices? which are then replaced by `find_thisface_verts`
    s32 mtlId; // from compressed GdFaceData; -1 == coloured face?
    struct ObjMaterial *mtl; // initialize to NULL; set by `map_face_materials` from mtlId
};

#define CAMERA_FLAG_CONTROLLABLE 0x4

struct ObjCamera {
    struct GdObj header;
    struct GdVec3f worldPos;   // position vec? from d_set_initpos
    struct ObjCamera* prev;
    struct ObjCamera* next;
    s32 id;
    s32 flags;   // flag of some sort
    struct GdObj* unk30;   // pointer to some type of object
    struct GdVec3f lookAt;  // point that the camera faces
    struct GdVec3f unk40;   // relative position related?
    struct GdVec3f unk4C;
    f32 unk58;      // GdVec3f ?
    f32 unk60;
    Mat4f unk64;    //matrix4x4
    f32 unkA4;
    Mat4f unkA8;    //matrix4x4
    Mat4f unkE8;
    struct GdVec3f unk128;  //possibly
    struct GdVec3f unk134;
    struct GdVec3f zoomPositions[4]; // zoom positions (*1, *1.5, *2, empty fourth)
    s32 maxZoomLevel; // max number of zoom positions
    s32 zoomLevel; // index into zoomPositions array
    f32 unk178;
    f32 unk17C;
    struct GdVec3f unk180;
    struct ObjView *unk18C; // view that has/is using this camera?
};

enum GdMtlTypes {
    GD_MTL_STUB_DL = 0x01,
    GD_MTL_BREAK = 0x04,
    GD_MTL_SHINE_DL = 0x10,
    GD_MTL_TEX_OFF = 0x20,
    GD_MTL_LIGHTS = 0x40 // uses default case
};

struct ObjMaterial {
    struct GdObj header;
    s32 id;
    char name[8];
    s32 type;
    struct GdColour Ka;  // ambient color
    struct GdColour Kd;  // diffuse color
    void *texture; //set by d_usetexture; never seems to be non-null though.
    s32 gddlNumber;
};

struct ObjWeight {
    struct GdObj header;
    s32 vtxId;  // ID of vertex that this weight applies to
    struct GdVec3f vec20;    // based on func_80181894? maybe a GdBoundingBox?
    f32 weightVal; // weight (unit?)
    struct ObjVertex* vtx;
};

/* This union is used in ObjGadget for a variable typed field.
** The type can be found by checking group unk4C */
union ObjVarVal {
    s32 i;
    f32 f;
    u64 l;
};

/*
 * A slider control used to adjust the value of a variable
 */
struct ObjGadget {
    struct GdObj header;
    struct GdVec3f worldPos;    // "world" position vec?
    s32 unk20;  // unused; only ever set to 0
    s32 type;
    f32 sliderPos;  // position of the slider (from 0 to 1)
    union ObjVarVal varval; //retype and rename varval30
    f32 rangeMin;
    f32 rangeMax;
    struct GdVec3f size;   // size (x = width, y = height)
    struct ObjGroup *valueGrp;  // group containing `ObjValPtr`s controlled by this gadget 
    struct ObjShape *shapePtr;
    struct ObjGroup *unk54;  //node group?
    s32 colourNum;
};

enum GdViewFlags {
    VIEW_2_COL_BUF      = 0x000008,
    VIEW_ALLOC_ZBUF     = 0x000010,
    VIEW_SAVE_TO_GLOBAL = 0x000040,
    VIEW_DEFAULT_PARENT = 0x000100,
    VIEW_BORDERED       = 0x000400,
    VIEW_UPDATE         = 0x000800,
    VIEW_UNK_1000       = 0x001000, // used in setup_view_buffers
    VIEW_UNK_2000       = 0x002000, // only see together with 0x4000
    VIEW_UNK_4000       = 0x004000,
    VIEW_COLOUR_BUF     = 0x008000,
    VIEW_Z_BUF          = 0x010000,
    VIEW_1_CYCLE        = 0x020000,
    VIEW_MOVEMENT       = 0x040000,
    VIEW_DRAW           = 0x080000,
    VIEW_WAS_UPDATED    = 0x100000,
    VIEW_LIGHT          = 0x200000
};

struct ObjView {
    struct GdObj header;
    s32 unk1C; // set as nonexistent return of `setup_view_buffers`
    s32 id;
    struct ObjCamera *activeCam; // is this really active?
    struct ObjGroup *components; // camera + joints + nets, etc..?
    struct ObjGroup *lights;     // only lights?
    struct GdObj *pickedObj; // selected with cursor (`update_view`)
    enum GdViewFlags flags;
    s32 projectionType; // enum? if 1 use guPerspective, if 0 (or 2?) use guOrtho (see `gdDrawView`)
    struct GdVec3f upperLeft; // position vec?
    f32 unk48; // what are these? are they another vec?
    f32 unk4C;
    struct GdVec3f lowerRight;
    struct GdVec3f clipping; // z-coordinate of (x: near, y: far) clipping plane?
    const char *namePtr;
    s32 gdDlNum;
    s32 unk74;
    s32 unk78;
    struct GdColour colour;
    struct ObjView *parent; // maybe not a true parent, but link to buffers in parent?
    void *zbuf;
    void *colourBufs[2]; // frame buffers?
    void (*proc)(struct ObjView *);   // Never non-null in game...?
    s32 unk9C;
};


typedef union ObjVarVal * (*valptrproc_t)(union ObjVarVal *, union ObjVarVal);

struct ObjLabel {
    struct GdObj header;
    struct GdVec3f position;
    char *fmtstr;  // format string for displaying the value contained in `valptr`
    s32 unk24;  // always 8
    struct ObjValPtr *valptr;
    valptrproc_t valfn;
    s32 unk30;       // set to 3 or 4 in the code, but never actually used. could possibly be colourNum?
};

/* unk30 types:
 * 3 = f32? f32 pointer?
**/

struct ObjAnimator {
    struct GdObj header;
    struct ObjGroup* animatedPartsGrp;  // group containing objects animated by this animator. I think all of them are joints.
    struct ObjGroup* animdataGrp;  //animation data? a group, but the link points to something weird..
    s32 animSeqNum;  // which entry in the AnimDataInfo array to use
    f32 unk24;
    f32 frame;  // key frame number. This is a float so that it can interpolate between key frames, though I think in practice, it's always a whole number.
    struct ObjGroup *attachedObjsGrp;
    s32 attachFlags;   //attach arg0, not used
    struct GdObj* attachedToObj;   // object that this object is attached to. Normally another Objanimator?
    void (*controlFunc) (struct ObjAnimator *);  // function that "controls" the animation sequence by choosing the frame number
    s32 state;   //state enum?
    s32 nods;  // Counts the number of nods when Mario is dozing off. When this reaches zero, he wakes up again
    s32 stillTimer;  // number of frames to remain in the part where Mario's head stays still with his eyes following the cursor
};

/* Animation Data Types */
enum GdAnimations {
    GD_ANIM_EMPTY                 = 0,  // Listed types are how the data are arranged in memory; maybe not be exact type
    GD_ANIM_MTX4x4                = 1,  // f32[4][4]
    GD_ANIM_SCALE3F_ROT3F_POS3F   = 2,  // f32[3][3]
    GD_ANIM_SCALE3S_POS3S_ROT3S   = 3,  // s16[9]
    GD_ANIM_SCALE3F_ROT3F_POS3F_2 = 4,  // f32[3][3]
    GD_ANIM_STUB                  = 5,
    GD_ANIM_ROT3S                 = 6,  // s16[3]
    GD_ANIM_POS3S                 = 7,  // s16[3]
    GD_ANIM_ROT3S_POS3S           = 8,  // s16[6]
    GD_ANIM_MTX4x4F_SCALE3F       = 9,  // {f32 mtx[4][4]; f32 vec[3];}
    GD_ANIM_CAMERA_EYE3S_LOOKAT3S = 11  // s16[6]
};
/* This struct is pointed to by the `obj` field in ListNode struct in the `animdata` ObjGroup */
struct AnimDataInfo {
    s32 count;  // count or -1 for end of array of AnimDataInfo structures
    enum GdAnimations type;  // types are used in "move_animator"
    void* data; // points to an array of `type` data
};
/* GD_ANIM_MTX4x4F_SCALE3F (9) type */
struct AnimMtxVec {
    Mat4f matrix;
    struct GdVec3f vec;  // seems to be a scale vec
};

enum ValPtrType {
    OBJ_VALUE_INT   = 1,
    OBJ_VALUE_FLOAT = 2
};

/**
 * An object that points to a value in memory (either a field of another object,
 * or a standalone variable). Used by `ObjLabel` and `ObjGadget` to manage their
 * values.
 */
struct ObjValPtr {
    struct GdObj header;
    struct GdObj *obj;   // maybe just a void *?
    uintptr_t offset;  // value pointed to is `obj` + `offset`
    enum ValPtrType datatype;
    s32 flag;       // TODO: better name for this? If 0x40000, then `offset` is an offset to a field in `obj`. Otherwise, `obj` is NULL, and `offset` is the address of a variable. 
};

enum GdLightFlags {
    LIGHT_UNK02 = 0x02, // old type of light?
    LIGHT_NEW_UNCOUNTED = 0x10,
    LIGHT_UNK20 = 0x20 // new, actually used type of light? used for phong shading?
};

struct ObjLight {
    struct GdObj header;
    s32 id;
    char name[8];
    s32 flags;
    f32 unk30;       // color (5C) = Kd (50) * 30
    f32 unk38; // calculated diffuse theta (in degrees?)
    s32 unk3C;
    s32 unk40;
    s32 unk4C;
    struct GdColour diffuse;
    struct GdColour colour;
    struct GdVec3f unk68;
    struct GdVec3f position;
    struct GdVec3f unk80;
    struct GdVec3f unk8C;
    s32 unk98;
    struct ObjShape *unk9C;
};

struct ObjZone {
    struct GdObj header;
    struct GdBoundingBox boundingBox;
    struct ObjGroup *unk2C;   // plane group?
    struct ObjGroup *unk30;   // guess based on Unknown801781DC; might have to change later
};

struct ObjUnk200000 {
    struct GdObj header;
    struct ObjVertex *unk30; //not sure; guessing for Unknown801781DC; 30 and 34 could switch with ObjZone
    struct ObjFace *unk34;   //not sure; guessing for Unknown801781DC
};

#endif // GD_TYPES_H
