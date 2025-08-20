#include <PR/ultratypes.h>

#if defined(VERSION_JP) || defined(VERSION_US)
#include "prevent_bss_reordering.h"
#endif

#include "debug_utils.h"
#include "draw_objects.h"
#include "dynlist_proc.h"
#include "dynlists/dynlist_macros.h"
#include "dynlists/dynlists.h"
#include "gd_main.h"
#include "gd_math.h"
#include "gd_types.h"
#include "joints.h"
#include "macros.h"
#include "objects.h"
#include "particles.h"
#include "renderer.h"
#include "shape_helper.h"
#include "skin.h"

// data
struct ObjGroup *gMarioFaceGrp = NULL;
struct ObjShape *gSpotShape = NULL;        // Shape used for drawing lights?
static struct ObjShape *sGrabJointTestShape = NULL; // Test shape for showing grab joints. This isn't rendered due to make_grabber_joint setting the drawFlags to OBJ_INVISIBLE.
struct ObjShape *gShapeRedSpark = NULL;
struct ObjShape *gShapeSilverSpark = NULL;
struct ObjShape *gShapeRedStar = NULL;
struct ObjShape *gShapeSilverStar = NULL;

struct ObjShape *sSimpleShape = NULL;

static f64 D_801A8668 = 0.0;

// bss
static char sGdLineBuf[0x100];
static s32 sGdLineBufCsr;
static struct GdFile *sGdShapeFile;
static struct ObjShape *sGdShapeListHead;
static u32 sGdShapeCount;
static struct GdVec3f D_801BAC60;
static struct ObjPlane *D_801BAC74;
static struct ObjPlane *D_801BAC78; // sShapeNetHead?
static struct ObjFace *D_801BAC9C;
static struct ObjFace *D_801BACA0;
/// factor for scaling vertices in an `ObjShape` when calling `scale_verts_in_shape()`
static struct GdVec3f sVertexScaleFactor;
/// factor for translating vertices in an `ObjShape` when calling `translate_verts_in_shape()`
static struct GdVec3f sVertexTranslateOffset;
static struct ObjGroup *D_801BAD08; // group of planes from make_netfromshape
static struct GdVec3f sShapeCenter;   // printed with "c="

// Forward Declarations
struct ObjMaterial *find_or_add_new_mtl(struct ObjGroup *, s32, f32, f32, f32);

/* Something to do with shape list/group initialization? */
void gdResetShapeHelper(void) {
    sGdShapeCount = 0;
    sGdShapeListHead = NULL;
    gGdLightGroup = make_group(0);
}

/**
 * Computes the normal vector for a face based on three of its vertices.
 */
void calc_face_normal(struct ObjFace *face) {
    struct GdVec3f p1;
    struct GdVec3f p2;
    struct GdVec3f p3;
    struct GdVec3f normal;
    struct ObjVertex *vtx1;
    struct ObjVertex *vtx2;
    struct ObjVertex *vtx3;
    f32 mul = 1000.0f;

    imin("calc_facenormal");

    if (face->vtxCount >= 3) {  // need at least three points to compute a normal
        vtx1 = face->vertices[0];
        p1.x = vtx1->pos.x;
        p1.y = vtx1->pos.y;
        p1.z = vtx1->pos.z;

        vtx2 = face->vertices[1];
        p2.x = vtx2->pos.x;
        p2.y = vtx2->pos.y;
        p2.z = vtx2->pos.z;

        vtx3 = face->vertices[2];
        p3.x = vtx3->pos.x;
        p3.y = vtx3->pos.y;
        p3.z = vtx3->pos.z;

        // calculate the cross product of edges (p2 - p1) and (p3 - p2)
        // not sure why each component is multiplied by 1000. maybe to avoid loss of precision when normalizing? 
        normal.x = (((p2.y - p1.y) * (p3.z - p2.z)) - ((p2.z - p1.z) * (p3.y - p2.y))) * mul;
        normal.y = (((p2.z - p1.z) * (p3.x - p2.x)) - ((p2.x - p1.x) * (p3.z - p2.z))) * mul;
        normal.z = (((p2.x - p1.x) * (p3.y - p2.y)) - ((p2.y - p1.y) * (p3.x - p2.x))) * mul;

        gd_normalize_vec3f(&normal);

        face->normal.x = normal.x;
        face->normal.y = normal.y;
        face->normal.z = normal.z;
    }
    imout();
}

struct ObjVertex *gd_make_vertex(f32 x, f32 y, f32 z) {
    struct ObjVertex *vtx;

    vtx = (struct ObjVertex *) make_object(OBJ_TYPE_VERTICES);
    vtx->id = GDCMD_BEGIN_LIST;

    vtx->pos.x = x;
    vtx->pos.y = y;
    vtx->pos.z = z;

    vtx->initPos.x = x;
    vtx->initPos.y = y;
    vtx->initPos.z = z;

    vtx->scaleFactor = 1.0f;
    vtx->gbiVerts = NULL;
    vtx->alpha = 1.0f;

