// Terminal-style dashboard, ported to LVGL widgets.
//
// The public update API keeps the original card names so data_poller.cpp can
// continue to push weather, Claude, and Codex snapshots without knowing which
// visual layout is active.

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

// Kept for the poller interface. The terminal layout does not draw the PNG
// weather icon, but the fetch path still compiles against this constant.
constexpr int kWeatherIconPx = 128;

// No-op in the terminal layout; retained to keep the data polling layer stable.
void setWeatherIconPng(const uint8_t* png, size_t len);

}  // namespace ui
