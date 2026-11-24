
// hey its me its enginity
#include "verity_engine.h"
#include "print.h"
#include "hud.h"
#include "level_update.h"
#include "game_init.h"
#include "object_list_processor.h"

VOICEITY voiceState = VS_GENERAL;
VOICEITY voiceStateNext = VS_GENERAL;

#define TX (12)
#define TY (12)

Wordity WordList[][8] = {
    [VS_GENERAL] = {
        // hellp
        // 
    },
    [VS_CAP] = {
        // metal
        // wing
        // vanish
    },
    [VS_ITEM] = {
        // 1up
        // box
        // 
    },
    [VS_ATTACK] = {
        // shoot
        // 
    },
    [VS_ANIME] = {
        // Explosion!
    },
}

void verity_engine_hud() {
    switch (voiceState) {
        case VS_GENERAL: print_text(TX, TY, "General"); break;
        case VS_CAP: print_text(TX, TY, "Cap"); break;
        case VS_ITEM: print_text(TX, TY, "Item"); break;
        case VS_ATTACK: print_text(TX, TY, "Attack"); break;

        default: break;
    }
}

void verity_engine_dispatch() {
    if (gMarioState && gMarioObject) {
        verity_engine_hud();

        if (gPlayer1Controller->buttonPressed & L_TRIG) {
            voiceStateNext++;
            if (voiceStateNext >= VS_MAX) {
                voiceStateNext = VS_GENERAL;
            }
        }
    }


    if (voiceStateNext != voiceState) {
        // TODO: load dictionary here
    }
}

