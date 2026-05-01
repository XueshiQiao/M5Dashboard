// Terminal layout — registers as ui::kTerminalLayout.
//
// One screen, Tamzen 20x40 (2x bitmap scale of 10x20 Bold) monospace,
// ASCII-only glyphs. Compressed layout: at 38 px line height we only
// get ~18 rows in 720 px, so the M5DASH figlet is dropped, neofetch
// is folded into 2 dense rows, and the tail strip shrinks to 1 line.

#include "ui/layouts/terminal/terminal_layout.h"

#include "config.h"
#include "m5_io.h"
#include "ui/fonts/fonts.h"

#include <Arduino.h>
#include <esp_heap_caps.h>
#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

constexpr uint32_t kBg      = 0x000805;
constexpr uint32_t kPrompt  = 0x33FF77;
constexpr uint32_t kCmd     = 0xE7E7E0;
constexpr uint32_t kKey     = 0x77DDFF;
constexpr uint32_t kVal     = 0xE0E5DC;
constexpr uint32_t kAccent  = 0xFFC857;
constexpr uint32_t kMuted   = 0x80978B;
constexpr uint32_t kDim     = 0x4A6258;
constexpr uint32_t kGood    = 0x55FF7F;
constexpr uint32_t kWarn    = 0xFFD23F;
constexpr uint32_t kCrit    = 0xFF5757;

lv_font_t g_font_body;
const lv_font_t* kFont = &g_font_body;

// Tamzen 20x40 Bold (2× bitmap scale). adv_w = 20, line_height = 36.
constexpr int kCharW = 20;
constexpr int kRowH  = 38;
constexpr int kPadL  = 12;
constexpr int kPadT  = 4;

struct Handles {
  lv_obj_t* d_line1;     // uptime / wifi / ip
  lv_obj_t* d_line2;     // memory / battery / layout

  lv_obj_t* w_summary;   // 2 rows
  lv_obj_t* w_forecast;  // 1 row, all 5 days

  lv_obj_t* u_claude_id;
  lv_obj_t* u_claude_session;
  lv_obj_t* u_claude_weekly;
  lv_obj_t* u_codex_id;
  lv_obj_t* u_codex_session;
  lv_obj_t* u_codex_weekly;
};

Handles g_h = {};
lv_timer_t* g_status_timer = nullptr;

int row(int n) { return kPadT + n * kRowH; }
int col(int n) { return kPadL + n * kCharW; }