    vtx->normal.x = 0.0f;
    vtx->normal.y = 1.0f;
    vtx->normal.z = 0.0f;

    return vtx;
}

struct ObjFace *make_face_with_colour(f32 r, f32 g, f32 b) {
    struct ObjFace *newFace;

    imin("make_face");
    newFace = (struct ObjFace *) make_object(OBJ_TYPE_FACES);

    newFace->colour.r = r;
    newFace->colour.g = g;
    newFace->colour.b = b;

    newFace->vtxCount = 0;
    newFace->mtlId = -1;
    newFace->mtl = NULL;

    imout();
    return newFace;
}

struct ObjFace *make_face_with_material(struct ObjMaterial *mtl) {
    struct ObjFace *newFace;

    newFace = (struct ObjFace *) make_object(OBJ_TYPE_FACES);

    newFace->vtxCount = 0;
    newFace->mtlId = mtl->id;
    newFace->mtl = mtl;

    return newFace;
}

void add_4_vertices_to_face(struct ObjFace *face, struct ObjVertex *vtx1, struct ObjVertex *vtx2,
                     struct ObjVertex *vtx3, struct ObjVertex *vtx4) {
    face->vertices[0] = vtx1;
    face->vertices[1] = vtx2;
    face->vertices[2] = vtx3;
    face->vertices[3] = vtx4;
    face->vtxCount = 4;
    calc_face_normal(face);
}

void add_3_vtx_to_face(struct ObjFace *face, struct ObjVertex *vtx1, struct ObjVertex *vtx2,
                       struct ObjVertex *vtx3) {
    face->vertices[0] = vtx1;
    face->vertices[1] = vtx2;
    face->vertices[2] = vtx3;
    face->vtxCount = 3;
    calc_face_normal(face);
}

/**
 * Creates an `ObjShape` object
 */
struct ObjShape *make_shape(s32 flag, const char *name) {
    struct ObjShape *newShape;
    struct ObjShape *curShapeHead;

    newShape = (struct ObjShape *) make_object(OBJ_TYPE_SHAPES);

    if (name != NULL) {
        gd_strcpy(newShape->name, name);
    } else {
        gd_strcpy(newShape->name, "NoName");
    }

    sGdShapeCount++;

    curShapeHead = sGdShapeListHead;
    sGdShapeListHead = newShape;

    if (curShapeHead != NULL) {
        newShape->nextShape = curShapeHead;
        curShapeHead->prevShape = newShape;
    }

    newShape->id = sGdShapeCount;
    newShape->flag = flag;

    newShape->vtxCount = 0;
    newShape->faceCount = 0;
    newShape->dlNums[0] = 0;
    newShape->dlNums[1] = 0;
    newShape->unk3C = 0;
    newShape->faceGroup = NULL; /* whoops, NULL-ed twice */

    newShape->alpha = 1.0f;

    newShape->vtxGroup = NULL;
    newShape->faceGroup = NULL;
    newShape->mtlGroup = NULL;
    newShape->unk30 = 0;
    newShape->unk50 = 0;

    return newShape;
}

void clear_buf_to_cr(void) {
    sGdLineBufCsr = 0;
    sGdLineBuf[sGdLineBufCsr] = '\r';
}

s8 get_current_buf_char(void) {
    return sGdLineBuf[sGdLineBufCsr];
}

s8 get_and_advance_buf(void) {
    if (get_current_buf_char() == '\0') {
        return '\0';
    }

    return sGdLineBuf[sGdLineBufCsr++];
}

s8 load_next_line_into_buf(void) {
    sGdLineBufCsr = 0;

    if (gd_feof(sGdShapeFile) != 0) {
        sGdLineBuf[sGdLineBufCsr] = '\0';
    } else {
        gd_fread_line(sGdLineBuf, 0xFF, sGdShapeFile);
    }

    return get_current_buf_char();
}

s32 is_line_end(char c) {
    return c == '\r' || c == '\n';
}

s32 is_white_space(char c) {
    return c == ' ' || c == '\t';
}

/* Advances buffer cursor to next non-white-space character, if possible.
 * Returns TRUE if a character is found, or FALSE if EOF or \0 */
s32 scan_to_next_non_whitespace(void) {
    char curChar;

    for (curChar = get_current_buf_char(); curChar != '\0'; curChar = get_current_buf_char()) {
        if (is_white_space(curChar)) {
            get_and_advance_buf();
            continue;
        }

        if (curChar == '\x1a') { //'SUB' character: "soft EOF" in older systems
            return FALSE;
            continue; // unreachable
        }

        if (is_line_end(curChar)) {
            if (load_next_line_into_buf() == '\0') {
                return FALSE;
            }
        } else {
            break;
        }
    }

    return !!curChar;
}

