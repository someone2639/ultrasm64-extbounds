#ifndef GD_MATH_H
#define GD_MATH_H

#include <PR/ultratypes.h>

#include "gd_types.h"
#include "macros.h"

struct Row4 {
    f32 c0, c1, c2, c3;
};

struct InvMat4 {
    struct Row4 r0, r1, r2, r3;
};

enum GdRotAxis {
    GD_X_AXIS,
    GD_Y_AXIS,
    GD_Z_AXIS
};

// Needed for gd_math.c itself.
void gdMatrixAdjunctF(Mat4f *src, Mat4f *dst);
f32 gdMatrixDeterminantF(Mat4f *mtx);
f32 gdDeterminant3x3(f32 r0c0, f32 r0c1, f32 r0c2,
               f32 r1c0, f32 r1c1, f32 r1c2, 
               f32 r2c0, f32 r2c1, f32 r2c2);
f32 gdDeterminant2x2(f32 a, f32 b, f32 c, f32 d);

void gdMatrixLookAtF(Mat4f *mtx, f32 xFrom, f32 yFrom, f32 zFrom, f32 xTo, f32 yTo, f32 zTo,
                     f32 zColY, f32 yColY, f32 xColY);
void gdVectorScaleF(Mat4f *mtx, struct GdVec3f *vec);
void gd_rot_mat_about_vec(Mat4f *mtx, struct GdVec3f *vec);
void gdMatrixTranslateF(Mat4f *mtx, struct GdVec3f *vec);
void gdLookAtFromOriginF(Mat4f *mtx, struct GdVec3f *vec, f32 roll);
f32 gdClampF(f32 value, f32 limit);
void gd_clamp_vec3f(struct GdVec3f *vec, f32 limit);
void gd_rot_2d_vec(f32 deg, f32 *x, f32 *y);
void gdMatrixAxisRotateF(Mat4f *mtx, s32 axisnum, f32 ang);
f32 gdVectorLength(struct GdVec3f *vec);
s32 gdVectorNormalize(struct GdVec3f *vec);
void gdVectorCrossF(struct GdVec3f *a, struct GdVec3f *b, struct GdVec3f *dst);
f32 gdVectorDotF(struct GdVec3f *a, struct GdVec3f *b);
void gdMatrixInvertF(Mat4f *src, Mat4f *dst);
void gd_create_rot_mat_angular(Mat4f *mtx, struct GdVec3f *vec, f32 ang);
void gdMakeIdentityMatrixF(Mat4f *mtx);
void gdCopyMatrixF(const Mat4f *src, Mat4f *dst);
void gdTransformVector(struct GdVec3f *vec, const Mat4f *mtx);
void gdRotateVector(struct GdVec3f *vec, const Mat4f *mtx);
void gdMultiplyMatrixF(const Mat4f *mA, const Mat4f *mB, Mat4f *dst);
void gdPrintVector(UNUSED const char *prefix, const struct GdVec3f *vec);
void gdPrintBoundingBox(UNUSED const char *prefix, const struct GdBoundingBox *p);
void gdPrintMatrix(UNUSED const char *prefix, const Mat4f *mtx);

#endif // GD_MATH_H
