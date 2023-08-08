#define SCREEN_WD SCREEN_WIDTH
#define SCREEN_HT SCREEN_HEIGHT

#define PAINT_CANVAS_TW 64
#define PAINT_CANVAS_TH 64
#define PAINT_CANVAS_W  128
#define PAINT_CANVAS_H  128
#define PAINT_CANVAS_X  (SCREEN_WD/2 - PAINT_CANVAS_W/2)
#define PAINT_CANVAS_Y  (SCREEN_HT/2 - PAINT_CANVAS_H/2)

#define PAINT_PAL_W     16
#define PAINT_PAL_H     16
#define PAINT_PAL_SX    16
#define PAINT_PAL_X     (SCREEN_WD/2 - (PAINT_PAL_SX*16)/2)
#define PAINT_PAL_Y     (PAINT_CANVAS_Y+PAINT_CANVAS_H+2)

#define PAINT_TOOL_TW   16
#define PAINT_TOOL_TH   84
#define PAINT_TOOL_W    14
#define PAINT_TOOL_H    14
#define PAINT_TOOL_SY   15
#define PAINT_TOOL_X    (PAINT_CANVAS_X - 18)
#define PAINT_TOOL_YT   PAINT_CANVAS_Y
#define PAINT_TOOL_YW   (PAINT_CANVAS_Y+PAINT_CANVAS_H-PAINT_TOOL_H)

#define PAINT_BANNER_TW 240
#define PAINT_BANNER_TH 36
#define PAINT_BANNER_W  (PAINT_BANNER_TW/2)
#define PAINT_BANNER_H  (PAINT_BANNER_TH/2)
#define PAINT_BANNER_X  (SCREEN_WD/2 - PAINT_BANNER_W/2)
#define PAINT_BANNER_Y  28

#define PAINT_DONE_TW   32
#define PAINT_DONE_TH   15
#define PAINT_DONE_W    32
#define PAINT_DONE_H    15
#define PAINT_DONE_X    (PAINT_PAL_X+(PAINT_PAL_SX*16)-PAINT_DONE_W)
#define PAINT_DONE_Y    PAINT_BANNER_Y

#define PAINT_CLEAR_TW  16
#define PAINT_CLEAR_TH  13
#define PAINT_CLEAR_W   13
#define PAINT_CLEAR_H   13
#define PAINT_CLEAR_X   (PAINT_CANVAS_X - 17)
#define PAINT_CLEAR_Y   (PAINT_CANVAS_Y + PAINT_CANVAS_H/2 - 7)

#define txt_paint cpr_Texture
#define pal_paint ((u16 *)(txt_paint+2048))

extern u8 txt_paint[];

extern void paint_draw(void);
extern void paint_clear(void);
extern void paint_init(void);
extern int paint_update(void);
extern int paint_load(void);
extern int paint_save(void);