s32 is_next_buf_word(char *a0) {
    char curChar;
    char wordBuf[0xfc];
    u32 bufLength;

    bufLength = 0;
    for (curChar = get_and_advance_buf(); curChar != '\0'; curChar = get_and_advance_buf()) {
        if (is_white_space(curChar) || is_line_end(curChar)) {
            break;
            continue; // unreachable + nonsensical
        }
        wordBuf[bufLength] = curChar;
        bufLength++;
    }

    wordBuf[bufLength] = '\0';

    return !gd_str_not_equal(a0, wordBuf);
}

s32 getfloat(f32 *floatPtr) {
    char charBuf[0x100];
    u32 bufCsr;
    char curChar;
    u32 sp34;
    f64 parsedDouble;

    imin("getfloat");

    if (is_line_end(get_current_buf_char())) {
        fatal_printf("getfloat(): Unexpected EOL");
    }

    while (is_white_space(get_current_buf_char())) {
        get_and_advance_buf();
    }

    bufCsr = 0;

    for (curChar = get_and_advance_buf(); curChar != '\0'; curChar = get_and_advance_buf()) {
        if (!is_white_space(curChar) && !is_line_end(curChar)) {
            charBuf[bufCsr] = curChar;
            bufCsr++;
        } else {
            break;
        }
    }

    charBuf[bufCsr] = '\0';

    parsedDouble = gd_lazy_atof(charBuf, &sp34);
    *floatPtr = (f32) parsedDouble;

    imout();
    return !!bufCsr;
}

s32 getint(s32 *intPtr) {
    char charBuf[0x100];
    u32 bufCsr;
    char curChar;

    imin("getint");

    if (is_line_end(get_current_buf_char())) {
        fatal_printf("getint(): Unexpected EOL");
    }

    while (is_white_space(get_current_buf_char())) {
        get_and_advance_buf();
    }

    bufCsr = 0;
    for (curChar = get_and_advance_buf(); curChar != '\0'; curChar = get_and_advance_buf()) {
        if (is_white_space(curChar) || is_line_end(curChar)) {
            break;
        }

        charBuf[bufCsr] = curChar;
        bufCsr++;
    }

    charBuf[bufCsr] = '\0';
    *intPtr = gd_atoi(charBuf);

    imout();
    return !!bufCsr;
}

void func_8019807C(struct ObjVertex *vtx) {
    gd_rot_2d_vec(D_801BAC60.x, &vtx->pos.y, &vtx->pos.z);
    gd_rot_2d_vec(D_801BAC60.y, &vtx->pos.x, &vtx->pos.z);
    gd_rot_2d_vec(D_801BAC60.z, &vtx->pos.x, &vtx->pos.y);
}

void func_801980E8(f32 *a0) {
    gd_rot_2d_vec(D_801BAC60.x, &a0[1], &a0[2]);
    gd_rot_2d_vec(D_801BAC60.y, &a0[0], &a0[2]);
    gd_rot_2d_vec(D_801BAC60.z, &a0[0], &a0[1]);
}

void Unknown80198154(f32 x, f32 y, f32 z) {
    D_801BAC60.x = x;
    D_801BAC60.y = y;
    D_801BAC60.z = z;
}

void Unknown80198184(struct ObjShape *shape, f32 x, f32 y, f32 z) {
    UNUSED struct GdVec3f unusedVec;
    unusedVec.x = x;
    unusedVec.y = y;
    unusedVec.z = z;

    apply_to_obj_types_in_group(OBJ_TYPE_VERTICES, (applyproc_t) func_8019807C, shape->vtxGroup);
}

void scale_obj_position(struct GdObj *obj) {
    struct GdVec3f pos;

    if (obj->type == OBJ_TYPE_GROUPS) {
        return;
    }

    set_cur_dynobj(obj);
    dGetRelPos(&pos);

    pos.x *= sVertexScaleFactor.x;
    pos.y *= sVertexScaleFactor.y;
    pos.z *= sVertexScaleFactor.z;

    dSetRelativePosition(pos.x, pos.y, pos.z);
    dSetInitPos(pos.x, pos.y, pos.z);
}

void translate_obj_position(struct GdObj *obj) {
    struct GdVec3f pos;

    set_cur_dynobj(obj);
    dGetRelPos(&pos);

    pos.x += sVertexTranslateOffset.x;
    pos.y += sVertexTranslateOffset.y;
    pos.z += sVertexTranslateOffset.z;

    dSetRelativePosition(pos.x, pos.y, pos.z);
}

void scale_verts_in_shape(struct ObjShape *shape, f32 x, f32 y, f32 z) {
    sVertexScaleFactor.x = x;
    sVertexScaleFactor.y = y;
    sVertexScaleFactor.z = z;

    if (shape->vtxGroup != NULL) {
        apply_to_obj_types_in_group(OBJ_TYPE_ALL, (applyproc_t) scale_obj_position, shape->vtxGroup);
    }
}

// Guessing on the type of a0
void translate_verts_in_shape(struct ObjShape *shape, f32 x, f32 y, f32 z) {
    sVertexTranslateOffset.x = x;
    sVertexTranslateOffset.y = y;
    sVertexTranslateOffset.z = z;

    apply_to_obj_types_in_group(OBJ_TYPE_ALL, (applyproc_t) translate_obj_position, shape->vtxGroup);
}

