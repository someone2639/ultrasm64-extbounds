#ifndef MODEL_TABLE_H
#define MODEL_TABLE_H

#include "model_ids.h"
#include "types.h"
#include "actors/common0.h"
#include "actors/common1.h"
#include "actors/group0.h"
#include "actors/group1.h"
#include "actors/group2.h"
#include "actors/group3.h"
#include "actors/group4.h"
#include "actors/group5.h"
#include "actors/group6.h"
#include "actors/group7.h"
#include "actors/group8.h"
#include "actors/group9.h"
#include "actors/group10.h"
#include "actors/group11.h"
#include "actors/group12.h"
#include "actors/group13.h"
#include "actors/group14.h"
#include "actors/group15.h"
#include "actors/group16.h"
#include "actors/group17.h"
#include "game/ingame_menu.h"
#include "levels/menu/header.h"
#include "levels/bowser_2/header.h"
#include "levels/bowser_3/header.h"
#include "levels/intro/header.h"
#include "actors/vanilla_actors/bbh/header.h"
#include "actors/vanilla_actors/bitdw/header.h"
#include "actors/vanilla_actors/bitfs/header.h"
#include "actors/vanilla_actors/bits/header.h"
#include "actors/vanilla_actors/bob/header.h"
#include "actors/vanilla_actors/castle_grounds/header.h"
#include "actors/vanilla_actors/castle_inside/header.h"
#include "actors/vanilla_actors/ccm/header.h"
#include "actors/vanilla_actors/ddd/header.h"
#include "actors/vanilla_actors/hmc/header.h"
#include "actors/vanilla_actors/jrb/header.h"
#include "actors/vanilla_actors/lll/header.h"
#include "actors/vanilla_actors/rr/header.h"
#include "actors/vanilla_actors/sl/header.h"
#include "actors/vanilla_actors/ssl/header.h"
#include "actors/vanilla_actors/thi/header.h"
#include "actors/vanilla_actors/ttc/header.h"
#include "actors/vanilla_actors/ttm/header.h"
#include "actors/vanilla_actors/vcutm/header.h"
#include "actors/vanilla_actors/wdw/header.h"
#include "actors/vanilla_actors/wf/header.h"

// Handle `_` folders
#define ___actor_dataSegmentRomStart NULL
#define ___actor_dataSegmentRomEnd NULL

#define DEFINE_MODEL(model, folder, geo) extern u8 _##folder##_actor_dataSegmentRomStart[]; extern u8 _##folder##_actor_dataSegmentRomEnd[];
#include "model_defines.h"
#undef DEFINE_MODEL

static const Actor model_table[] = {
#define DEFINE_MODEL(model, folder, geo) [model] = { _##folder##_actor_dataSegmentRomStart,  _##folder##_actor_dataSegmentRomEnd, geo},
#include "model_defines.h"
#undef DEFINE_MODEL
};

#endif // MTABLE_H
