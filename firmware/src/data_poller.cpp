// data_poller.cpp — background task that hits the dashboard server.
//
// Why a task: HTTPClient.GET() blocks. If we did it from the LVGL loop
// the UI would freeze for the whole request. The task runs on core 0
// (Arduino's loopTask is on core 1 by default), produces WeatherData /
// ClaudeData snapshots, and parks them behind a mutex. poller_drain()
// runs from loop() (single-threaded with LVGL), grabs anything new and
// hands it to the screen update functions.

#include "data_poller.h"

#include <Arduino.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "data.h"
#include "m5_io.h"
#include "screen_grid.h"

namespace {

// Server JSON for /api/weather is ~600 bytes; /api/claude is ~400. 8 KB
// gives plenty of headroom and stays in PSRAM-backed heap.
constexpr size_t kHttpBufSize = 8192;

// Polling cadence. Server-side memos are 60 s for claude/codex, 15 min
// for weather, 30 min for news — anything tighter than the server cache
// is wasted work.
constexpr uint32_t kClaudePollMs  = 60UL * 1000;
constexpr uint32_t kCodexPollMs   = 60UL * 1000;
constexpr uint32_t kWeatherPollMs = 15UL * 60 * 1000;
constexpr uint32_t kNewsPollMs    = 30UL * 60 * 1000;

SemaphoreHandle_t g_mu = nullptr;
data::WeatherData g_wx_snap     = {};
data::ClaudeData  g_claude_snap = {};
data::CodexData   g_codex_snap  = {};
data::NewsData    g_news_snap   = {};
bool              g_wx_dirty    = false;
bool              g_claude_dirty = false;
bool              g_codex_dirty  = false;
bool              g_news_dirty   = false;

// Weather icon staging. The poller task fetches PNG bytes into
// g_icon_stage_buf; poller_drain (LVGL thread) hands them to the screen.
constexpr size_t kIconStageBytes = 16 * 1024;
uint8_t* g_icon_stage_buf = nullptr;
size_t   g_icon_stage_len = 0;
bool     g_icon_dirty     = false;
char     g_icon_last_code[8] = "";   // last code we successfully fetched

// Brand-icon staging — one-shot per session for Claude and Codex. PNGs
// from /api/icon/{claude,codex} are tiny (~1.5–2 KB).
constexpr size_t kBrandStageBytes = 8 * 1024;
struct BrandStage {
  const char* name;          // url slug, "claude" or "codex"
  uint8_t*    buf;
  size_t      len;
  bool        dirty;
  bool        fetched;
};
BrandStage g_brand_claude = { "claude", nullptr, 0, false, false };
BrandStage g_brand_codex  = { "codex",  nullptr, 0, false, false };

bool fetchBrandIcon(BrandStage& b) {
  if (!b.buf) {
    b.buf = static_cast<uint8_t*>(
        heap_caps_malloc(kBrandStageBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (!b.buf) {
      Serial.printf("[poller] brand %s: PSRAM alloc failed\n", b.name);
      return false;
    }
  }
  char url[200];
  snprintf(url, sizeof(url),
           "%s/api/icon/%s?size=%d",
           cfg::SERVER_URL, b.name, ui::kBrandIconPx);
  size_t len = 0;
  if (!m5io::httpGetBinary(url, cfg::AUTH_TOKEN, b.buf, kBrandStageBytes, &len)) {
    Serial.printf("[poller] brand %s: HTTP failed\n", b.name);
    return false;
  }
  xSemaphoreTake(g_mu, portMAX_DELAY);
  b.len   = len;
  b.dirty = true;
  xSemaphoreGive(g_mu);
  Serial.printf("[poller] brand %s: %u bytes\n", b.name, (unsigned)len);
  return true;
}

bool fetchAndStageIcon(const char* code) {
  if (!code || !code[0]) return false;
  if (!g_icon_stage_buf) {
    g_icon_stage_buf = static_cast<uint8_t*>(
        heap_caps_malloc(kIconStageBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (!g_icon_stage_buf) {
      Serial.println("[poller] icon: PSRAM alloc failed");
      return false;
    }
  }
  char url[200];
  snprintf(url, sizeof(url),
           "%s/api/weather/icon/%s?size=%d",
           cfg::SERVER_URL, code, ui::kWeatherIconPx);
  size_t len = 0;
  if (!m5io::httpGetBinary(url, cfg::AUTH_TOKEN,
                           g_icon_stage_buf, kIconStageBytes, &len)) {
    Serial.printf("[poller] icon code=%s: HTTP failed\n", code);
    return false;
  }
  xSemaphoreTake(g_mu, portMAX_DELAY);
  g_icon_stage_len = len;
  g_icon_dirty     = true;
  strncpy(g_icon_last_code, code, sizeof(g_icon_last_code) - 1);
  g_icon_last_code[sizeof(g_icon_last_code) - 1] = '\0';
  xSemaphoreGive(g_mu);
  Serial.printf("[poller] icon code=%s bytes=%u\n", code, (unsigned)len);
  return true;
}

bool fetchAndParseWeather(char* buf, size_t cap) {
  char url[160];
  snprintf(url, sizeof(url), "%s/api/weather", cfg::SERVER_URL);
  if (!m5io::httpGetJson(url, cfg::AUTH_TOKEN, buf, cap)) {
    Serial.println("[poller] weather: HTTP failed");
    return false;
  }
  data::WeatherData w = {};
  if (!data::parseWeather(buf, &w)) {
    Serial.printf("[poller] weather: parse failed; first 120 chars: %.120s\n", buf);
    return false;
  }
  xSemaphoreTake(g_mu, portMAX_DELAY);
  g_wx_snap  = w;
  g_wx_dirty = true;
  // Decide whether to refetch the icon. We compare under the same lock
  // so g_icon_last_code stays consistent with what's actually been staged.
  bool need_icon = (strcmp(w.iconCode, g_icon_last_code) != 0);
  xSemaphoreGive(g_mu);
  Serial.printf("[poller] weather: %s %d°C %s (code=%s)%s",
                w.city, (int)w.tempC, w.condition, w.iconCode,
                w.air.present ? "" : "\n");
  if (w.air.present) {
    Serial.printf("  AQI %u %s\n", (unsigned)w.air.aqi, w.air.category);
  }
  if (need_icon) fetchAndStageIcon(w.iconCode);
  return true;
}

bool fetchAndParseClaude(char* buf, size_t cap) {
  char url[160];
  snprintf(url, sizeof(url), "%s/api/claude", cfg::SERVER_URL);
  if (!m5io::httpGetJson(url, cfg::AUTH_TOKEN, buf, cap)) {
    Serial.println("[poller] claude: HTTP failed");
    return false;
  }
  data::ClaudeData c = {};
  if (!data::parseClaude(buf, &c)) {
    Serial.printf("[poller] claude: parse failed; first 120 chars: %.120s\n", buf);
    return false;
  }
  xSemaphoreTake(g_mu, portMAX_DELAY);
  g_claude_snap  = c;
  g_claude_dirty = true;
  xSemaphoreGive(g_mu);
  Serial.printf("[poller] claude: %s plan=%s session=%d%% weekly=%d%%\n",
                c.emailMasked, c.plan,
                (int)c.session.utilizationPct, (int)c.weekly.utilizationPct);
  return true;
}

bool fetchAndParseNews(char* buf, size_t cap) {
  char url[160];
  snprintf(url, sizeof(url), "%s/api/news", cfg::SERVER_URL);
  if (!m5io::httpGetJson(url, cfg::AUTH_TOKEN, buf, cap)) {
    Serial.println("[poller] news: HTTP failed");
    return false;
  }
  data::NewsData n = {};
  if (!data::parseNews(buf, &n)) {
    Serial.printf("[poller] news: parse failed; first 120 chars: %.120s\n", buf);
    return false;
  }
  xSemaphoreTake(g_mu, portMAX_DELAY);
  g_news_snap  = n;
  g_news_dirty = true;
  xSemaphoreGive(g_mu);
  Serial.printf("[poller] news:   %u items\n", (unsigned)n.count);
  for (uint8_t i = 0; i < n.count; ++i) {
    Serial.printf("  > %.80s\n", n.items[i].title);
  }
  return true;
}

bool fetchAndParseCodex(char* buf, size_t cap) {
  char url[160];
  snprintf(url, sizeof(url), "%s/api/codex", cfg::SERVER_URL);
  if (!m5io::httpGetJson(url, cfg::AUTH_TOKEN, buf, cap)) {
    Serial.println("[poller] codex: HTTP failed");
    return false;
  }
  data::CodexData c = {};
  if (!data::parseCodex(buf, &c)) {
    Serial.printf("[poller] codex: parse failed; first 120 chars: %.120s\n", buf);
    return false;
  }
  xSemaphoreTake(g_mu, portMAX_DELAY);
  g_codex_snap  = c;
  g_codex_dirty = true;
  xSemaphoreGive(g_mu);
  Serial.printf("[poller] codex:  %s plan=%s session=%d%% weekly=%d%%\n",
                c.emailMasked, c.plan,
                (int)c.session.utilizationPct, (int)c.weekly.utilizationPct);
  return true;
}

void task_main(void* /*arg*/) {
  static char http_buf[kHttpBufSize];

  m5io::wifiBegin(cfg::WIFI_SSID, cfg::WIFI_PASSWORD);
  Serial.printf("[poller] WiFi.begin '%s' ...\n", cfg::WIFI_SSID);

  // Wait up to 30 s for association.
  uint32_t t0 = millis();
  while (!m5io::wifiConnected() && millis() - t0 < 30000) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  if (!m5io::wifiConnected()) {
    Serial.println("[poller] WiFi did not connect in 30 s — will keep retrying");
  } else {
    Serial.printf("[poller] WiFi up: RSSI %d dBm\n", m5io::wifiRssi());
  }

  uint32_t lastWeather = 0;
  uint32_t lastClaude  = 0;
  uint32_t lastCodex   = 0;
  uint32_t lastNews    = 0;

  while (true) {
    if (m5io::wifiConnected()) {
      uint32_t now = millis();
      // Brand icons: one-shot per session. Cheap retry on first wake.
      if (!g_brand_claude.fetched && fetchBrandIcon(g_brand_claude))
        g_brand_claude.fetched = true;
      if (!g_brand_codex.fetched  && fetchBrandIcon(g_brand_codex))
        g_brand_codex.fetched  = true;

      if (lastWeather == 0 || now - lastWeather >= kWeatherPollMs) {
        if (fetchAndParseWeather(http_buf, sizeof(http_buf))) lastWeather = now;
        else                                                  lastWeather = now - kWeatherPollMs + 5000;  // retry in 5 s
      }
      if (lastClaude == 0 || now - lastClaude >= kClaudePollMs) {
        if (fetchAndParseClaude(http_buf, sizeof(http_buf)))  lastClaude = now;
        else                                                  lastClaude = now - kClaudePollMs + 5000;
      }
      if (lastCodex == 0 || now - lastCodex >= kCodexPollMs) {
        if (fetchAndParseCodex(http_buf, sizeof(http_buf)))   lastCodex = now;
        else                                                  lastCodex = now - kCodexPollMs + 5000;
      }
      if (lastNews == 0 || now - lastNews >= kNewsPollMs) {
        if (fetchAndParseNews(http_buf, sizeof(http_buf)))    lastNews = now;
        else                                                  lastNews = now - kNewsPollMs + 5000;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

}  // namespace

void poller_start() {
  g_mu = xSemaphoreCreateMutex();
  // Pin to core 0; Arduino loopTask runs on core 1 by default.
  xTaskCreatePinnedToCore(task_main, "poller", 8192, nullptr, 5, nullptr, 0);
}

void poller_drain() {
  if (!g_mu) return;
  bool have_wx = false, have_cl = false, have_cx = false, have_news = false, have_icon = false;
  data::WeatherData wx;
  data::ClaudeData  cl;
  data::CodexData   cx;
  data::NewsData    nw;
  // Ferry the icon bytes via function-statics (in .bss, allocated once)
  // so we can hand them off to the LVGL side without holding the mutex
  // through lv_image_set_src.
  static uint8_t icon_copy [kIconStageBytes];
  static uint8_t cl_copy   [kBrandStageBytes];
  static uint8_t cx_copy   [kBrandStageBytes];
  size_t icon_len = 0;
  size_t cl_icon_len = 0, cx_icon_len = 0;
  bool have_cl_icon = false, have_cx_icon = false;

  xSemaphoreTake(g_mu, portMAX_DELAY);
  if (g_wx_dirty)     { wx = g_wx_snap;     g_wx_dirty = false;     have_wx   = true; }
  if (g_claude_dirty) { cl = g_claude_snap; g_claude_dirty = false; have_cl   = true; }
  if (g_codex_dirty)  { cx = g_codex_snap;  g_codex_dirty  = false; have_cx   = true; }
  if (g_news_dirty)   { nw = g_news_snap;   g_news_dirty   = false; have_news = true; }
  if (g_icon_dirty && g_icon_stage_buf) {
    icon_len = g_icon_stage_len;
    if (icon_len > sizeof(icon_copy)) icon_len = sizeof(icon_copy);
    memcpy(icon_copy, g_icon_stage_buf, icon_len);
    g_icon_dirty = false;
    have_icon = true;
  }
  if (g_brand_claude.dirty && g_brand_claude.buf) {
    cl_icon_len = g_brand_claude.len;
    if (cl_icon_len > sizeof(cl_copy)) cl_icon_len = sizeof(cl_copy);
    memcpy(cl_copy, g_brand_claude.buf, cl_icon_len);
    g_brand_claude.dirty = false;
    have_cl_icon = true;
  }
  if (g_brand_codex.dirty && g_brand_codex.buf) {
    cx_icon_len = g_brand_codex.len;
    if (cx_icon_len > sizeof(cx_copy)) cx_icon_len = sizeof(cx_copy);
    memcpy(cx_copy, g_brand_codex.buf, cx_icon_len);
    g_brand_codex.dirty = false;
    have_cx_icon = true;
  }
  xSemaphoreGive(g_mu);

  if (have_wx)        ui::updateWeatherCard(wx);
  if (have_cl)        ui::updateClaudeCard(cl);
  if (have_cx)        ui::updateCodexCard(cx);
  if (have_news)      ui::updateInboxCard(nw);
  if (have_icon)      ui::setWeatherIconPng(icon_copy, icon_len);
  if (have_cl_icon)   ui::setClaudeIconPng(cl_copy, cl_icon_len);
  if (have_cx_icon)   ui::setCodexIconPng (cx_copy, cx_icon_len);
}
