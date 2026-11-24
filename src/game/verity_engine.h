#pragma once
#include <ultra64.h>
#include "types.h"

typedef struct {
    s16 word[50];
    void (*dispatch)(struct Object *);
} Wordity;


// all the menu items
typedef enum {
    VS_GENERAL = 0,
    VS_CAP,
    VS_ITEM,
    VS_ATTACK,
    VS_ANIME,

    VS_MAX
} VOICEITY;

