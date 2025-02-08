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


const GeoLayout *ModelIDs[] = {
    [MODEL_BBH_HAUNTED_DOOR] = haunted_door_geo,
    [MODEL_BOWSER_2_TILTING_ARENA] = bowser_2_geo_000170,
    [MODEL_BOWSER_3_FALLING_PLATFORM_1] = bowser_3_geo_000290,
    [MODEL_BOWSER_3_FALLING_PLATFORM_2] = bowser_3_geo_0002A8,
    [MODEL_BOWSER_3_FALLING_PLATFORM_3] = bowser_3_geo_0002C0,
    [MODEL_BOWSER_3_FALLING_PLATFORM_4] = bowser_3_geo_0002D8,
    [MODEL_BOWSER_3_FALLING_PLATFORM_5] = bowser_3_geo_0002F0,
    [MODEL_BOWSER_3_FALLING_PLATFORM_6] = bowser_3_geo_000308,
    [MODEL_BOWSER_3_FALLING_PLATFORM_7] = bowser_3_geo_000320,
    [MODEL_BOWSER_3_FALLING_PLATFORM_8] = bowser_3_geo_000338,
    [MODEL_BOWSER_3_FALLING_PLATFORM_9] = bowser_3_geo_000350,
    [MODEL_BOWSER_3_FALLING_PLATFORM_10] = bowser_3_geo_000368,
    [MODEL_COURTYARD_SPIKY_TREE] = spiky_tree_geo,
    [MODEL_CASTLE_DOOR_0_STARS] = castle_door_0_star_geo,
    [MODEL_CASTLE_DOOR_1_STAR] = castle_door_1_star_geo,
    [MODEL_CASTLE_DOOR_3_STARS] = castle_door_3_stars_geo,
    [MODEL_CASTLE_KEY_DOOR] = key_door_geo,
    [MODEL_CCM_CABIN_DOOR] = cabin_door_geo,
    [MODEL_HMC_HAZY_MAZE_DOOR] = hazy_maze_door_geo,
    [MODEL_MAIN_MENU_MARIO_SAVE_BUTTON] = geo_menu_mario_save_button,
    [MODEL_MAIN_MENU_RED_ERASE_BUTTON] = geo_menu_erase_button,
    [MODEL_MAIN_MENU_BLUE_COPY_BUTTON] = geo_menu_copy_button,
    [MODEL_MAIN_MENU_YELLOW_FILE_BUTTON] = geo_menu_file_button,
    [MODEL_MAIN_MENU_GREEN_SCORE_BUTTON] = geo_menu_score_button,
    [MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE] = geo_menu_mario_save_button_fade,
    [MODEL_MAIN_MENU_MARIO_NEW_BUTTON] = geo_menu_mario_new_button,
    [MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE] = geo_menu_mario_new_button_fade,
    [MODEL_MAIN_MENU_PURPLE_SOUND_BUTTON] = geo_menu_sound_button,
    [MODEL_MAIN_MENU_GENERIC_BUTTON] = geo_menu_generic_button,
    [MODEL_MARIO] = mario_geo,
    [MODEL_SMOKE] = smoke_geo,
    [MODEL_SPARKLES] = sparkles_geo,
    [MODEL_BUBBLE] = bubble_geo,
    [MODEL_SMALL_WATER_SPLASH] = small_water_splash_geo,
    [MODEL_IDLE_WATER_WAVE] = idle_water_wave_geo,
    [MODEL_WATER_SPLASH] = water_splash_geo,
    [MODEL_WAVE_TRAIL] = wave_trail_geo,
    [MODEL_YELLOW_COIN] = yellow_coin_geo,
    [MODEL_STAR] = star_geo,
    [MODEL_TRANSPARENT_STAR] = transparent_star_geo,
    [MODEL_WOODEN_SIGNPOST] = wooden_signpost_geo,
    [MODEL_RED_FLAME] = red_flame_geo,
    [MODEL_BLUE_FLAME] = blue_flame_geo,
    [MODEL_BURN_SMOKE] = burn_smoke_geo,
    [MODEL_LEAVES] = leaves_geo,
    [MODEL_PURPLE_MARBLE] = purple_marble_geo,
    [MODEL_FISH] = fish_geo,
    [MODEL_FISH_SHADOW] = fish_shadow_geo,
    [MODEL_SPARKLES_ANIMATION] = sparkles_animation_geo,
    [MODEL_BUTTERFLY] = butterfly_geo,
    [MODEL_MIST] = mist_geo,
    [MODEL_WHITE_PUFF] = white_puff_geo,
    [MODEL_WHITE_PARTICLE] = white_particle_geo,
    [MODEL_YELLOW_COIN_NO_SHADOW] = yellow_coin_no_shadow_geo,
    [MODEL_BLUE_COIN] = blue_coin_geo,
    [MODEL_BLUE_COIN_NO_SHADOW] = blue_coin_no_shadow_geo,
    [MODEL_MARIOS_WINGED_METAL_CAP] = marios_winged_metal_cap_geo,
    [MODEL_MARIOS_METAL_CAP] = marios_metal_cap_geo,
    [MODEL_MARIOS_WING_CAP] = marios_wing_cap_geo,
    [MODEL_MARIOS_CAP] = marios_cap_geo,
    [MODEL_BOWSER_KEY_CUTSCENE] = bowser_key_cutscene_geo,
    [MODEL_BOWSER_KEY] = bowser_key_geo,
    [MODEL_RED_FLAME_SHADOW] = red_flame_shadow_geo,
    [MODEL_1UP] = mushroom_1up_geo,
    [MODEL_RED_COIN] = red_coin_geo,
    [MODEL_RED_COIN_NO_SHADOW] = red_coin_no_shadow_geo,
    [MODEL_NUMBER] = number_geo,
    [MODEL_EXPLOSION] = explosion_geo,
    [MODEL_DIRT_ANIMATION] = dirt_animation_geo,
    [MODEL_CARTOON_STAR] = cartoon_star_geo,
    [MODEL_WOODEN_DOOR] = wooden_door_geo,
    [MODEL_METAL_DOOR] = metal_door_geo,
    [MODEL_CASTLE_DOOR] = castle_door_geo,
    [MODEL_WARP_PIPE] = warp_pipe_geo,
    [MODEL_BUBBLY_TREE] = bubbly_tree_geo,
    [MODEL_BLUE_COIN_SWITCH] = blue_coin_switch_geo,
    [MODEL_AMP] = dAmpGeo,
    [MODEL_PURPLE_SWITCH] = purple_switch_geo,
    [MODEL_CHECKERBOARD_PLATFORM] = checkerboard_platform_geo,
    [MODEL_BREAKABLE_BOX] = breakable_box_geo,
    [MODEL_BREAKABLE_BOX_NO_SHADOW] = breakable_box_no_shadow_geo,
    [MODEL_EXCLAMATION_BOX_OUTLINE] = exclamation_box_outline_geo,
    [MODEL_EXCLAMATION_BOX] = exclamation_box_geo,
    [MODEL_GOOMBA] = goomba_geo,
    [MODEL_KOOPA_SHELL] = koopa_shell_geo,
    [MODEL_METAL_BOX] = metal_box_geo,
    [MODEL_BLACK_BOBOMB] = black_bobomb_geo,
    [MODEL_BOBOMB_BUDDY] = bobomb_buddy_geo,
    [MODEL_BOWLING_BALL] = bowling_ball_geo,
    [MODEL_CANNON_BARREL] = cannon_barrel_geo,
    [MODEL_CANNON_BASE] = cannon_base_geo,
    [MODEL_HEART] = heart_geo,
    [MODEL_FLYGUY] = flyguy_geo,
    [MODEL_CHUCKYA] = chuckya_geo,
    [MODEL_TRAJECTORY_MARKER_BALL] = bowling_ball_track_geo,
    [MODEL_BULLET_BILL] = bullet_bill_geo,
    [MODEL_YELLOW_SPHERE] = yellow_sphere_geo,
    [MODEL_HOOT] = hoot_geo,
    [MODEL_YOSHI_EGG] = yoshi_egg_geo,
    [MODEL_THWOMP] = thwomp_geo,
    [MODEL_HEAVE_HO] = heave_ho_geo,
    [MODEL_BLARGG] = blargg_geo,
    [MODEL_BULLY] = bully_geo,
    [MODEL_BULLY_BOSS] = bully_boss_geo,
    [MODEL_WATER_BOMB] = water_bomb_geo,
    [MODEL_WATER_BOMB_SHADOW] = water_bomb_shadow_geo,
    [MODEL_KING_BOBOMB] = king_bobomb_geo,
    [MODEL_MANTA_RAY] = manta_seg5_geo_05008D14,
    [MODEL_UNAGI] = unagi_geo,
    [MODEL_SUSHI] = sushi_geo,
    [MODEL_CLAM_SHELL] = clam_shell_geo,
    [MODEL_POKEY_HEAD] = pokey_head_geo,
    [MODEL_POKEY_BODY_PART] = pokey_body_part_geo,
    [MODEL_TWEESTER] = tweester_geo,
    [MODEL_KLEPTO] = klepto_geo,
    [MODEL_EYEROK_LEFT_HAND] = eyerok_left_hand_geo,
    [MODEL_EYEROK_RIGHT_HAND] = eyerok_right_hand_geo,
    [MODEL_MONTY_MOLE] = monty_mole_geo,
    [MODEL_UKIKI] = ukiki_geo,
    [MODEL_FWOOSH] = fwoosh_geo,
    [MODEL_SPINDRIFT] = spindrift_geo,
    [MODEL_MR_BLIZZARD_HIDDEN] = mr_blizzard_hidden_geo,
    [MODEL_MR_BLIZZARD] = mr_blizzard_geo,
    [MODEL_PENGUIN] = penguin_geo,
    [MODEL_CAP_SWITCH] = cap_switch_geo,
    [MODEL_BOO] = boo_geo,
    [MODEL_BETA_BOO_KEY] = small_key_geo,
    [MODEL_HAUNTED_CHAIR] = haunted_chair_geo,
    [MODEL_MAD_PIANO] = mad_piano_geo,
    [MODEL_BOOKEND_PART] = bookend_part_geo,
    [MODEL_BOOKEND] = bookend_geo,
    [MODEL_HAUNTED_CAGE] = haunted_cage_geo,
    [MODEL_BIRDS] = birds_geo,
    [MODEL_PEACH] = peach_geo,
    [MODEL_YOSHI] = yoshi_geo,
    [MODEL_ENEMY_LAKITU] = enemy_lakitu_geo,
    [MODEL_SPINY_BALL] = spiny_ball_geo,
    [MODEL_SPINY] = spiny_geo,
    [MODEL_WIGGLER_HEAD] = wiggler_head_geo,
    [MODEL_WIGGLER_BODY] = wiggler_body_geo,
    [MODEL_BUBBA] = bubba_geo,
    [MODEL_BOWSER] = bowser_geo,
    [MODEL_BOWSER_BOMB] = bowser_bomb_geo,
    [MODEL_BOWSER_SMOKE] = bowser_impact_smoke_geo,
    [MODEL_BOWSER_FLAMES] = bowser_flames_geo,
    [MODEL_BOWSER_WAVE] = invisible_bowser_accessory_geo,
    [MODEL_BOWSER_NO_SHADOW] = bowser_geo_no_shadow,
    [MODEL_BUB] = bub_geo,
    [MODEL_TREASURE_CHEST_BASE] = treasure_chest_base_geo,
    [MODEL_TREASURE_CHEST_LID] = treasure_chest_lid_geo,
    [MODEL_CYAN_FISH] = cyan_fish_geo,
    [MODEL_WATER_RING] = water_ring_geo,
    [MODEL_WATER_MINE] = water_mine_geo,
    [MODEL_SEAWEED] = seaweed_geo,
    [MODEL_SKEETER] = skeeter_geo,
    [MODEL_PIRANHA_PLANT] = piranha_plant_geo,
    [MODEL_WHOMP] = whomp_geo,
    [MODEL_KOOPA_WITH_SHELL] = koopa_with_shell_geo,
    [MODEL_KOOPA_WITHOUT_SHELL] = koopa_without_shell_geo,
    [MODEL_METALLIC_BALL] = metallic_ball_geo,
    [MODEL_CHAIN_CHOMP] = chain_chomp_geo,
    [MODEL_KOOPA_FLAG] = koopa_flag_geo,
    [MODEL_WOODEN_POST] = wooden_post_geo,
    [MODEL_MIPS] = mips_geo,
    [MODEL_BOO_CASTLE] = boo_castle_geo,
    [MODEL_LAKITU] = lakitu_geo,
    [MODEL_TOAD] = toad_geo,
    [MODEL_CHILL_BULLY] = chilly_chief_geo,
    [MODEL_BIG_CHILL_BULLY] = chilly_chief_big_geo,
    [MODEL_MONEYBAG] = moneybag_geo,
    [MODEL_SWOOP] = swoop_geo,
    [MODEL_SCUTTLEBUG] = scuttlebug_geo,
    [MODEL_MR_I_IRIS] = mr_i_iris_geo,
    [MODEL_MR_I_BODY] = mr_i_geo,
    [MODEL_DORRIE] = dorrie_geo,
    [MODEL_SNUFIT] = snufit_geo,
    [MODEL_SNOW_TREE] = snow_tree_geo,
    [MODEL_SSL_PALM_TREE] = palm_tree_geo,

#ifdef IA8_30FPS_COINS
    [MODEL_SILVER_COIN] = silver_coin_geo,
    [MODEL_SILVER_COIN_NO_SHADOW] = silver_coin_no_shadow_geo,
#endif
};

#endif // MTABLE_H
