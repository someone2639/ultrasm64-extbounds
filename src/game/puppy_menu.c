

#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include "sm64.h"
#include "types.h"
#include "level_update.h"
#include "puppy_menu.h"
#include "audio/external.h"
#include "audio/data.h"
#include "game_init.h"
#include "engine/math_util.h"
#include "print.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "include/text_strings.h"
#include "segment2.h"
#include "ingame_menu.h"
#include "memory.h"
#include "object_list_processor.h"
#include "object_helpers.h"
#include "behavior_data.h"
#include "save_file.h"
#include "mario.h"

struct gPuppyStruct gPuppyCam;
struct sPuppyVolume *sPuppyVolumeStack[MAX_PUPPYCAM_VOLUMES];
s16 sFloorHeight = 0;
u8 gPCOptionOpen = 0;
s8 gPCOptionSelected = 0;
s32 gPCOptionTimer = 0;
u8 gPCOptionIndex = 0;
u8 gPCOptionScroll = 0;
u16 gPuppyVolumeCount = 0;
struct MemoryPool *gPuppyMemoryPool;
s32 gPuppyError = 0;

#if defined(VERSION_EU)
static u8 gPCOptionStringsFR[][64] = {{NC_ANALOGUE_FR}, {NC_CAMX_FR}, {NC_CAMY_FR}, {NC_INVERTX_FR}, {NC_INVERTY_FR}, {NC_CAMC_FR}, {NC_SCHEME_FR},};
static u8 gPCOptionStringsDE[][64] = {{NC_ANALOGUE_DE}, {NC_CAMX_DE}, {NC_CAMY_DE}, {NC_INVERTX_DE}, {NC_INVERTY_DE}, {NC_CAMC_DE}, {NC_SCHEME_DE},};
static u8 gPCFlagStringsFR[][64] = {{OPTION_DISABLED_FR}, {OPTION_ENABLED_FR}, {OPTION_SCHEME1_FR}, {OPTION_SCHEME2_FR}, {OPTION_SCHEME3_FR}};
static u8 gPCFlagStringsDE[][64] = {{OPTION_DISABLED_DE}, {OPTION_ENABLED_DE}, {OPTION_SCHEME1_DE}, {OPTION_SCHEME2_DE}, {OPTION_SCHEME3_DE}};
static u8 gPCToggleStringsFR[][64] = {{NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}};
static u8 gPCToggleStringsDE[][64] = {{NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}, {NC_ANALOGUE_EN}};
//static u8 gPCToggleStringsFR[][64] = {{NC_BUTTON_FR}, {NC_BUTTON2_FR}, {NC_OPTION_FR}, {NC_HIGHLIGHT_L_FR}, {NC_HIGHLIGHT_R_FR}};
//static u8 gPCToggleStringsDE[][64] = {{NC_BUTTON_DE}, {NC_BUTTON2_DE}, {NC_OPTION_DE}, {NC_HIGHLIGHT_L_DE}, {NC_HIGHLIGHT_R_DE}};
#endif
static u8 gPCOptionStringsEN[][64] = {{WIDESCREEN}, {NC_CAMX_EN}, {NC_CAMY_EN}, {NC_INVERTX_EN}, {NC_INVERTY_EN}, {NC_CAMC_EN}, {NC_SCHEME_EN},};
static u8 gPCFlagStringsEN[][64] = {{OPTION_DISABLED_EN}, {OPTION_ENABLED_EN}, {OPTION_SCHEME1_EN}, {OPTION_SCHEME2_EN}, {OPTION_SCHEME3_EN}};
static u8 gPCToggleStringsEN[][64] = {{NC_BUTTON_EN}, {NC_BUTTON2_EN}, {NC_OPTION_EN}, {NC_HIGHLIGHT_L}, {NC_HIGHLIGHT_R}};


#define OPT 32 //Just a temp thing

