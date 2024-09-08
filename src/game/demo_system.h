#pragma once

#include "types.h"
#include "level_update.h"
#include "level_table.h"

#define DEMO_BANK_INPUT_CAPACITY (DEMO_INPUTS_POOL_SIZE / sizeof(struct DemoInput))
#define PRESS_START_DEMO_TIMER 30

struct DemoFile {
    void *romStart;
    void *romEnd;
};

struct DemoHeader {
    u32 magic; // 'DEMO'
    u32 *demoStart;
    u16 rng;
    u8 reserved[6];
}; // Current size: 0x10

u8 player_action_reads_stick(struct MarioState *m);
s32 run_level_id_or_demo(s32 level);
s32 print_demo_header(UNUSED s32 arg);
s32 print_demo_footer(UNUSED s32 arg);
void record_demo();
void run_demo_inputs(void);

extern void *demoInputsMalloc;
extern u32 gCurrentDemoSize;
extern u32 gCurrentDemoIdx;
extern struct DemoFile gDemos[LEVEL_COUNT];
extern u8 demoFile[], demoFileEnd[];
extern u16 gDemoLevel;

