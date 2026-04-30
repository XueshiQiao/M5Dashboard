// data.{h,cpp} — value types for the server payloads + JSON parsers.
//
// Keep these plain C++: no LVGL, no M5Unified. Both screen_grid (LVGL side)
// and the polling layer (m5_io / main) include this and pass instances by
// value/reference.

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace data {

// --- Weather ---------------------------------------------------------------

struct ForecastDay {
  char     day[6];      // "MON"..."SUN" or "—"
  char     glyph[8];    // SUN / PCLDY / RAIN / SNOW / STORM / FOG
  char     iconCode[8]; // QWeather 3-digit code, e.g. "100"
  int16_t  highC;
  int16_t  lowC;
};

struct AirQuality {
  bool     present;
  uint16_t aqi;          // numeric AQI (US-EPA scale, 0–500)
  char     category[16]; // "Good" / "Moderate" / "Unhealthy" / etc.
  char     primary [16]; // primary pollutant short name, e.g. "PM 2.5"
  uint8_t  r, g, b;      // QWeather's recommended badge color
};

struct WeatherData {
  bool     valid;
  char     city[40];
  int16_t  tempC;
  char     condition[24];
  char     glyph[8];
  char     iconCode[8]; // QWeather 3-digit code for the current condition
  int16_t  windKmh;
  int16_t  humidityPct;
  int16_t  highC;
  int16_t  lowC;
  AirQuality  air;
  ForecastDay forecast[5];
};

// Returns true on success; on failure, out->valid is false.
bool parseWeather(const char* json, WeatherData* out);

// --- Claude usage ----------------------------------------------------------

struct UsageSlice {
  bool    present;
  int16_t utilizationPct;
  char    resetIn[24];   // "3h 6m to reset", "Mon 06:00 reset"
};

struct ClaudeData {
  bool       valid;
  char       email[64];
  char       emailMasked[40];
  char       plan[16];
  UsageSlice session;
  UsageSlice weekly;
  bool       extraEnabled;
};

// Codex's /api/codex returns the same envelope shape (email/plan/session/
// weekly/extraUsage) so we share the value type. parseCodex is a thin
// alias for parseClaude so call sites read by intent.
using CodexData = ClaudeData;

bool parseClaude(const char* json, ClaudeData* out);
bool parseCodex (const char* json, CodexData*  out);

// --- AI news (Hacker News via the server) ----------------------------------

struct NewsItem {
  char title[96];   // truncated to fit one line on the inbox card
  int  score;
};

struct NewsData {
  bool      valid;
  uint8_t   count;       // number of populated items in `items` (0..3)
  NewsItem  items[3];    // inbox card has 3 slots
};

bool parseNews(const char* json, NewsData* out);

}  // namespace data