void Unknown80198444(struct ObjVertex *vtx) {
    f64 distance;

    add_obj_pos_to_bounding_box(&vtx->header);

    distance = vtx->pos.x * vtx->pos.x + vtx->pos.y * vtx->pos.y + vtx->pos.z * vtx->pos.z;

    if (distance != 0.0) {
        distance = gd_sqrt_d(distance); // sqrtd?

        if (distance > D_801A8668) {
            D_801A8668 = distance;
        }
    }
}

void Unknown80198524(struct ObjVertex *vtx) {
    vtx->pos.x -= sShapeCenter.x;
    vtx->pos.y -= sShapeCenter.y;
    vtx->pos.z -= sShapeCenter.z;

    vtx->pos.x /= D_801A8668;
    vtx->pos.y /= D_801A8668;
    vtx->pos.z /= D_801A8668;
}

void Unknown801985E8(struct ObjShape *shape) {
    struct GdBoundingBox bbox;

    D_801A8668 = 0.0;
    reset_bounding_box();
    apply_to_obj_types_in_group(OBJ_TYPE_VERTICES, (applyproc_t) Unknown80198444, shape->vtxGroup);

    get_some_bounding_box(&bbox);

    sShapeCenter.x = (f32)((bbox.minX + bbox.maxX) / 2.0); //? 2.0f
    sShapeCenter.y = (f32)((bbox.minY + bbox.maxY) / 2.0); //? 2.0f
    sShapeCenter.z = (f32)((bbox.minZ + bbox.maxZ) / 2.0); //? 2.0f

    gd_print_vec("c=", &sShapeCenter);

    apply_to_obj_types_in_group(OBJ_TYPE_VERTICES, (applyproc_t) Unknown80198524, shape->vtxGroup);
}

void get_OBJ_shape(struct ObjShape *shape) {
    struct GdColour faceClr;
    s32 curFaceVtx;
    s32 faceVtxIndex;
    struct GdVec3f tempVec;
    struct ObjFace *newFace;
    struct ObjVertex *vtxArr[4000];
    struct ObjFace *faceArr[4000];
    s32 faceCount = 0;
    s32 vtxCount = 0;

    faceClr.r = 1.0f;
    faceClr.g = 0.5f;
    faceClr.b = 1.0f;

    sGdLineBufCsr = 0;

    while (scan_to_next_non_whitespace()) {
        switch (get_and_advance_buf()) {
            case 'v':
                getfloat(&tempVec.x);
                getfloat(&tempVec.y);
                getfloat(&tempVec.z);

                vtxArr[vtxCount] = gd_make_vertex(tempVec.x, tempVec.y, tempVec.z);
                func_8019807C(vtxArr[vtxCount]);
                vtxCount++;

                if (vtxCount >= 4000) {
                    fatal_printf("Too many vertices in shape data");
                }

                shape->vtxCount++;
                break;

            case 'f':
                newFace = make_face_with_colour(faceClr.r, faceClr.g, faceClr.b);
                faceArr[faceCount] = newFace;
                faceCount++;

                if (faceCount >= 4000) {
                    fatal_printf("Too many faces in shape data");
                }

                curFaceVtx = 0;
                while (get_current_buf_char() != '\0') {
                    getint(&faceVtxIndex);

                    if (curFaceVtx > 3) {
                        fatal_printf("Too many points in a face(%d)", curFaceVtx);
                    }

                    /* .obj vertex list is 1-indexed */
                    newFace->vertices[curFaceVtx] = vtxArr[faceVtxIndex - 1];
                    curFaceVtx++;

                    if (is_line_end(get_current_buf_char())) {
                        break;
                    }
                }

                /* These are already set by make_face_with_colour... */
                newFace->colour.r = faceClr.r;
                newFace->colour.g = faceClr.g;
                newFace->colour.b = faceClr.b;

                newFace->vtxCount = curFaceVtx;

                if (newFace->vtxCount > 3) {
                    fatal_printf("Too many points in a face(%d)", newFace->vtxCount);
                }

                calc_face_normal(newFace);

                shape->faceCount++;
                break;

            case 'g':
                break;
            case '#':
                break;
            default:
                break;
        }

        clear_buf_to_cr();
    }

    shape->vtxGroup = make_group_of_type(OBJ_TYPE_VERTICES, (struct GdObj *) vtxArr[0], NULL);
    shape->faceGroup = make_group_of_type(OBJ_TYPE_FACES, (struct GdObj *) faceArr[0], NULL);
}