static u8 (*gPCOptionStringsPtr)[OPT][64] = &gPCOptionStringsEN;
static u8 (*gPCFlagStringsPtr)[OPT][64] = &gPCFlagStringsEN;
static u8 (*gPCToggleStringsPtr)[OPT][64] = &gPCToggleStringsEN;


static const struct gPCOptionStruct
{
    u8 gPCOptionName; //This is the position in the newcam_options text array. It doesn't have to directly correlate with its position in the struct
    s16 *gPCOptionVar; //This is the value that the option is going to directly affect.
    u8 gPCOptionStart; //This is where the text array will start. Set it to 255 to have it be ignored.
    s32 gPCOptionMin; //The minimum value of the option.
    s32 gPCOptionMax; //The maximum value of the option.
};
static const struct gPCOptionStruct gPCOptions[]=
{ //If the min and max are 0 and 1, then the value text is used, otherwise it's ignored.
    {/*Option Name*/ 0, /*Option Variable*/ &widescreen,      /*Option Value Text Start*/ 0, /*Option Minimum*/ FALSE, /*Option Maximum*/ TRUE},
    //{/*Option Name*/ 6, /*Option Variable*/ &gPuppyCam.options.inputType,       /*Option Value Text Start*/ 2, /*Option Minimum*/ 0, /*Option Maximum*/ 2},
    //{/*Option Name*/ 1, /*Option Variable*/ &gPuppyCam.options.sensitivityX,   /*Option Value Text Start*/ 255, /*Option Minimum*/ 10, /*Option Maximum*/ 500},
    //{/*Option Name*/ 2, /*Option Variable*/ &gPuppyCam.options.sensitivityY,   /*Option Value Text Start*/ 255, /*Option Minimum*/ 10, /*Option Maximum*/ 500},
    {/*Option Name*/ 3, /*Option Variable*/ &gPuppyCam.options.invertX,        /*Option Value Text Start*/ 0, /*Option Minimum*/ FALSE, /*Option Maximum*/ TRUE},
    {/*Option Name*/ 4, /*Option Variable*/ &gPuppyCam.options.invertY,        /*Option Value Text Start*/ 0, /*Option Minimum*/ FALSE, /*Option Maximum*/ TRUE},
    //{/*Option Name*/ 5, /*Option Variable*/ &gPuppyCam.options.turnAggression, /*Option Value Text Start*/ 255, /*Option Minimum*/ 0, /*Option Maximum*/ 100},
};

u8 gPCOptionCap = sizeof(gPCOptions) / sizeof(struct gPCOptionStruct); //How many options there are in newcam_uptions.

//Some macros for the sake of basic human sanity.
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define ABS(x) ((x) > 0.f ? (x) : -(x))

s16 LENSIN(s16 length, s16 direction)
{
    return (length * sins(direction));
}
s16 LENCOS(s16 length, s16 direction)
{
    return (length * coss(direction));
}



///MENU

#define BLANK 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT

static void puppycam_display_box(s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 a)
{
    gDPSetCombineMode(gDisplayListHead++, BLANK, BLANK);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    if (a !=255)
    {
        gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    }
    else
    {
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF);
    }
    gDPSetEnvColor(gDisplayListHead++, r, g, b, a);
    gDPFillRectangle(gDisplayListHead++, x1, y1, x2, y2);
    gDPPipeSync(gDisplayListHead++);
    gDPSetEnvColor(gDisplayListHead++,255,255,255,255);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gSPDisplayList(gDisplayListHead++,dl_hud_img_end);
}

