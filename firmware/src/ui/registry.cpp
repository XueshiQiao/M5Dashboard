#include "ui/registry.h"

#include <string.h>

namespace ui {
namespace {

constexpr int kMaxLayouts = 8;
const Layout* g_layouts[kMaxLayouts] = {};
int g_count = 0;
const Layout* g_active = nullptr;

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
  g_active = l;
  if (l->build) l->build();
  return true;
}

void deliverWeather(const data::WeatherData& d) {
  if (g_active && g_active->onWeather) g_active->onWeather(d);
}
void deliverClaude(const data::ClaudeData& d) {
  if (g_active && g_active->onClaude) g_active->onClaude(d);
}
void deliverCodex(const data::CodexData& d) {
  if (g_active && g_active->onCodex) g_active->onCodex(d);
}
void deliverNews(const data::NewsData& d) {
  if (g_active && g_active->onNews) g_active->onNews(d);
}
void deliverWeatherIcon(const uint8_t* png, size_t len) {
  if (g_active && g_active->onWeatherIcon) g_active->onWeatherIcon(png, len);
}
void deliverClaudeIcon(const uint8_t* png, size_t len) {
  if (g_active && g_active->onClaudeIcon) g_active->onClaudeIcon(png, len);
}
void deliverCodexIcon(const uint8_t* png, size_t len) {
  if (g_active && g_active->onCodexIcon) g_active->onCodexIcon(png, len);
}

IconSizes activeIconSizes() {
  if (!g_active) return { 0, 0 };
  return g_active->iconSizes;
}

}  // namespace ui
