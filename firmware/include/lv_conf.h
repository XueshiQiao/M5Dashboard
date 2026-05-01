// lv_conf.h — minimal LVGL 9.x configuration for the M5Tab5 dashboard.
//
// We rely on lv_conf_internal.h's `#ifndef X #define X DEFAULT` chain to
// fill in everything we don't override. Only the settings that meaningfully
// shape *this* project are listed.
//
// The build_flag `-DLV_CONF_INCLUDE_SIMPLE` makes LVGL look for this file
// on the include path (PIO adds `include/` automatically).

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

// ---- color & rendering ---------------------------------------------------
#define LV_COLOR_DEPTH              16
// Keep LVGL's native (little-endian) RGB565 layout; the lgfx side does the
// byte-swap to whatever the panel wants via setSwapBytes(true). Doing the
// swap *both* places, or in neither, was producing cumulative per-row
// pixel skew that looked like vertical streaks worst at the bottom.
#define LV_COLOR_16_SWAP            0
#define LV_DRAW_BUF_ALIGN           4
#define LV_DRAW_BUF_STRIDE_ALIGN    1

// ---- HAL -----------------------------------------------------------------
#define LV_DEF_REFR_PERIOD          16     // ~60 FPS
#define LV_DPI_DEF                  130    // ~5", desktop reading distance
#define LV_USE_OS                   LV_OS_NONE   // we drive lv_timer_handler from loop()

// ---- memory --------------------------------------------------------------
// Use clib malloc/free instead of LVGL's builtin allocator. The builtin
// allocator places its full LV_MEM_SIZE pool in .bss, which on ESP32-P4
// with arduino-esp32's already-large BSS footprint exceeds internal SRAM
// and the linker drops sections. clib malloc draws from the system heap,
// which on this board can spill into the 32 MB PSRAM.
#define LV_USE_STDLIB_MALLOC        LV_STDLIB_CLIB
#define LV_USE_STDLIB_STRING        LV_STDLIB_CLIB
#define LV_USE_STDLIB_SPRINTF       LV_STDLIB_CLIB

// ---- diagnostics ---------------------------------------------------------
#define LV_USE_LOG                  1
#define LV_LOG_LEVEL                LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF               1
#define LV_USE_PERF_MONITOR         0
#define LV_USE_MEM_MONITOR          0
#define LV_USE_REFR_DEBUG           0

#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

// ---- widgets (defaults are mostly on; explicit for the ones we'll use) ---
#define LV_USE_LABEL                1
#define LV_LABEL_TEXT_SELECTION     1
#define LV_USE_BUTTON               1
#define LV_USE_IMAGE                1
#define LV_USE_LINE                 1
#define LV_USE_BAR                  1
#define LV_USE_SLIDER               1
#define LV_USE_CHART                1
#define LV_USE_LIST                 1
#define LV_USE_SCALE                1
#define LV_USE_SWITCH               1
#define LV_USE_TABVIEW              1
#define LV_USE_FLEX                 1
#define LV_USE_GRID                 1
#define LV_USE_OBSERVER             1

// ---- themes --------------------------------------------------------------
#define LV_USE_THEME_DEFAULT        1
#define LV_THEME_DEFAULT_DARK       1
#define LV_THEME_DEFAULT_GROW       1
#define LV_THEME_DEFAULT_TRANSITION_TIME  80

// ---- fonts ---------------------------------------------------------------
#define LV_FONT_MONTSERRAT_8        0
#define LV_FONT_MONTSERRAT_10       0
#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_14       1
#define LV_FONT_MONTSERRAT_16       1
#define LV_FONT_MONTSERRAT_18       1
#define LV_FONT_MONTSERRAT_20       1
#define LV_FONT_MONTSERRAT_22       1
#define LV_FONT_MONTSERRAT_24       1
#define LV_FONT_MONTSERRAT_26       0
#define LV_FONT_MONTSERRAT_28       1
#define LV_FONT_MONTSERRAT_30       1
#define LV_FONT_MONTSERRAT_32       1
#define LV_FONT_MONTSERRAT_36       1
#define LV_FONT_MONTSERRAT_40       1
#define LV_FONT_MONTSERRAT_44       0
#define LV_FONT_MONTSERRAT_48       1
#define LV_FONT_UNSCII_8            0
#define LV_FONT_UNSCII_16           1

#define LV_FONT_DEFAULT             &lv_font_montserrat_16

// ---- image decoders ------------------------------------------------------
// lodepng decodes PNG bytes from memory, used for the QWeather icons fetched
// from the dashboard server. ~10–15 KB flash; runs comfortably on the P4
// since icons are <5 KB and refresh at most every 15 min.
#define LV_USE_LODEPNG              1

// ---- compiler ------------------------------------------------------------
#define LV_BIG_ENDIAN_SYSTEM        0

#endif  // LV_CONF_H
