#pragma once

#include "game/object_list_processor.h"

typedef struct {
    u32 *offset;
    u32 size;
} BhvFile;

typedef struct {
    u32 index;
    u32 *bhvScript;
} LoadedScript;

extern u32 gBhvTableLoaded;

extern LoadedScript gLoadedBehaviors[OBJECT_POOL_CAPACITY];
extern BehaviorScript *gMarioBhvScript;

int get_idx_from_bhv (void *bhv);
void load_bhv_table();
void unload_bhv_table();
const BehaviorScript *get_streamed_bhv(u32 index);