struct ObjGroup *group_faces_in_mtl_grp(struct ObjGroup *mtlGroup, struct GdObj *fromObj,
                                        struct GdObj *toObj) {
    struct ObjMaterial *curObjAsMtl;
    struct ObjGroup *newGroup;
    struct GdObj *curObj;
    register struct ListNode *node;
    struct GdObj *curLinkedObj;

    newGroup = make_group(0);

    for (node = mtlGroup->firstMember; node != NULL; node = node->next) {
        curLinkedObj = node->obj;
        curObjAsMtl = (struct ObjMaterial *) curLinkedObj;

        curObj = fromObj;
        while (curObj != NULL) {
            if (curObj == toObj) {
                break;
            }

            if (curObj->type == OBJ_TYPE_FACES) {
                if (((struct ObjFace *) curObj)->mtl == curObjAsMtl) {
                    addto_group(newGroup, curObj);
                }
            }
            curObj = curObj->prev;
        }
    }

    return newGroup;
}

struct ObjMaterial *find_or_add_new_mtl(struct ObjGroup *group, UNUSED s32 a1, f32 r, f32 g, f32 b) {
    struct ObjMaterial *newMtl;
    register struct ListNode *node;
    struct ObjMaterial *foundMtl;

    for (node = group->firstMember; node != NULL; node = node->next) {
        foundMtl = (struct ObjMaterial *) node->obj;

        if (foundMtl->header.type == OBJ_TYPE_MATERIALS) {
            if (foundMtl->Kd.r == r) {
                if (foundMtl->Kd.g == g) {
                    if (foundMtl->Kd.b == b) {
                        return foundMtl;
                    }
                }
            }
        }
    }

    newMtl = make_material(0, NULL, 1);
    set_cur_dynobj((struct GdObj *)newMtl);
    dSetDiffuse(r, g, b);
    addto_group(group, (struct GdObj *) newMtl);

    return newMtl;
}

struct ObjShape *make_grid_shape(enum ObjTypeFlag gridType, s32 a1, s32 a2, s32 a3, s32 a4) {
    void *objBuf[32][32]; // vertex or particle depending on gridType
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp5C;
    s32 parI;
    s32 row;
    s32 col;
    UNUSED s32 sp4C = 0;
    struct ObjShape *gridShape;
    f32 sp44;
    struct ObjFace *sp40 = NULL;  // first made shape?
    struct ObjGroup *parOrVtxGrp; // group of made particles or vertices (based on gridType)
    struct ObjGroup *mtlGroup;
    struct GdVec3f *sp30;     // GdVec3f* ? from gd_get_colour
    struct GdVec3f *sp2C;     //^
    struct ObjMaterial *mtl1; // first made material
    struct ObjMaterial *mtl2; // second made material

    sp30 = (struct GdVec3f *) gd_get_colour(a1);
    sp2C = (struct GdVec3f *) gd_get_colour(a2);

    mtl1 = make_material(0, NULL, 1);
    set_cur_dynobj((struct GdObj *) mtl1);
    dSetDiffuse(sp30->x, sp30->y, sp30->z);
    mtl1->type = 0x40;

    mtl2 = make_material(0, NULL, 2);
    set_cur_dynobj((struct GdObj *) mtl2);
    dSetDiffuse(sp2C->x, sp2C->y, sp2C->z);
    mtl2->type = 0x40;

    mtlGroup = make_group(2, mtl1, mtl2);
    gridShape = make_shape(0, "grid");
    gridShape->faceCount = 0;
    gridShape->vtxCount = 0;

    sp44 = 2.0 / a3; //? 2.0f
    sp5C = -1.0f;
    sp6C = 0.0f;
    sp70 = -1.0f;

    for (col = 0; col <= a4; col++) {
        sp68 = sp5C;
        for (row = 0; row <= a3; row++) {
            gridShape->vtxCount++;
            if (gridType == OBJ_TYPE_VERTICES) {
                objBuf[row][col] = gd_make_vertex(sp68, sp6C, sp70);
            } else if (gridType == OBJ_TYPE_PARTICLES) {
                objBuf[row][col] = make_particle(0, 0, sp68, sp6C + 2.0f, sp70);
                ((struct ObjParticle *) objBuf[row][col])->unk44 = (1.0 + sp68) / 2.0;
                ((struct ObjParticle *) objBuf[row][col])->unk48 = (1.0 + sp70) / 2.0;
            }
            sp68 += sp44;
        }
        sp70 += sp44;
    }

    for (col = 0; col < a4; col++) {
        for (row = 0; row < a3; row++) {
            gridShape->faceCount += 2;
            if (a1 != a2) {
                if ((row + col) & 1) {
                    D_801BAC9C = make_face_with_material(mtl1);
                    D_801BACA0 = make_face_with_material(mtl1);
                } else {
                    D_801BAC9C = make_face_with_material(mtl2);
                    D_801BACA0 = make_face_with_material(mtl2);
                }
            } else {
                D_801BAC9C = make_face_with_material(mtl1);
                D_801BACA0 = make_face_with_material(mtl2);
            }

            if (sp40 == NULL) {
                sp40 = D_801BAC9C;
            }

            add_3_vtx_to_face(D_801BAC9C, objBuf[row][col + 1], objBuf[row + 1][col + 1],
                              objBuf[row][col]);
            add_3_vtx_to_face(D_801BACA0, objBuf[row + 1][col + 1], objBuf[row + 1][col],
                              objBuf[row][col]);
        }
    }