//I actually took the time to redo this, properly. Lmao. Please don't bully me over this anymore :(
void puppycam_change_setting(s8 toggle)
{
    if (gPlayer1Controller->buttonDown & A_BUTTON)
        toggle*= 5;
    if (gPlayer1Controller->buttonDown & B_BUTTON)
        toggle*= 10;

    if (gPCOptions[gPCOptionSelected].gPCOptionMin == FALSE && gPCOptions[gPCOptionSelected].gPCOptionMax == TRUE)
    {
        *gPCOptions[gPCOptionSelected].gPCOptionVar ^= 1;
    }
    else
        *gPCOptions[gPCOptionSelected].gPCOptionVar += toggle;
    //Forgive me father, for I have sinned. I guess if you wanted a selling point for a 21:9 monitor though, "I can view this line in puppycam's code without scrolling!" can be added to it.
    *gPCOptions[gPCOptionSelected].gPCOptionVar = CLAMP(*gPCOptions[gPCOptionSelected].gPCOptionVar, gPCOptions[gPCOptionSelected].gPCOptionMin, gPCOptions[gPCOptionSelected].gPCOptionMax);
}

void puppycam_print_text(s16 x, s16 y, u8 str[], u8 col)
{
    u8 textX;
    textX = get_str_x_pos_from_center(x,str,10.0f);
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    print_generic_string(textX+1,y-1,str);
    if (col != 0)
    {
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    }
    else
    {
        gDPSetEnvColor(gDisplayListHead++, 255, 32, 32, 255);
    }
    print_generic_string(textX,y,str);
}

//Options menu
void puppycam_display_options()
{
    u8 i = 0;
    u8 newstring[32];
    s16 scroll;
    s16 scrollpos;
    s16 var = gPCOptions;
    s16 vr;
    u16 maxvar;
    u16 minvar;
    f32 newcam_sinpos;

    puppycam_display_box(47,83,281,84,0x0,0x0,0x0, 0xFF);
    puppycam_display_box(47,218,281,219,0x0,0x0,0x0, 0xFF);
    puppycam_display_box(47,83,48,219,0x0,0x0,0x0, 0xFF);
    puppycam_display_box(280,83,281,219,0x0,0x0,0x0, 0xFF);
    puppycam_display_box(271,83,272,219,0x0,0x0,0x0, 0xFF);

    puppycam_display_box(48,84,272,218,0x0,0x0,0x0, 0x50);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_hud_lut_string(HUD_LUT_GLOBAL, 64, 40, (*gPCToggleStringsPtr)[2]);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    if (gPCOptionCap>4)
    {
        puppycam_display_box(272,84,280,218,0x80,0x80,0x80, 0xFF);
        scrollpos = (62)*((f32)gPCOptionScroll/(gPCOptionCap-4));
        puppycam_display_box(272,84+scrollpos,280,156+scrollpos,0xFF,0xFF,0xFF, 0xFF);
    }

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT);
    for (i = 0; i < gPCOptionCap; i++)
    {
        scroll = 140-(32*i)+(gPCOptionScroll*32);
        if (scroll <= 140 && scroll > 32)
        {
            puppycam_print_text(160,scroll,(*gPCOptionStringsPtr)[gPCOptions[i].gPCOptionName],gPCOptionSelected-i);
            if (gPCOptions[i].gPCOptionStart != 255)
            {
                var = *gPCOptions[i].gPCOptionVar+gPCOptions[i].gPCOptionStart;
                if (var < sizeof(gPCFlagStringsEN)) //Failsafe for if it somehow indexes an out of bounds array.
                    puppycam_print_text(160,scroll-12,(*gPCFlagStringsPtr)[var],gPCOptionSelected-i);
            }
            else
            {
                int_to_str(*gPCOptions[i].gPCOptionVar,newstring);
                puppycam_print_text(160,scroll-12,newstring,gPCOptionSelected-i);
                puppycam_display_box(96,111+(32*i)-(gPCOptionScroll*32),224,117+(32*i)-(gPCOptionScroll*32),0x80,0x80,0x80, 0xFF);
                maxvar = gPCOptions[i].gPCOptionMax - gPCOptions[i].gPCOptionMin;
                minvar = *gPCOptions[i].gPCOptionVar - gPCOptions[i].gPCOptionMin;
                puppycam_display_box(96,111+(32*i)-(gPCOptionScroll*32),96+(((f32)minvar/maxvar)*128),117+(32*i)-(gPCOptionScroll*32),0xFF,0xFF,0xFF, 0xFF);
                puppycam_display_box(94+(((f32)minvar/maxvar)*128),109+(32*i)-(gPCOptionScroll*32),98+(((f32)minvar/maxvar)*128),119+(32*i)-(gPCOptionScroll*32),0xFF,0x0,0x0, 0xFF);
                gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
            }
        }
    }
    newcam_sinpos = sins(gGlobalTimer*5000)*4;
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_generic_string(80-newcam_sinpos, 132-(32*(gPCOptionSelected-gPCOptionScroll)),  (*gPCToggleStringsPtr)[3]);
    print_generic_string(232+newcam_sinpos, 132-(32*(gPCOptionSelected-gPCOptionScroll)),  (*gPCToggleStringsPtr)[4]);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

