#pragma once

#include "gd_types.h"

void animate_mario_head_gameover(struct ObjAnimator *self);
void animate_mario_head_normal(struct ObjAnimator *self);

void grabbable_joint_update_func(struct ObjJoint *self);
void eye_joint_update_func(struct ObjJoint *self);