    if (gridType == OBJ_TYPE_PARTICLES) {
        for (parI = 0; parI <= a3; parI++) {
            ((struct ObjParticle *) objBuf[parI][0])->flags |= 2;
            ((struct ObjParticle *) objBuf[parI][a4])->flags |= 2;
        }

        for (parI = 0; parI <= a4; parI++) {
            ((struct ObjParticle *) objBuf[0][parI])->flags |= 2;
            ((struct ObjParticle *) objBuf[a3][parI])->flags |= 2;
        }
    }

    parOrVtxGrp = make_group_of_type(gridType, (struct GdObj *) objBuf[0][0], NULL);
    gridShape->vtxGroup = parOrVtxGrp;
    gridShape->mtlGroup = mtlGroup;

    gridShape->faceGroup = group_faces_in_mtl_grp(gridShape->mtlGroup, (struct GdObj *) sp40, NULL);

    printf("grid: points=%d, faces=%d\n", gridShape->vtxGroup->id, gridShape->faceGroup->id);
    return gridShape;
}

void Unknown80199E44(UNUSED s32 a0, struct GdObj *a1, struct GdObj *a2, UNUSED s32 a3) {
    UNUSED struct ObjGroup *sp1C = make_group(2, a1, a2);
}

void Unknown80199E88(struct ObjFace *face) {
    D_801BAC74 = make_plane(FALSE, face);

    if (D_801BAC78 == NULL) {
        D_801BAC78 = D_801BAC74;
    }
}

struct ObjNet *make_netfromshape(struct ObjShape *shape) {
    struct ObjNet *newNet;

    if (shape == NULL) {
        fatal_printf("make_netfromshape(): null shape ptr");
    }

    D_801BAC78 = NULL;
    apply_to_obj_types_in_group(OBJ_TYPE_FACES, (applyproc_t) Unknown80199E88, shape->faceGroup);
    D_801BAD08 = make_group_of_type(OBJ_TYPE_PLANES, (struct GdObj *) D_801BAC78, NULL);
    newNet = make_net(0, shape, NULL, D_801BAD08, shape->vtxGroup);
    newNet->netType = 1;

    return newNet;
}

/**
 * Controls the dizzy (game over) animation of Mario's head.
 */
void animate_mario_head_gameover(struct ObjAnimator *self) {
    switch (self->state) {
        case 0:
            self->frame = 1.0f;
            self->animSeqNum = 1;  // game over anim sequence
            self->state = 1;
            break;
        case 1:
            self->frame += 1.0f;
            // After the gameover animation ends, switch to the normal animation
            if (self->frame == 166.0f) {
                self->frame = 69.0f;
                self->state = 4;
                self->controlFunc = animate_mario_head_normal;
                self->animSeqNum = 0;  // normal anim sequence
            }
            break;
    }
}

/**
 * Controls the normal animation of Mario's head. This functions like a state machine.
 */
void animate_mario_head_normal(struct ObjAnimator *self) {
    s32 state = 0; // TODO: label these states
    s32 aBtnPressed = gGdCtrl.dragging;

    switch (self->state) {
        case 0:
            // initialize?
            self->frame = 1.0f;
            self->animSeqNum = 0;  // normal anim sequence
            state = 2;
            self->nods = 5;
            break;
        case 2:
            if (aBtnPressed) {
                state = 5;
            }

            self->frame += 1.0f;

            if (self->frame == 810.0f) {
                self->frame = 750.0f;
                self->nods--;
                if (self->nods == 0) {
                    state = 3;
                }
            }
            break;
        case 3:
            self->frame += 1.0f;

            if (self->frame == 820.0f) {
                self->frame = 69.0f;
                state = 4;
            }
            break;
        case 4:
            self->frame += 1.0f;

            if (self->frame == 660.0f) {
                self->frame = 661.0f;
                state = 2;
                self->nods = 5;
            }
            break;
        case 5:
            if (self->frame == 660.0f) {
                state = 7;
            } else if (self->frame > 660.0f) {
                self->frame -= 1.0f;
            } else if (self->frame < 660.0f) {
                self->frame += 1.0f;
            }

            self->stillTimer = 150;
            break;
        case 7:  // Mario is staying still while his eyes follow the cursor
            if (aBtnPressed) {
                self->stillTimer = 300;
            } else {
                self->stillTimer--;
                if (self->stillTimer == 0) {
                    state = 6;
                }
            }
            self->frame = 660.0f;
            break;
        case 6:
            state = 2;
            self->nods = 5;
            break;
    }

    if (state != 0) {
        self->state = state;
    }
}

/**
 * Loads the Mario head from `GODDARD_MAIN_FACE_SCENE`, sets up grabbers, and makes
 * sparkle particles
 */
