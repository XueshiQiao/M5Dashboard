#pragma once

#include <stddef.h>
#include <stdint.h>

#include "data.h"

namespace ui {

struct IconSizes {
  int weatherPx;
  int brandPx;
};

struct Layout {
  const char* name;

  void (*build)();
  void (*destroy)();   // called by registry before swap; tear down lv_timers etc.

  void (*onWeather)(const data::WeatherData&);
  void (*onClaude) (const data::ClaudeData&);
  void (*onCodex)  (const data::CodexData&);
  void (*onNews)   (const data::NewsData&);

  void (*onWeatherIcon)(const uint8_t* png, size_t len);
  void (*onClaudeIcon) (const uint8_t* png, size_t len);
  void (*onCodexIcon)  (const uint8_t* png, size_t len);

  IconSizes iconSizes;
};

}  // namespace ui
