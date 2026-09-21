#ifndef GD_DEBUGGING_UTILS_H
#define GD_DEBUGGING_UTILS_H

#include <PR/ultratypes.h>

#include "gd_types.h"
#include "macros.h"

#define GD_NUM_MEM_TRACKERS 32
#define GD_NUM_TIMERS 32

// structs
struct MemTracker {
    /* 0x00 */ const char *name;  // name (used as an identifier)
    /* 0x04 */ f32 begin;         // used heap space (in bytes) before allocating memory
    /* 0x08 */ f32 end;           // used heap space (in bytes) after allocating memory
    /* 0x0C */ f32 total;         // total memory (in bytes) allocated between all start_memtracker/stop_memtracker calls
};

struct GdTimer {
    /* 0x00 */ s32 start;   // in cycles
    /* 0x04 */ s32 end;     // in cycles
    /* 0x08 */ s32 total;   // in cycles
    /* 0x0C */ f32 unused;
    /* 0x10 */ f32 scaledTotal;  // total / sTimeScaleFactor (1.0f) Unused function modified value
    /* 0x14 */ f32 prevScaledTotal;
    /* 0x18 */ const char *name;
    /* 0x1C */ s32 gadgetColourNum;  // color of gadget that represents timer?
    /* 0x20 */ s32 resetCount;
}; // sizeof = 0x24

union PrintVal {
    f32 f;
    s32 i;
    s64 pad;
};

/* based on fields set in gd_fopen; gd_malloc_perm(84) for size */
struct GdFile {
    /* 0x00 */ u8  filler1[4];
    /* 0x04 */ u32 pos;
    /* 0x08 */ s8 *stream;
    /* Known Flags for +0xC field:
    ** 1 : write mode
    ** 2 : binary mode
    ** 4 : eof */
    /* 0x0C */ u32 flags;
    /* 0x10 */ u8  filler2[64];
    /* 0x50 */ u32 size;
}; /* sizeof() = 0x54 */

// bss
extern u8 *gGdStreamBuffer;
void imin_impl(const char *);
#define imin(...) imin_impl(__FUNCTION__)
void imout(void);

// functions
struct MemTracker *start_memtracker(const char *);
u32 stop_memtracker(const char *);
void remove_all_memtrackers(void);
struct MemTracker *get_memtracker_by_index(s32);
void print_all_memtrackers(void);
void print_all_timers(void);
void deactivate_timing(void);
void activate_timing(void);
void remove_all_timers(void);
struct GdTimer *get_timer(const char *);
struct GdTimer *get_timernum(s32);
void start_timer(const char *);
void restart_timer(const char *);
void split_timer(const char *);
void stop_timer(const char *);
f32 get_scaled_timer_total(const char *);
void fatal_print(const char *) NORETURN;
void fatal_printf(const char *, ...) NORETURN;
f32 gd_rand_float(void);
s32 gd_atoi(const char *);
f64 gd_lazy_atof(const char *, u32 *);
char *sprint_val_withspecifiers(char *, union PrintVal, char *);
void gd_strcpy(char *, const char *);
char *gd_strdup(const char *);
u32 gd_strlen(const char *);
char *gd_strcat(char *, const char *);
s32 gd_str_not_equal(const char *, const char *);
s32 gd_str_contains(const char *, const char *);
s32 gd_feof(struct GdFile *);
struct GdFile *gd_fopen(const char *, const char *);
s32 gd_fread(s8 *, s32, s32, struct GdFile *);
void gd_fclose(struct GdFile *);
u32 gd_get_file_size(struct GdFile *);
s32 gd_fread_line(char *, u32, struct GdFile *);

#endif // GD_DEBUGGING_UTILS_H