s32 load_mario_head(void (*aniFn)(struct ObjAnimator *)) {
    struct ObjNet *sp54; // net made with sp48 group
    struct ObjGroup *sp48; // Joint group
    struct ObjGroup *mainShapesGrp;
    struct GdObj *sp38;       // object list head before making a bunch of joints
    struct GdObj *faceJoint;        // joint on the face that `grabberJoint` pulls
    struct ObjJoint *grabberJoint;  // joint that's dragged by the cursor
    struct ObjCamera *camera;
    struct ObjAnimator *animator;
    struct ObjParticle *particle;

    // Load Mario head from the dynlist

    start_memtracker("mario face");
    dSetNameSuffix("l");  // add "l" to the end of all dynobj names generated by the dynlist, for some reason

    dUseIntegerNames(TRUE);
    // animator = (struct ObjAnimator *) dMakeObject(D_ANIMATOR, AsDynName(DYNOBJ_MARIO_MAIN_ANIMATOR));
    // animator->controlFunc = aniFn;
    dUseIntegerNames(FALSE);
    // FIXME: make segment address work once seg4 is disassembled
    gMarioFaceGrp = (struct ObjGroup *) load_dynlist(GODDARD_MAIN_FACE_SCENE);
    stop_memtracker("mario face");

    // Make camera

    camera = (struct ObjCamera *) dMakeObject(D_CAMERA, NULL);
    dSetRelativePosition(0.0f, 200.0f, 2000.0f);
    dSetWorldPos(0.0f, 200.0f, 2000.0f);
    dSetFlags(4);
    camera->lookAt.x = 0.0f;
    camera->lookAt.y = 200.0f;
    camera->lookAt.z = 0.0f;

    addto_group(gMarioFaceGrp, &camera->header);
    // addto_group(gMarioFaceGrp, &animator->header);

    dSetNameSuffix(NULL);  // stop adding "l" to generated dynobj names

    // Make sparkle particles

    // particle = make_particle(0, COLOUR_WHITE, 0.0f, 0.0f, 0.0f);
    // particle->unk60 = 3;
    // particle->unk64 = 3;
    // particle->attachedToObj = &camera->header;
    // particle->shapePtr = gShapeSilverSpark;
    // addto_group(gGdLightGroup, &particle->header);

    // particle = make_particle(0, COLOUR_WHITE, 0.0f, 0.0f, 0.0f);
    // particle->unk60 = 3;
    // particle->unk64 = 2;
    // particle->attachedToObj = dUseObject("N228l"); // DYNOBJ_SILVER_STAR_LIGHT
    // particle->shapePtr = gShapeSilverSpark;
    // addto_group(gGdLightGroup, &particle->header);

    // particle = make_particle(0, COLOUR_RED, 0.0f, 0.0f, 0.0f);
    // particle->unk60 = 3;
    // particle->unk64 = 2;
    // particle->attachedToObj = dUseObject("N231l"); // DYNOBJ_RED_STAR_LIGHT
    // particle->shapePtr = gShapeRedSpark;
    // addto_group(gGdLightGroup, &particle->header);

    mainShapesGrp = (struct ObjGroup *) dUseObject("N1000l");  // DYNOBJ_MARIO_MAIN_SHAPES_GROUP
    create_gddl_for_shapes(mainShapesGrp);
    sp38 = gGdObjectList;

    // Make grabbers to move the face with the cursor

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, -500.0f, 0.0f, -150.0f);
    // faceJoint = dUseObject("N167l");  // DYNOBJ_MARIO_LEFT_EAR_JOINT_1
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, 500.0f, 0.0f, -150.0f);
    // faceJoint = dUseObject("N176l");  // DYNOBJ_MARIO_RIGHT_EAR_JOINT_1
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, 0.0f, 700.0f, 300.0f);
    // faceJoint = dUseObject("N131l");  // DYNOBJ_MARIO_CAP_JOINT_1
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // // drag eyelids and eyebrows along with cap?
    // faceJoint = dUseObject("N206l");  // DYNOBJ_LEFT_EYELID_JOINT_1
    // addto_group(grabberJoint->attachedObjsGrp, faceJoint);
    // faceJoint = dUseObject("N215l");  // DYNOBJ_RIGHT_EYELID_JOINT_1
    // addto_group(grabberJoint->attachedObjsGrp, faceJoint);
    // faceJoint = dUseObject("N31l");  // DYNOBJ_MARIO_LEFT_EYEBROW_MPART_JOINT_1
    // addto_group(grabberJoint->attachedObjsGrp, faceJoint);
    // faceJoint = dUseObject("N65l");  // DYNOBJ_MARIO_RIGHT_EYEBROW_MPART_JOINT_1
    // addto_group(grabberJoint->attachedObjsGrp, faceJoint);

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, 0.0f, 0.0f, 600.0f);
    // faceJoint = dUseObject("N185l");  // DYNOBJ_MARIO_NOSE_JOINT_1
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, 0.0f, -300.0f, 300.0f);
    // faceJoint = dUseObject("N194l");  // DYNOBJ_MARIO_LEFT_JAW_JOINT
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, 250.0f, -150.0f, 300.0f);
    // faceJoint = dUseObject("N158l");  // DYNOBJ_MARIO_RIGHT_LIP_CORNER_JOINT_1
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // faceJoint = dUseObject("N15l");  // DYNOBJ_MARIO_LEFT_MUSTACHE_JOINT_1
    // addto_group(grabberJoint->attachedObjsGrp, faceJoint);

    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, -250.0f, -150.0f, 300.0f);
    // faceJoint = dUseObject("N149l");  // DYNOBJ_MARIO_LEFT_LIP_CORNER_JOINT_1
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);

    // faceJoint = dUseObject("N6l");  // DYNOBJ_MARIO_RIGHT_MUSTACHE_JOINT_1
    // addto_group(grabberJoint->attachedObjsGrp, faceJoint);

    // // make the left eye follow cursor
    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, 100.0f, 200.0f, 400.0f);
    // faceJoint = dUseObject("N112l");  // DYNOBJ_MARIO_RIGHT_EYE_UNKNOWN_NET
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);
    // grabberJoint->updateFunc = eye_joint_update_func;
    // grabberJoint->rootAnimator = animator;
    // grabberJoint->header.drawFlags &= ~OBJ_IS_GRABBABLE;

    // // make the right eye follow cursor
    // grabberJoint = make_grabber_joint(sGrabJointTestShape, 0, -100.0f, 200.0f, 400.0f);
    // faceJoint = dUseObject("N96l");  // DYNOBJ_MARIO_LEFT_EYE_UNKNOWN_NET
    // grabberJoint->attachedObjsGrp = make_group(1, faceJoint);
    // grabberJoint->updateFunc = eye_joint_update_func;
    // grabberJoint->rootAnimator = animator;
    // grabberJoint->header.drawFlags &= ~OBJ_IS_GRABBABLE;

    sp48 = make_group_of_type(OBJ_TYPE_JOINTS, sp38, NULL);
    sp54 = make_net(0, NULL, sp48, NULL, NULL);
    sp54->netType = 3;
    addto_group(gMarioFaceGrp, &sp48->header);
    addto_groupfirst(gMarioFaceGrp, &sp54->header);

    return 0;
}

