// VIBEHUB terminal cockpit alternate UI.
//
// Drop this file and its matching screen_grid.cpp into src/ to try it.
// The public surface intentionally matches the active UI so the poller
// and data layer do not need changes.

#pragma once

#include "data.h"

namespace ui {

void buildGridScreen();

void updateWeatherCard(const data::WeatherData& d);
void updateClaudeCard (const data::ClaudeData&  d);
void updateCodexCard  (const data::CodexData&   d);
void updateInboxCard  (const data::NewsData&    d);

constexpr int kWeatherIconPx = 112;
constexpr int kBrandIconPx   = 34;

void setWeatherIconPng(const uint8_t* png, size_t len);
void setClaudeIconPng (const uint8_t* png, size_t len);
void setCodexIconPng  (const uint8_t* png, size_t len);

}  // namespace ui
