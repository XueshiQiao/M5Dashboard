// theme.h — dark, terminal-leaning palette + screen geometry constants.
//
// Coordinates assume landscape rotation: 1280 wide × 720 tall.
// Bottom 80px is the tab bar. Content area = 1280 × 640.

#pragma once
#include <M5Unified.h>

namespace ui {

// ---- screen geometry ------------------------------------------------------
constexpr int SCREEN_W   = 1280;
constexpr int SCREEN_H   = 720;
constexpr int TABBAR_H   = 80;
constexpr int CONTENT_H  = SCREEN_H - TABBAR_H;   // 640
constexpr int CONTENT_Y0 = 0;
constexpr int TABBAR_Y0  = CONTENT_H;

// ---- palette (RGB565) -----------------------------------------------------
// Anchored on a near-black background with cool greys; one accent per panel.
inline uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return M5.Display.color565(r, g, b);
}

inline uint16_t bg()         { return rgb(0x0A, 0x0E, 0x12); }  // page
inline uint16_t card()       { return rgb(0x14, 0x19, 0x1F); }  // panel fill
inline uint16_t card_alt()   { return rgb(0x18, 0x1E, 0x26); }  // header strip
inline uint16_t border()     { return rgb(0x2A, 0x31, 0x40); }  // 1px panel border
inline uint16_t divider()    { return rgb(0x1F, 0x26, 0x30); }  // tab bar separator

inline uint16_t text_hi()    { return rgb(0xE6, 0xED, 0xF3); }  // primary
inline uint16_t text_md()    { return rgb(0x8B, 0x98, 0xAB); }  // secondary
inline uint16_t text_lo()    { return rgb(0x5C, 0x67, 0x76); }  // dim labels

// Panel accents
inline uint16_t accent_wx()      { return rgb(0xF2, 0xC9, 0x4C); }  // amber — weather
inline uint16_t accent_claude()  { return rgb(0x6F, 0xCB, 0xFF); }  // sky cyan
inline uint16_t accent_codex()   { return rgb(0x80, 0xE2, 0x7E); }  // mint green
inline uint16_t accent_inbox()   { return rgb(0xFF, 0x6B, 0x9D); }  // pink

inline uint16_t status_ok()      { return accent_codex(); }
inline uint16_t status_warn()    { return accent_wx(); }
inline uint16_t status_bad()     { return rgb(0xFF, 0x6B, 0x6B); }

}  // namespace ui