void gdResetDynListAndShapeProcessors(void) {
    imin("gdResetDynListAndShapeProcessors()");
    gdResetDynListProcessor();
    gdResetShapeHelper();
    imout();
}

struct ObjGroup *Unknown8019AB98(UNUSED u32 a0) {
    struct ObjLight *light1;
    struct ObjLight *light2;
    struct GdObj *oldObjHead = gGdObjectList; // obj head node before making lights

    light1 = make_light(0, NULL, 0);
    light1->position.x = 100.0f;
    light1->position.y = 200.0f;
    light1->position.z = 300.0f;

    light1->diffuse.r = 1.0f;
    light1->diffuse.g = 0.0f;
    light1->diffuse.b = 0.0f;

    light1->unk30 = 1.0f;

    light1->unk68.x = 0.4f;
    light1->unk68.y = 0.9f;

    light1->unk80.x = 4.0f;
    light1->unk80.y = 4.0f;
    light1->unk80.z = 2.0f;

    light2 = make_light(0, NULL, 1);
    light2->position.x = 100.0f;
    light2->position.y = 200.0f;
    light2->position.z = 300.0f;

    light2->diffuse.r = 0.0f;
    light2->diffuse.g = 0.0f;
    light2->diffuse.b = 1.0f;

    light2->unk30 = 1.0f;

    light2->unk80.x = -4.0f;
    light2->unk80.y = 4.0f;
    light2->unk80.z = -2.0f;

    gGdLightGroup = make_group_of_type(OBJ_TYPE_LIGHTS, oldObjHead, NULL);

    return gGdLightGroup;
}

struct ObjGroup *Unknown8019ADC4(UNUSED u32 a0) {
    UNUSED struct ObjLight *unusedLight;
    struct ObjLight *newLight;
    struct GdObj *oldObjHead;

    unusedLight = make_light(0, NULL, 0);
    oldObjHead = gGdObjectList;
    newLight = make_light(0, NULL, 0);

    newLight->position.x = 0.0f;
    newLight->position.y = -500.0f;
    newLight->position.z = 0.0f;

    newLight->diffuse.r = 1.0f;
    newLight->diffuse.g = 0.0f;
    newLight->diffuse.b = 0.0f;

    newLight->unk30 = 1.0f;

    gGdLightGroup = make_group_of_type(OBJ_TYPE_LIGHTS, oldObjHead, NULL);

    return gGdLightGroup;
}

struct ObjGroup *Unknown8019AEC4(UNUSED u32 a0) {
    UNUSED struct GdObj *sp1C = gGdObjectList;

    gGdLightGroup = make_group(0);

    return gGdLightGroup;
}
