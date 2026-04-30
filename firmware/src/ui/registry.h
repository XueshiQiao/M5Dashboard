#pragma once

#include "ui/layout.h"

namespace ui {

void registerLayout(const Layout* layout);
const Layout* findLayout(const char* name);
const Layout* activeLayout();
bool activateLayout(const char* name);

void deliverWeather(const data::WeatherData& d);
void deliverClaude (const data::ClaudeData&  d);
void deliverCodex  (const data::CodexData&   d);
void deliverNews   (const data::NewsData&    d);
void deliverWeatherIcon(const uint8_t* png, size_t len);
void deliverClaudeIcon (const uint8_t* png, size_t len);
void deliverCodexIcon  (const uint8_t* png, size_t len);

IconSizes activeIconSizes();

}  // namespace ui
