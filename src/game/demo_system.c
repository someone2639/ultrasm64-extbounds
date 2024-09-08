#include <string.h>
#include "types.h"
#include "segments.h"

#include "demo_system.h"
#include "game_init.h"
#include "level_update.h"
#include "memory.h"
#include "save_file.h"

void *demoInputsMalloc = NULL;
u32 gCurrentDemoSize = 0;
u32 gCurrentDemoIdx = 0;
struct DemoFile gDemos[LEVEL_COUNT] ALIGNED8;
static u16 sDemoCountdown = 0;
u16 gDemoLevel = 0;
extern u16 gRandomSeed16;

// Level to String conversion for telling the player where to save the file
#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8) "stub_level",
#define DEFINE_LEVEL(_0, _1, _2, filename, _4, _5, _6, _7, _8, _9, _10) #filename,
static char sLevel2Str[LEVEL_COUNT][20] = {
    #include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

u8 player_action_reads_stick(struct MarioState *m) {
    if (m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_ON_POLE)) {
        return TRUE;
    }
    return FALSE;
}

void apply_demo_inputs_to_player(struct MarioState *m) {
    if (player_action_reads_stick(m)) {
        // do nothing
        return;
    }
    if (gCurrDemoInput->stickMag > 0.0f) {
        m->intendedMag = gCurrDemoInput->stickMag;
        m->input |= INPUT_NONZERO_ANALOG;
    }
    m->intendedYaw = gCurrDemoInput->stickYaw;
}


void dma_new_demo_data() {
    void *demoBank = get_segment_base_addr(SEGMENT_DEMO_INPUTS);

    u8 *romStart = gDemos[gDemoLevel].romStart + (sizeof(struct DemoInput) * gCurrentDemoIdx);
    u8 *romEnd;
    if (gCurrentDemoIdx + DEMO_BANK_INPUT_CAPACITY > gCurrentDemoSize) {
        romEnd = gDemos[gDemoLevel].romEnd;
    }
    else {
        romEnd = romStart + DEMO_INPUTS_POOL_SIZE;
    }

    dma_read(demoBank, romStart, romEnd);
}

/**
 * If a demo sequence exists, this will run the demo input list until it is complete.
 */
void run_demo_inputs(void) {
    // Eliminate the unused bits.
    gPlayer1Controller->controllerData->button &= VALID_BUTTONS;

    // Check if a demo inputs list exists and if so,
    // run the active demo input list.
    if (gCurrDemoInput != NULL) {
        // The timer variable being 0 at the current input means the demo is over.
        // Set the button to the END_DEMO mask to end the demo.
        if (gCurrDemoInput->timer == 0) {
            gPlayer1Controller->controllerData->stick_x = 0;
            gPlayer1Controller->controllerData->stick_y = 0;
            gPlayer1Controller->controllerData->button = END_DEMO;
        } else {
            // Backup the start button if it is pressed, since we don't want the
            // demo input to override the mask where start may have been pressed.
            u16 startPushed = (gPlayer1Controller->controllerData->button & START_BUTTON);

            // Perform the demo inputs by assigning the current button mask and the stick inputs.
            if (player_action_reads_stick(gMarioState)) {
                gPlayer1Controller->controllerData->stick_x = gCurrDemoInput->stickPos[0];
                gPlayer1Controller->controllerData->stick_y = gCurrDemoInput->stickPos[1];
            }

            // To assign the demo input, the button information is stored in
            // an 8-bit mask rather than a 16-bit mask. this is because only
            // A, B, Z, Start, and the C-Buttons are used in a demo, as bits
            // in that order. In order to assign the mask, we need to take the
            // upper 4 bits (A, B, Z, and Start) and shift then left by 8 to
            // match the correct input mask. We then add this to the masked
            // lower 4 bits to get the correct button mask.
            gPlayer1Controller->controllerData->button = gCurrDemoInput->buttonMask;

            // If start was pushed, put it into the demo sequence being input to end the demo.
            gPlayer1Controller->controllerData->button |= startPushed;

            // Run the current demo input's timer down. if it hits 0, advance the demo input list.
            if (--gCurrDemoInput->timer == 0) {
                struct DemoInput *demoBank = get_segment_base_addr(SEGMENT_DEMO_INPUTS);
                u8 needs_dma = (gCurrDemoInput == &demoBank[DEMO_BANK_INPUT_CAPACITY - 1]);
                gCurrDemoInput++;
                gCurrentDemoIdx++;
                if (needs_dma) {
                    dma_new_demo_data();
                    gCurrDemoInput = demoBank;
                }
            }
        }
    }
}

/**
 * Run the demo timer on the PRESS START screen after a number of frames.
 * This function returns the level ID from the first byte of a demo file.
 * It also returns the level ID from intro_regular (file select or level select menu)
 */