lv_obj_t* mkLabel(lv_obj_t* parent, const char* text, uint32_t color,
                  int x, int y, int w, int rows = 1) {
  lv_obj_t* l = lv_label_create(parent);
  lv_label_set_text(l, text);
  lv_label_set_long_mode(l, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_font(l, kFont, 0);
  lv_obj_set_style_text_color(l, lv_color_hex(color), 0);
  lv_obj_set_style_text_letter_space(l, 0, 0);
  lv_obj_set_style_text_line_space(l, 2, 0);
  lv_obj_set_pos(l, x, y);
  lv_obj_set_size(l, w, rows * kRowH);
  return l;
}

void setText(lv_obj_t* l, const char* text, uint32_t color) {
  if (!l) return;
  lv_label_set_text(l, text);
  lv_obj_set_style_text_color(l, lv_color_hex(color), 0);
}

uint32_t pctColor(int pct) {
  if (pct >= 90) return kCrit;
  if (pct >= 60) return kWarn;
  return kGood;
}

void renderBar(char* buf, size_t cap, int pct, int cells) {
  if (cap < (size_t)(cells + 3)) { if (cap) buf[0] = '\0'; return; }
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  int filled = (pct * cells + 50) / 100;
  if (filled > cells) filled = cells;
  size_t off = 0;
  buf[off++] = '[';
  for (int i = 0; i < cells; ++i) buf[off++] = (i < filled) ? '#' : '.';
  buf[off++] = ']';
  buf[off] = '\0';
}

void formatUptime(uint32_t ms, char* buf, size_t cap) {
  uint32_t s  = ms / 1000;
  uint32_t hh = s / 3600;
  uint32_t mm = (s / 60) % 60;
  uint32_t ss = s % 60;
  snprintf(buf, cap, "%02u:%02u:%02u",
           (unsigned)hh, (unsigned)mm, (unsigned)ss);
}

// ─── device info ─────────────────────────────────────────────────────────
void refreshDevice() {
  char up[16];
  formatUptime(millis(), up, sizeof(up));

  char ssid_rssi[64];
  if (m5io::wifiConnected()) {
    snprintf(ssid_rssi, sizeof(ssid_rssi), "%s @ %d dBm",
             cfg::WIFI_SSID, m5io::wifiRssi());
  } else {
    snprintf(ssid_rssi, sizeof(ssid_rssi), "disconnected");
  }

  char ip[24];
  m5io::wifiIpString(ip, sizeof(ip));

  char line1[128];
  snprintf(line1, sizeof(line1), "up %s   wifi %s   ip %s",
           up, ssid_rssi, ip);
  setText(g_h.d_line1, line1,
          m5io::wifiConnected() ? kVal : kCrit);

  size_t free_psram  = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  size_t total_psram = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  const auto pwr = m5io::powerState();
  char bbuf[24];
  if (pwr == m5io::PowerState::BatteryCable) {
    snprintf(bbuf, sizeof(bbuf), "%d%% [CHG]", m5io::batteryPct());
  } else if (pwr == m5io::PowerState::BatteryOnly) {
    snprintf(bbuf, sizeof(bbuf), "%d%%", m5io::batteryPct());
  } else if (pwr == m5io::PowerState::NoBatteryCable) {
    snprintf(bbuf, sizeof(bbuf), "USB");
  } else {
    snprintf(bbuf, sizeof(bbuf), "n/a");
  }

  char line2[128];
  snprintf(line2, sizeof(line2), "mem %u/%u MB   bat %s   layout terminal",
           (unsigned)(free_psram / (1024 * 1024)),
           (unsigned)(total_psram / (1024 * 1024)),
           bbuf);
  setText(g_h.d_line2, line2, kKey);
}

void statusTimerCb(lv_timer_t*) { refreshDevice(); }

// ─── builders ────────────────────────────────────────────────────────────
void mkPrompt(lv_obj_t* scr, int r, const char* cmd) {
  mkLabel(scr, "joey@m5tab5:~$", kPrompt, col(0), row(r),
          14 * kCharW + 4);
  mkLabel(scr, cmd, kCmd, col(14) + 8, row(r), 50 * kCharW);
}

void buildBanner(lv_obj_t* scr) {
  mkPrompt(scr, 0, "neofetch");
  g_h.d_line1 = mkLabel(scr, "up -   wifi -   ip -",
                        kVal, col(0), row(1), 64 * kCharW);
  g_h.d_line2 = mkLabel(scr, "mem -   bat -   layout terminal",
                        kKey, col(0), row(2), 64 * kCharW);
}

void buildWeather(lv_obj_t* scr) {
  mkPrompt(scr, 4, "weather --now");
  g_h.w_summary = mkLabel(scr,
      "city -   temp -   range -\n"
      "humidity -   wind -   aqi -",
      kVal, col(0), row(5), 64 * kCharW, 2);
  g_h.w_forecast = mkLabel(scr,
      "forecast: -",
      kAccent, col(0), row(7), 64 * kCharW);
}

void buildUsageBlock(lv_obj_t* scr, int r,
                     lv_obj_t** id_out, lv_obj_t** session_out, lv_obj_t** weekly_out,
                     const char* placeholder_id) {
  *id_out      = mkLabel(scr, placeholder_id, kAccent,
                         col(0), row(r), 64 * kCharW);
  *session_out = mkLabel(scr,
      "  session [..........]   -   reset -",
      kVal, col(0), row(r + 1), 64 * kCharW);
  *weekly_out  = mkLabel(scr,
      "  weekly  [..........]   -   reset -",
      kVal, col(0), row(r + 2), 64 * kCharW);
}

void buildUsage(lv_obj_t* scr) {
  mkPrompt(scr, 9, "usage --watch");
  buildUsageBlock(scr, 10, &g_h.u_claude_id, &g_h.u_claude_session, &g_h.u_claude_weekly,
                  "claude  waiting...");
  buildUsageBlock(scr, 13, &g_h.u_codex_id,  &g_h.u_codex_session,  &g_h.u_codex_weekly,
                  "codex   waiting...");
}

void destroy() {
  if (g_status_timer) {
    lv_timer_del(g_status_timer);
    g_status_timer = nullptr;
  }
  memset(&g_h, 0, sizeof(g_h));
}

void buildScreen() {
  g_font_body = lv_font_tamzen_40b;

  lv_obj_t* scr = lv_screen_active();
  // Registry already cleans the screen before invoking build().
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  buildBanner(scr);
  buildWeather(scr);
  buildUsage(scr);

  refreshDevice();
  g_status_timer = lv_timer_create(statusTimerCb, 1000, nullptr);
}

void onWeather(const data::WeatherData& d) {
  if (!d.valid || !g_h.w_summary) return;

  char body[256];
  if (d.air.present) {
    snprintf(body, sizeof(body),
             "%s  %d\xC2\xB0""C  %s  range %d/%d\xC2\xB0""C\n"
             "humidity %d%%  wind %d km/h  aqi %u %s",
             d.city, (int)d.tempC, d.condition,
             (int)d.highC, (int)d.lowC,
             (int)d.humidityPct, (int)d.windKmh,
             (unsigned)d.air.aqi, d.air.category);
  } else {
    snprintf(body, sizeof(body),
             "%s  %d\xC2\xB0""C  %s  range %d/%d\xC2\xB0""C\n"
             "humidity %d%%  wind %d km/h",
             d.city, (int)d.tempC, d.condition,
             (int)d.highC, (int)d.lowC,
             (int)d.humidityPct, (int)d.windKmh);
  }
  setText(g_h.w_summary, body, kVal);

  char fc[256];
  snprintf(fc, sizeof(fc),
           "%s %d/%d %s   %s %d/%d %s   %s %d/%d %s",
           d.forecast[0].day, (int)d.forecast[0].highC, (int)d.forecast[0].lowC, d.forecast[0].glyph,
           d.forecast[1].day, (int)d.forecast[1].highC, (int)d.forecast[1].lowC, d.forecast[1].glyph,
           d.forecast[2].day, (int)d.forecast[2].highC, (int)d.forecast[2].lowC, d.forecast[2].glyph);
  setText(g_h.w_forecast, fc, kAccent);
}

void renderUsageLine(lv_obj_t* lbl, const char* label, int pct, const char* reset) {
  if (!lbl) return;
  char bar[16];
  renderBar(bar, sizeof(bar), pct, 10);
  char line[128];
  snprintf(line, sizeof(line), "  %-7s %s %3d%%  reset %s",
           label, bar, pct, reset && reset[0] ? reset : "-");
  setText(lbl, line, pctColor(pct));
}

void renderUsageIdLine(lv_obj_t* lbl, const char* who, const data::ClaudeData& d) {
  if (!lbl) return;
  const char* email = d.emailMasked[0] ? d.emailMasked : (d.email[0] ? d.email : "-");
  const char* plan  = d.plan[0] ? d.plan : "-";
  char line[120];
  snprintf(line, sizeof(line), "%-7s %s %s",
           who, email, plan);
  setText(lbl, line, kAccent);
}

void onClaude(const data::ClaudeData& d) {
  if (!d.valid || !g_h.u_claude_id) return;
  renderUsageIdLine(g_h.u_claude_id, "claude", d);
  if (d.session.present) renderUsageLine(g_h.u_claude_session, "session",
                                         d.session.utilizationPct, d.session.resetIn);
  if (d.weekly.present)  renderUsageLine(g_h.u_claude_weekly,  "weekly",
                                         d.weekly.utilizationPct, d.weekly.resetIn);
}

void onCodex(const data::CodexData& d) {
  if (!d.valid || !g_h.u_codex_id) return;
  renderUsageIdLine(g_h.u_codex_id, "codex", d);
  if (d.session.present) renderUsageLine(g_h.u_codex_session, "session",
                                         d.session.utilizationPct, d.session.resetIn);
  if (d.weekly.present)  renderUsageLine(g_h.u_codex_weekly,  "weekly",
                                         d.weekly.utilizationPct, d.weekly.resetIn);
}

}  // namespace

const Layout kTerminalLayout = {
  "terminal",
  buildScreen,
  destroy,
  onWeather,
  onClaude,
  onCodex,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  { 0, 0 },
};

}  // namespace ui
