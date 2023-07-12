#include <ultra64.h>

#include "object_list_processor.h"
#include "main.h"
#include "contpakreal.h"
#include "debug.h"
#include "game_init.h"

#define _ULTRA64
#include "libcart/include/cart.h"
#include "libcart/ff/ff.h"

#define CPR_FILENAME "troll.bin"

#include "troll_sprites/nocard.h"

FATFS FatFs;
FIL cpr_File;

u8 cpr_Texture[4096];
u8 cpr_DrawComplete = FALSE;
u8 cpr_Mounted = FALSE;

void cpr_debug() {
    // print_text_fmt_int(50, 50, "DRAWN %d", cpr_DrawComplete);
    // print_text_fmt_int(50, 70, "MOUNTED %d", cpr_Mounted);
    // print_text_fmt_int(50, 90, "GONE %d", cpr_filegone());
    // print_text_fmt_int(50, 110, "fptr %d", cpr_File.fptr);
}

void cpr_tick() {
    if ((gCurrLevelNum == CPR_STARTLEVEL) && (cpr_Mounted == FALSE)) {
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_time_stop();
        // gSPDisplayList(gDisplayListHead++, nocard_bg_dl);
        render_multi_image(nocard_tex_0, 0, 0, 320, 240, 1, 1, G_CYC_1CYCLE);
        return;
    }

    if ((gCurrLevelNum == CPR_STARTLEVEL) && (cpr_DrawComplete == FALSE)) {
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        enable_time_stop();
        if (cpr_minigame() == 1) {
            cpr_DrawComplete = TRUE;
            clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        }
    }


    if (cpr_DrawComplete == TRUE) {
        cpr_apply();
    }
}

size_t cpr_filesize() {
    FILINFO result;
    FRESULT code = f_stat(CPR_FILENAME, &result);


    return result.fsize;
}

void cpr_makefile() {
    FRESULT code = f_open(&cpr_File, CPR_FILENAME, FA_WRITE | FA_READ | FA_CREATE_ALWAYS);

    char buf[50];
    sprintf(buf, "CREATE FAIL %d", code);
    assert(code == 0, buf);
}

void cpr_openfile() {
    FRESULT code = f_open(&cpr_File, CPR_FILENAME, FA_WRITE | FA_READ);

    char buf[50];
    sprintf(buf, "OPEN FAIL %d", code);
    assert(code == 0, buf);
}

void cpr_read() {
    if (cpr_filesize() == 0) return; // silent fail

    u32 bytes_read;
    FRESULT code = f_read(&cpr_File, cpr_Texture, sizeof(cpr_Texture), &bytes_read);

    char buf[50];
    sprintf(buf, "READ FAIL %d", code);
    assert(code == 0, buf);
    sprintf(buf, "READ TOO LARGE/SMALL %d", bytes_read);
    assert(bytes_read == sizeof(cpr_Texture), buf);

    f_rewind(&cpr_File);
}

void cpr_write() {
    u32 bytes_writ;
    FRESULT code = f_write(&cpr_File, cpr_Texture, sizeof(cpr_Texture), &bytes_writ);

    char buf[50];
    sprintf(buf, "WRITE FAIL %d", code);
    assert(code == 0, buf);
    sprintf(buf, "WRITE TOO LARGE/SMALL %d", bytes_writ);
    assert(bytes_writ == sizeof(cpr_Texture), buf);
}

void cpr_close() {
    FRESULT code = f_close(&cpr_File);

    char buf[50];
    sprintf(buf, "CLOSE FAIL %d", code);
    assert(code == FR_OK, buf);
}

void cpr_deletefile() {
    FRESULT code = f_close(&cpr_File);
    code = f_unlink(CPR_FILENAME);

    if (code == FR_NO_FILE) return;

    char buf[50];
    sprintf(buf, "DELETE FAIL %d", code);
    assert(code == FR_OK, buf);
}

int cpr_filegone() {
    FILINFO result;
    FRESULT code = f_stat(CPR_FILENAME, &result);


    return (code == FR_NO_FILE);
}

void cpr_checkfile() {
    if (cpr_filegone()) {
        cpr_makefile();
    } else {
        cpr_DrawComplete = TRUE;
        cpr_openfile();
        cpr_read();
    }
}

FRESULT initFatFs() {
    //Mount SD Card
    FRESULT code = f_mount(&FatFs, "", 1);

    return code;
}

void cpr_init() {
    cart_init();
    FRESULT code = initFatFs();

    if (code == FR_OK) {
        cpr_Mounted = TRUE;
        memset(cpr_Texture, 0xFF, sizeof(cpr_Texture));
        cpr_checkfile();
    } else {
        return;
    }
}