s32 run_level_id_or_demo(s32 level) {
    gCurrDemoInput = NULL;

    if (level == LEVEL_NONE) {
        if (!gPlayer1Controller->buttonDown && !gPlayer1Controller->stickMag) {
            // start the demo. 800 frames has passed while
            // player is idle on PRESS START screen.
            if ((++sDemoCountdown) == PRESS_START_DEMO_TIMER) {
                u32 demoCount = 0;

                // DMA in the Level Demo List
                // Should always DMA in (LEVEL_COUNT * 8) bytes
                dma_read((u8 *) &gDemos, demoFile, demoFileEnd);

                // Find a non-null demo in the list
                // (If a demo played already, increment first before checking)
                do {
                    if (gDemoLevel >= LEVEL_MAX) {
                        gDemoLevel = 0;
                    }
                    gDemoLevel++;
                    demoCount++;
                     // No demos installed in assets/demos/; continue playing the title screen
                    if (demoCount > (LEVEL_MAX * 2)) {
                        sDemoCountdown = 0;
                        return level;
                    }
                } while (gDemos[gDemoLevel].romStart == NULL);

                gCurrentDemoSize = (u32) gDemos[gDemoLevel].romEnd - (u32) gDemos[gDemoLevel].romStart;
                gCurrentDemoIdx = 0;
                dma_new_demo_data();
#ifdef DEMO_RECORDING_MODE
                print_demo_header();
#endif // DEMO_RECORDING_MODE
                struct DemoInput *demoBank = get_segment_base_addr(SEGMENT_DEMO_INPUTS);

                // Point the current input to the demo segment
                gCurrDemoInput = demoBank;
                level = gDemoLevel + 1;
                gCurrSaveFileNum = 1;
                gCurrActNum = 1;
            }
        } else { // activity was detected, so reset the demo countdown.
            sDemoCountdown = 0;
        }
    }
    return level;
}

#ifdef DEMO_RECORDING_MODE
// TODO: When libcart is merged, replace all these print functions
//       with file i/o that automatically saves the file to the SD Card.
s32 print_demo_header(UNUSED s32 arg) {
    char header[500];
    sprintf(header, R"(
#include "demo_macros.inc"

start_demo %s
set_rng %d
)", sLevel2Str[START_LEVEL - 1], gRandomSeed16);
    osSyncPrintf(header);
    return 0;
}

void print_demo_input(struct DemoInput *d) {
    char buttonStr[20];
    char *buttonPtr = buttonStr;

    if (d->timer == 0) {
        osSyncPrintf("end_demo\n");
        return;
    }

    if (d->buttonMask == 0) {
        sprintf(buttonStr, "_");
    } else {
        u16 button = d->buttonMask;

        if (button & A_BUTTON) {
            buttonPtr += sprintf(buttonPtr, "A | ");
        }
        if (button & B_BUTTON) {
            buttonPtr += sprintf(buttonPtr, "B | ");
        }
        if (button & L_TRIG) {
            buttonPtr += sprintf(buttonPtr, "L | ");
        }
        if (button & R_TRIG) {
            buttonPtr += sprintf(buttonPtr, "R | ");
        }
        if (button & Z_TRIG) {
            buttonPtr += sprintf(buttonPtr, "Z | ");
        }
        if (button & START_BUTTON) {
            buttonPtr += sprintf(buttonPtr, "Start | ");
        }

        if (button & U_CBUTTONS) {
            buttonPtr += sprintf(buttonPtr, "C_Up | ");
        }
        if (button & D_CBUTTONS) {
            buttonPtr += sprintf(buttonPtr, "C_Down | ");
        }
        if (button & L_CBUTTONS) {
            buttonPtr += sprintf(buttonPtr, "C_Left | ");
        }
        if (button & R_CBUTTONS) {
            buttonPtr += sprintf(buttonPtr, "C_Right | ");
        }

        u32 len = strlen(buttonStr);
        buttonStr[len - 3] = 0; // Remove the trailing ' | '
    }

    char text[100];

    if (player_action_reads_stick(gMarioState)) {
        sprintf(text, "for %3d frames;  mag %2f;  stick %3d, %3d;  press %s\n",
            d->timer,
            d->stickMag,
            gPlayer1Controller->rawStickX,
            gPlayer1Controller->rawStickY,
            buttonStr
        );
    } else {
        sprintf(text, "for %3d frames;  mag %2f;  yaw %6d;  press %s\n",
            d->timer,
            d->stickMag,
            d->stickYaw,
            buttonStr
        );
    }
    osSyncPrintf(text);
}

s32 print_demo_footer(UNUSED s32 arg) {
    char footer[300];
    sprintf(footer, R"(
end_demo
/* Copy the above output to 'assets/demos/%s.s' */
)", sLevel2Str[START_LEVEL - 1]);
    osSyncPrintf(footer);
    return 0;
}

// this function records distinct inputs over a 255-frame interval to RAM locations and was likely
// used to record the demo sequences seen in the final game. This function is unused.
void record_demo(void) {
    if (gMarioState == NULL) return;
    // record the player's button mask and current rawStickX and rawStickY.
    u16 buttonMask = gPlayer1Controller->buttonDown;
    s16 intendedYaw = gMarioState->intendedYaw;
    f32 stickMag = gMarioState->intendedMag;

    // Rrecord the distinct input and timer so long as they are unique.
    // If the timer hits 0xFF, reset the timer for the next demo input.
    if (gRecordedDemoInput.timer == 0xFF || buttonMask != gRecordedDemoInput.buttonMask
        || intendedYaw != gRecordedDemoInput.stickYaw || stickMag != gRecordedDemoInput.stickMag) {
        print_demo_input(&gRecordedDemoInput);
        gRecordedDemoInput.timer = 0;
        gRecordedDemoInput.buttonMask = buttonMask;
        gRecordedDemoInput.stickYaw = intendedYaw;
        gRecordedDemoInput.stickMag = stickMag;
    }
    gRecordedDemoInput.timer++;
}

#endif // DEMO_RECORDING_MODE

