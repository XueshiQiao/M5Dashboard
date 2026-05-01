#include "ui/registry.h"

#include <esp_heap_caps.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

constexpr int kMaxLayouts = 8;
const Layout* g_layouts[kMaxLayouts] = {};
int g_count = 0;
const Layout* g_active = nullptr;

// Cache the last snapshot of every data source. When we swap to a new
// layout the registry replays these so the new layout populates
// immediately, instead of sitting on placeholders until the next poll
// cycle (60 s for usage, 15 min for weather, 30 min for news).
data::WeatherData g_last_weather = {};
data::ClaudeData  g_last_claude  = {};
data::CodexData   g_last_codex   = {};
data::NewsData    g_last_news    = {};
bool g_have_weather = false;
bool g_have_claude  = false;
bool g_have_codex   = false;
bool g_have_news    = false;

constexpr size_t kIconCacheBytes  = 16 * 1024;   // weather PNG can hit ~14 KB
constexpr size_t kBrandCacheBytes =  8 * 1024;
uint8_t* g_weather_icon_cache = nullptr;
size_t   g_weather_icon_cache_len = 0;
uint8_t* g_claude_icon_cache = nullptr;
size_t   g_claude_icon_cache_len = 0;
uint8_t* g_codex_icon_cache = nullptr;
size_t   g_codex_icon_cache_len = 0;

void cacheBytes(uint8_t** slot, size_t* slot_len, size_t cap,
                const uint8_t* png, size_t len) {
  if (!*slot) {
    *slot = static_cast<uint8_t*>(
        heap_caps_malloc(cap, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (!*slot) return;
  }
  if (len > cap) return;
  memcpy(*slot, png, len);
  *slot_len = len;
}

}  // namespace

void registerLayout(const Layout* layout) {
  if (!layout || g_count >= kMaxLayouts) return;
  g_layouts[g_count++] = layout;
}

const Layout* findLayout(const char* name) {
  if (!name) return nullptr;
  for (int i = 0; i < g_count; ++i) {
    if (g_layouts[i] && g_layouts[i]->name &&
        strcmp(g_layouts[i]->name, name) == 0) {
      return g_layouts[i];
    }
  }
  return nullptr;
}

const Layout* activeLayout() { return g_active; }

bool activateLayout(const char* name) {
  const Layout* l = findLayout(name);
  if (!l) return false;

  // Tear down the previous layout: kill its lv_timers (which would
  // otherwise keep firing into widgets we're about to free), then wipe
  // every child of the active screen. The screen's own event callbacks
  // (e.g. the gesture handler in main.cpp) survive lv_obj_clean.
  if (g_active && g_active->destroy) g_active->destroy();
  lv_obj_clean(lv_screen_active());

  g_active = l;
  if (l->build) l->build();

  // Replay cached snapshots so the new layout shows real data the
  // instant it appears, rather than placeholders until the next poll.
  if (g_have_weather && l->onWeather) l->onWeather(g_last_weather);
  if (g_have_claude  && l->onClaude)  l->onClaude(g_last_claude);
  if (g_have_codex   && l->onCodex)   l->onCodex(g_last_codex);
  if (g_have_news    && l->onNews)    l->onNews(g_last_news);
  if (g_weather_icon_cache_len > 0 && l->onWeatherIcon)
    l->onWeatherIcon(g_weather_icon_cache, g_weather_icon_cache_len);
  if (g_claude_icon_cache_len > 0 && l->onClaudeIcon)
    l->onClaudeIcon(g_claude_icon_cache, g_claude_icon_cache_len);
  if (g_codex_icon_cache_len > 0 && l->onCodexIcon)
    l->onCodexIcon(g_codex_icon_cache, g_codex_icon_cache_len);

  return true;
}

bool activateNextLayout() {
  if (!g_active || g_count == 0) return false;
  int idx = -1;
  for (int i = 0; i < g_count; ++i) {
    if (g_layouts[i] == g_active) { idx = i; break; }
  }
  if (idx < 0) return false;
  return activateLayout(g_layouts[(idx + 1) % g_count]->name);
}

bool activatePreviousLayout() {
  if (!g_active || g_count == 0) return false;
  int idx = -1;
  for (int i = 0; i < g_count; ++i) {
    if (g_layouts[i] == g_active) { idx = i; break; }
  }
  if (idx < 0) return false;
  return activateLayout(g_layouts[(idx - 1 + g_count) % g_count]->name);
}

void deliverWeather(const data::WeatherData& d) {
  g_last_weather = d;
  g_have_weather = true;
  if (g_active && g_active->onWeather) g_active->onWeather(d);
}
void deliverClaude(const data::ClaudeData& d) {
  g_last_claude = d;
  g_have_claude = true;
  if (g_active && g_active->onClaude) g_active->onClaude(d);
}
void deliverCodex(const data::CodexData& d) {
  g_last_codex = d;
  g_have_codex = true;
  if (g_active && g_active->onCodex) g_active->onCodex(d);
}
void deliverNews(const data::NewsData& d) {
  g_last_news = d;
  g_have_news = true;
  if (g_active && g_active->onNews) g_active->onNews(d);
}

void deliverWeatherIcon(const uint8_t* png, size_t len) {
  cacheBytes(&g_weather_icon_cache, &g_weather_icon_cache_len, kIconCacheBytes, png, len);
  if (g_active && g_active->onWeatherIcon) g_active->onWeatherIcon(png, len);
}
void deliverClaudeIcon(const uint8_t* png, size_t len) {
  cacheBytes(&g_claude_icon_cache, &g_claude_icon_cache_len, kBrandCacheBytes, png, len);
  if (g_active && g_active->onClaudeIcon) g_active->onClaudeIcon(png, len);
}
void deliverCodexIcon(const uint8_t* png, size_t len) {
  cacheBytes(&g_codex_icon_cache, &g_codex_icon_cache_len, kBrandCacheBytes, png, len);
  if (g_active && g_active->onCodexIcon) g_active->onCodexIcon(png, len);
}

IconSizes activeIconSizes() {
  if (!g_active) return { 0, 0 };
  return g_active->iconSizes;
}

}  // namespace ui
