#include <ultra64.h>

#include "sm64.h"
#include "behavior_data.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "object_load.h"

LoadedScript gLoadedBehaviors[OBJECT_POOL_CAPACITY] = {0};

BhvFile *gBhvTable = NULL;
u32 gBhvTableSize = 0;
u32 gBhvTableLoaded = FALSE;
u32 numBhvs = 0;
u32 numBhvsLoaded = 0;
BehaviorScript *gMarioBhvScript;

extern u8 gObjectTable[];
extern u8 gObjectTableEnd[];

void load_bhv_table() {
    gBhvTableSize = ALIGN16((u32)gObjectTableEnd - (u32)gObjectTable);
    gBhvTable = main_pool_alloc(gBhvTableSize, MEMORY_POOL_LEFT);

    dma_read(
        (u8*)gBhvTable,
        gObjectTable,
        (u8*)gObjectTableEnd
    );

    numBhvs = gBhvTableSize / sizeof(BhvFile);
    gBhvTableLoaded = TRUE;
    numBhvsLoaded = 0;
}

void unload_bhv_table() {
    if (gBhvTable) {
        main_pool_free(gBhvTable);
        gBhvTable = NULL;
    }
    for (u32 i = 0; i < numBhvsLoaded; i++) {
        gLoadedBehaviors[i].index = 0;
        main_pool_free(gLoadedBehaviors[i].bhvScript);
        gLoadedBehaviors[i].bhvScript = NULL;
    }
    gBhvTableLoaded = FALSE;
    numBhvsLoaded = 0;
}

int get_idx_from_bhv (void *bhv) {
    for (u32 i = 0; i < numBhvsLoaded; i++) {
        if (gLoadedBehaviors[i].bhvScript == bhv) {
            return gLoadedBehaviors[i].index;
        }
    }

    return -1;
}

const BehaviorScript *get_streamed_bhv(u32 index) {
    char buf[500];

    // check if object exists
    for (u32 i = 0; i < numBhvsLoaded; i++) {
        if (gLoadedBehaviors[i].index == index) {
            return gLoadedBehaviors[i].bhvScript;
        }
    }
    // sprintf(buf, "loading %d\n", index);
    // osSyncPrintf(buf);

    // otherwise, load it
    gLoadedBehaviors[numBhvsLoaded].index = index;
    gLoadedBehaviors[numBhvsLoaded].bhvScript = main_pool_alloc(ALIGN8(gBhvTable[index].size), MEMORY_POOL_LEFT);
    dma_read(
        (u8*)gLoadedBehaviors[numBhvsLoaded].bhvScript,
        (u8*)gBhvTable[index].offset,
        (u8*)((u32)gBhvTable[index].offset + gBhvTable[index].size)
    );

    if (index == (u32)bhvMario) {
        gMarioBhvScript = gLoadedBehaviors[numBhvsLoaded].bhvScript;
    }

    // sprintf(buf, "   to %08X\n", gLoadedBehaviors[numBhvsLoaded].bhvScript);
    // osSyncPrintf(buf);

    return gLoadedBehaviors[numBhvsLoaded++].bhvScript;
}

