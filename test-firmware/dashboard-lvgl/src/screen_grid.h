// Plan A - Glance Grid screen, ported to LVGL widgets.
//
// 2x2 grid of tiles inside the 1280x720 landscape display:
//   TL: Weather card
//   TR: Claude Code usage card
//   BL: Codex usage card
//   BR: Inbox preview

#pragma once

#include "data.h"

namespace ui {

void buildGridScreen();

// Apply server data to the already-built cards. Safe to call from the
// LVGL task context (call site holds the LVGL lock — see lvgl_bridge).
// If d.valid is false, the call is a no-op (last good values stay on screen).
void updateWeatherCard(const data::WeatherData& d);
void updateClaudeCard (const data::ClaudeData&  d);
void updateCodexCard  (const data::CodexData&   d);
void updateInboxCard  (const data::NewsData&    d);

// Pixel sizes of the embedded brand/weather icons. Used by the fetcher
// so the PNGs requested from /api/weather/icon/:code and /api/icon/:name
// match the lv_image boxes (no LVGL scaling = sharper output).
constexpr int kWeatherIconPx = 128;
constexpr int kBrandIconPx   = 44;

// Hand a freshly-fetched PNG to the matching widget. Bytes are copied
// into internally-owned PSRAM; the caller's `png` may be freed/reused
// immediately. Length 0 clears the widget.
void setWeatherIconPng(const uint8_t* png, size_t len);
void setClaudeIconPng (const uint8_t* png, size_t len);
void setCodexIconPng  (const uint8_t* png, size_t len);

}  // namespace ui