//This has been separated for interesting reasons. Don't question it.
void puppycam_render_option_text(void)
{
    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    puppycam_print_text(278,212,(*gPCToggleStringsPtr)[gPCOptionOpen],1);
    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

extern struct SaveBuffer gSaveBuffer;

void puppycam_check_pause_buttons()
{
    if (gPlayer1Controller->buttonPressed & R_TRIG)
    {
        play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
        if (gPCOptionOpen == 0)
        {
            gPCOptionOpen = 1;
            #if defined(VERSION_EU)
            newcam_set_language();
            #endif
        }

        else
        {
            gPCOptionOpen = 0;
            //puppycam_set_save();
        }
    }

    if (gPCOptionOpen)
    {
        if (ABS(gPlayer1Controller->rawStickY) > 60 || gPlayer1Controller->buttonDown & U_JPAD || gPlayer1Controller->buttonDown & D_JPAD)
        {
            gPCOptionTimer -= 1;
            if (gPCOptionTimer <= 0)
            {
                switch (gPCOptionIndex)
                {
                    case 0: gPCOptionIndex++; gPCOptionTimer += 10; break;
                    default: gPCOptionTimer += 5; break;
                }
                play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
                if (gPlayer1Controller->rawStickY >= 60 || gPlayer1Controller->buttonDown & U_JPAD)
                {
                    gPCOptionSelected--;
                    if (gPCOptionSelected < 0)
                        gPCOptionSelected = gPCOptionCap-1;
                }
                else
                if (gPlayer1Controller->rawStickY <= -60 || gPlayer1Controller->buttonDown & D_JPAD)
                {
                    gPCOptionSelected++;
                    if (gPCOptionSelected >= gPCOptionCap)
                        gPCOptionSelected = 0;
                }
            }
        }
        else
        if (ABS(gPlayer1Controller->rawStickX) > 60 || gPlayer1Controller->buttonDown & L_JPAD || gPlayer1Controller->buttonDown & R_JPAD)
        {
            gPCOptionTimer -= 1;
            if (gPCOptionTimer <= 0)
            {
                switch (gPCOptionIndex)
                {
                    case 0: gPCOptionIndex++; gPCOptionTimer += 10; break;
                    default: gPCOptionTimer += 5; break;
                }
                play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
                if (gPlayer1Controller->rawStickX >= 60 || gPlayer1Controller->buttonDown & R_JPAD)
                    puppycam_change_setting(1);
                else
                if (gPlayer1Controller->rawStickX <= -60 || gPlayer1Controller->buttonDown & L_JPAD)
                    puppycam_change_setting(-1);
            }
        }
        else
        {
            gPCOptionTimer = 0;
            gPCOptionIndex = 0;
        }

        while (gPCOptionScroll - gPCOptionSelected < -3 && gPCOptionSelected > gPCOptionScroll)
            gPCOptionScroll +=1;
        while (gPCOptionScroll + gPCOptionSelected > 0 && gPCOptionSelected < gPCOptionScroll)
            gPCOptionScroll -=1;
    }
}
