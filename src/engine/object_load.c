#include <ultra64.h>

#include "sm64.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"

BehaviorScript *gLoadedBehaviors[OBJECT_POOL_CAPACITY] = {0};

u32 *gBhvTable = NULL;
u32 gBhvTableSize = 0;

extern u8 _object_tableSegmentRomStart[];
extern u8 _object_tableSegmentRomEnd[];

void load_bhv_table() {
    gBhvTableSize = (u32)_object_tableSegmentRomEnd - (u32)_object_tableSegmentRomStart;
    gBhvTable = main_pool_alloc(gBhvTableSize, MEMORY_POOL_LEFT);
    dma_read((u8*)gBhvTable, _object_tableSegmentRomStart, _object_tableSegmentRomEnd);

    u32 numBhvs = gBhvTableSize / 8;

    for (u32 i = 0; i < numBhvs; i++) {
        u32 *bhvRom = (u32*)gBhvTable[(i * 2) + 0];
        u32 bhvSize = gBhvTable[(i * 2) + 1];
        gLoadedBehaviors[i] = main_pool_alloc(bhvSize, MEMORY_POOL_LEFT);
        dma_read((u8*)gLoadedBehaviors[i], (u8*)bhvRom, (u8*)((u32)bhvRom + bhvSize));
    }
}

void unload_bhv_table() {
    u32 numBhvs = gBhvTableSize / 8;

    for (u32 i = 0; i < numBhvs; i++) {
        main_pool_free(gLoadedBehaviors[i]);
    }
    main_pool_free(gBhvTable);
}

void *get_streamed_bhv(u32 index) {
    return gLoadedBehaviors[index - 1];
}

