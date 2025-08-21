#include <ultra64.h>

#include "behaviors.h"
#include "draw_objects.h"
#include "dynlist_proc.h"
#include "gd_macros.h"
#include "gd_main.h"
#include "gd_math.h"
#include "joints.h"
#include "objects.h"
#include "sfx.h"

/**
 * Controls movement of grabbable joints
 */
void grabbable_joint_update_func(struct ObjJoint *self) {
    Mat4f *attObjMtx;
    struct GdVec3f offset;  // difference between current position and initial position
    register struct ListNode *att;
    struct GdObj *attobj;

    // The joint acts somewhat like a spring in that the further it is moved
    // from its original position, the more resistance it has to moving further

    offset.x = self->mat128[3][0] - self->initPos.x;
    offset.y = self->mat128[3][1] - self->initPos.y;
    offset.z = self->mat128[3][2] - self->initPos.z;

    if (self->header.drawFlags & OBJ_PICKED) {
        self->velocity.x = offset.x * -0.25;
        self->velocity.y = offset.y * -0.25;
        self->velocity.z = offset.z * -0.25;

        self->flags |= 0x2000;
    } else {
        if (gdControllerInfo.trgR == FALSE) { // R trigger is released
            // Set velocity so that the joint approaches its initial position
            self->velocity.x -= offset.x * 0.5; //? 0.5f
            self->velocity.y -= offset.y * 0.5; //? 0.5f
            self->velocity.z -= offset.z * 0.5; //? 0.5f

            // Decay the velocity
            self->velocity.x *= 0.8; //? 0.8f
            self->velocity.y *= 0.8; //? 0.8f
            self->velocity.z *= 0.8; //? 0.8f

            // If the joint's velocity has decayed enough and it is very close
            // to its original position, stop its movement altogether
            if (ABS(self->velocity.x) + ABS(self->velocity.y) + ABS(self->velocity.z) < 1.0) {
                if (ABS(offset.x) + ABS(offset.y) + ABS(offset.z) < 1.0) {
                    self->velocity.x = self->velocity.y = self->velocity.z = 0.0f;
                    self->mat128[3][0] -= offset.x;
                    self->mat128[3][1] -= offset.y;
                    self->mat128[3][2] -= offset.z;
                }
            }

            if (self->flags & 0x2000) {
                gd_play_sfx(GD_SFX_LET_GO_FACE);
            }

            self->flags &= ~0x2000;
        } else {
            // freeze position of joint
            self->velocity.x = self->velocity.y = self->velocity.z = 0.0f;
        }
    }

    // update position
    self->mat128[3][0] += self->velocity.x;
    self->mat128[3][1] += self->velocity.y;
    self->mat128[3][2] += self->velocity.z;

    if (self->header.drawFlags & OBJ_PICKED) {
        gdControllerInfo.cursorX -= (gdControllerInfo.cursorX - gdControllerInfo.dragStartX) * 0.2;
        gdControllerInfo.cursorY -= (gdControllerInfo.cursorY - gdControllerInfo.dragStartY) * 0.2;
    }

    // update position of attached objects
    offset.x = self->mat128[3][0] - self->initPos.x;
    offset.y = self->mat128[3][1] - self->initPos.y;
    offset.z = self->mat128[3][2] - self->initPos.z;
    for (att = self->attachedObjsGrp->firstMember; att != NULL; att = att->next) {
        attobj = att->obj;
        set_cur_dynobj(attobj);
        attObjMtx = dGetMatrixPointer();
        gd_add_vec3f_to_mat4f_offset(attObjMtx, &offset);
    }
}

/**
 * Update function for Mario's eye joints, which makes them follow the cursor
 */
void eye_joint_update_func(struct ObjJoint *self) {
    Mat4f *sp5C;
    struct GdVec3f sp50;
    struct GdVec3f sp44;
    register struct ListNode *att;
    struct GdObj *attobj;

    if (gdCurrentMoveCamera == NULL) {
        return;
    }

    if (self->rootAnimator != NULL) {
        if (self->rootAnimator->state != 7) {
            return;
        }
    }

    set_cur_dynobj((struct GdObj *)self);
    sp5C = dGetRotationMatrixPointer();
    sp44.x = (*sp5C)[3][0];
    sp44.y = (*sp5C)[3][1];
    sp44.z = (*sp5C)[3][2];
    world_pos_to_screen_coords(&sp44, gdCurrentMoveCamera, sCurrentMoveView);

    sp50.x = gdControllerInfo.cursorX - sp44.x;
    sp50.y = -(gdControllerInfo.cursorY - sp44.y);
    sp50.z = 0.0f;

    sp50.x *= 2.0; //?2.0f
    sp50.y *= 2.0; //?2.0f
    sp50.z *= 2.0; //?2.0f
    if (gd_vec3f_magnitude(&sp50) > 30.0f) {
        gd_normalize_vec3f(&sp50);
        sp50.x *= 30.0f;
        sp50.y *= 30.0f;
        sp50.z *= 30.0f;
    }

    for (att = self->attachedObjsGrp->firstMember; att != NULL; att = att->next) {
        attobj = att->obj;
        set_cur_dynobj(attobj);
        sp5C = dGetRotationMatrixPointer();
        gd_add_vec3f_to_mat4f_offset(sp5C, &sp50);
    }
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
    s32 pressedA = gdControllerInfo.dragging;

    switch (self->state) {
        case 0:
            // initialize?
            self->frame = 1.0f;
            self->animSeqNum = 0;  // normal anim sequence
            state = 2;
            self->nods = 5;
            break;
        case 2:
            if (pressedA) {
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
            if (pressedA) {
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


