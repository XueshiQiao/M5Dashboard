// Terminal layout — registers as ui::kTerminalLayout.
//
// One screen, Tamzen 10x20 monospace, ASCII-only glyphs (no fancy box
// drawing or block elements that the bitmap font lacks). Reads top-down
// like a real terminal session: neofetch banner with system info,
// `$ weather --now` pane, `$ usage --watch` pane with ASCII progress
// bars, and a live tail of the most recent poller events.

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

// ─── theme ───────────────────────────────────────────────────────────────
constexpr uint32_t kBg      = 0x000805;
constexpr uint32_t kPrompt  = 0x33FF77;
constexpr uint32_t kCmd     = 0xE7E7E0;
constexpr uint32_t kKey     = 0x77DDFF;
constexpr uint32_t kVal     = 0xE0E5DC;
constexpr uint32_t kAccent  = 0xFFC857;
constexpr uint32_t kLogo    = 0x4FFF8E;
constexpr uint32_t kMuted   = 0x80978B;
constexpr uint32_t kDim     = 0x4A6258;
constexpr uint32_t kGood    = 0x55FF7F;
constexpr uint32_t kWarn    = 0xFFD23F;
constexpr uint32_t kCrit    = 0xFF5757;

// Body font is Tamzen 10x20 — readable on the Tab5 5" panel and stays
// monospace. No fallback needed: every glyph we draw is plain ASCII or
// Latin-1 (Tamzen covers 0x20-0xFF).
lv_font_t g_font_body;
const lv_font_t* kFont = &g_font_body;

constexpr int kCharW = 10;
constexpr int kRowH  = 22;
constexpr int kPadL  = 18;
constexpr int kPadT  = 6;

// ─── handles ─────────────────────────────────────────────────────────────
struct Handles {
  lv_obj_t* d_uptime;
  lv_obj_t* d_resolution;
  lv_obj_t* d_layout;
  lv_obj_t* d_shell;
  lv_obj_t* d_wifi;
  lv_obj_t* d_ip;
  lv_obj_t* d_memory;
  lv_obj_t* d_brightness;
  lv_obj_t* d_battery;

  lv_obj_t* w_summary;
  lv_obj_t* w_aqi;
  lv_obj_t* w_forecast;

  lv_obj_t* u_claude_id;
  lv_obj_t* u_claude_session;
  lv_obj_t* u_claude_weekly;
  lv_obj_t* u_codex_id;
  lv_obj_t* u_codex_session;
  lv_obj_t* u_codex_weekly;

  lv_obj_t* t_lines[3];
};

Handles g_h = {};
lv_timer_t* g_status_timer = nullptr;

constexpr int kTailRows = 3;
char g_tail[kTailRows][120] = { "", "", "" };

// ─── primitives ──────────────────────────────────────────────────────────
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

// `[####################....]` ASCII bar. cells = inner cell count.
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
  if (hh >= 24) {
    uint32_t dd = hh / 24;
    hh = hh % 24;
    snprintf(buf, cap, "%ud %02u:%02u:%02u",
             (unsigned)dd, (unsigned)hh, (unsigned)mm, (unsigned)ss);
  } else {
    snprintf(buf, cap, "%02u:%02u:%02u",
             (unsigned)hh, (unsigned)mm, (unsigned)ss);
  }
}

void clockStamp(char* buf, size_t cap) {
  uint32_t s  = millis() / 1000;
  uint32_t hh = (s / 3600) % 24;
  uint32_t mm = (s / 60) % 60;
  uint32_t ss = s % 60;
  snprintf(buf, cap, "%02u:%02u:%02u",
           (unsigned)hh, (unsigned)mm, (unsigned)ss);
}

// `host ........ value` neofetch-style. key column is fixed at 12 chars.
void writeKv(char* buf, size_t cap, const char* key, const char* value) {
  constexpr int kKeyCol = 12;
  int klen = (int)strlen(key);
  if (klen > kKeyCol) klen = kKeyCol;
  int dots = kKeyCol - klen;
  if (dots < 1) dots = 1;
  size_t off = 0;
  for (int i = 0; i < klen && off + 1 < cap; ++i) buf[off++] = key[i];
  buf[off++] = ' ';
  for (int i = 0; i < dots && off + 1 < cap; ++i) buf[off++] = '.';
  buf[off++] = ' ';
  size_t vlen = strlen(value);
  if (off + vlen + 1 >= cap) vlen = cap > off + 1 ? cap - off - 1 : 0;
  memcpy(buf + off, value, vlen);
  off += vlen;
  buf[off] = '\0';
}

// ─── tail strip ─────────────────────────────────────────────────────────
void pushTail(const char* line) {
  for (int i = kTailRows - 1; i > 0; --i) {
    strncpy(g_tail[i], g_tail[i - 1], sizeof(g_tail[i]) - 1);
    g_tail[i][sizeof(g_tail[i]) - 1] = '\0';
  }
  strncpy(g_tail[0], line, sizeof(g_tail[0]) - 1);
  g_tail[0][sizeof(g_tail[0]) - 1] = '\0';
  for (int i = 0; i < kTailRows; ++i) {
    if (g_h.t_lines[i]) {
      lv_label_set_text(g_h.t_lines[i], g_tail[i]);
      lv_obj_set_style_text_color(g_h.t_lines[i],
          lv_color_hex(i == 0 ? kVal : kMuted), 0);
    }
  }
}

void logEvent(const char* msg) {
  char ts[16];
  clockStamp(ts, sizeof(ts));
  char line[120];
  snprintf(line, sizeof(line), "[%s] %s", ts, msg);
  pushTail(line);
}

// ─── device info ─────────────────────────────────────────────────────────
void refreshDevice() {
  char up[24];
  formatUptime(millis(), up, sizeof(up));
  char buf[96];

  writeKv(buf, sizeof(buf), "uptime", up);
  setText(g_h.d_uptime, buf, kVal);

  if (m5io::wifiConnected()) {
    char wbuf[64];
    snprintf(wbuf, sizeof(wbuf), "%s @ %d dBm",
             cfg::WIFI_SSID, m5io::wifiRssi());
    writeKv(buf, sizeof(buf), "wifi", wbuf);
    setText(g_h.d_wifi, buf, kVal);

    char ip[24];
    m5io::wifiIpString(ip, sizeof(ip));
    writeKv(buf, sizeof(buf), "ip", ip);
    setText(g_h.d_ip, buf, kVal);
  } else {
    writeKv(buf, sizeof(buf), "wifi", "disconnected");
    setText(g_h.d_wifi, buf, kCrit);
    writeKv(buf, sizeof(buf), "ip", "0.0.0.0");
    setText(g_h.d_ip, buf, kDim);
  }

  size_t free_psram  = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  size_t total_psram = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  char mbuf[48];
  snprintf(mbuf, sizeof(mbuf), "%u / %u MB psram",
           (unsigned)(free_psram / (1024 * 1024)),
           (unsigned)(total_psram / (1024 * 1024)));
  writeKv(buf, sizeof(buf), "memory", mbuf);
  setText(g_h.d_memory, buf, kVal);

  char br[16];
  snprintf(br, sizeof(br), "%d / 255", cfg::BRIGHTNESS_ACTIVE);
  writeKv(buf, sizeof(buf), "brightness", br);
  setText(g_h.d_brightness, buf, kVal);

  char bbuf[48];
  const auto pwr = m5io::powerState();
  if (pwr == m5io::PowerState::BatteryCable) {
    snprintf(bbuf, sizeof(bbuf), "%d %% [CHG]", m5io::batteryPct());
  } else if (pwr == m5io::PowerState::BatteryOnly) {
    snprintf(bbuf, sizeof(bbuf), "%d %%", m5io::batteryPct());
  } else if (pwr == m5io::PowerState::NoBatteryCable) {
    snprintf(bbuf, sizeof(bbuf), "USB only");
  } else {
    snprintf(bbuf, sizeof(bbuf), "n/a");
  }
  writeKv(buf, sizeof(buf), "battery", bbuf);
  setText(g_h.d_battery, buf, kVal);
}

void statusTimerCb(lv_timer_t*) { refreshDevice(); }

// ─── builders ────────────────────────────────────────────────────────────
void mkPrompt(lv_obj_t* scr, int r, const char* cmd, int c0 = 0) {
  mkLabel(scr, "joey@m5tab5:~$", kPrompt, col(c0), row(r), 14 * kCharW + 4);
  mkLabel(scr, cmd, kCmd, col(c0 + 14) + 4, row(r), 50 * kCharW);
}

void buildBanner(lv_obj_t* scr) {
  mkPrompt(scr, 0, "neofetch");

  // ASCII figlet for "M5DASH" — 5 rows, ASCII-only, fits Tamzen perfectly.
  static const char* kAsciiLogo =
      " __  __ ____  ____    _    ____  _   _ \n"
      "|  \\/  | ___||  _ \\  / \\  / ___|| | | |\n"
      "| |\\/| |___ \\| | | |/ _ \\ \\___ \\| |_| |\n"
      "| |  | |___ )| |_| / ___ \\ ___) |  _  |\n"
      "|_|  |_|____/|____/_/   \\_\\____/|_| |_|";
  mkLabel(scr, kAsciiLogo, kLogo, col(0), row(2), 42 * kCharW, 5);

  // Right column starts beside the logo.
  constexpr int kInfoCol = 44;
  const int kInfoX = col(kInfoCol);
  const int kInfoW = 1280 - kInfoX - 12;

  char buf[96];
  writeKv(buf, sizeof(buf), "os",   "m5dashboard 1.0  (esp32-p4)");
  mkLabel(scr, buf, kKey, kInfoX, row(2), kInfoW);
  writeKv(buf, sizeof(buf), "host", "m5stack tab5  /  esp32-p4 + c6");
  mkLabel(scr, buf, kKey, kInfoX, row(3), kInfoW);

  g_h.d_uptime     = mkLabel(scr, "uptime ........ -",        kVal, kInfoX, row(4),  kInfoW);
  g_h.d_resolution = mkLabel(scr, "resolution .... 1280x720", kVal, kInfoX, row(5),  kInfoW);
  g_h.d_layout     = mkLabel(scr, "layout ........ terminal", kVal, kInfoX, row(6),  kInfoW);
  g_h.d_shell      = mkLabel(scr, "shell ......... -",        kVal, kInfoX, row(7),  kInfoW);
  g_h.d_wifi       = mkLabel(scr, "wifi .......... -",        kVal, kInfoX, row(8),  kInfoW);
  g_h.d_ip         = mkLabel(scr, "ip ............ -",        kVal, kInfoX, row(9),  kInfoW);
  g_h.d_memory     = mkLabel(scr, "memory ........ -",        kVal, kInfoX, row(10), kInfoW);
  g_h.d_brightness = mkLabel(scr, "brightness .... -",        kVal, kInfoX, row(11), kInfoW);
  g_h.d_battery    = mkLabel(scr, "battery ....... -",        kVal, kInfoX, row(12), kInfoW);

  writeKv(buf, sizeof(buf), "shell", cfg::SERVER_URL);
  setText(g_h.d_shell, buf, kVal);
}

void buildWeather(lv_obj_t* scr) {
  mkPrompt(scr, 14, "weather --now");

  g_h.w_summary = mkLabel(scr,
      "city ......... waiting...\n"
      "temp ......... -\n"
      "range ........ -\n"
      "humidity ..... -\n"
      "wind ......... -",
      kVal, col(0), row(15), 60 * kCharW, 5);

  g_h.w_aqi = mkLabel(scr, "aqi .......... -",
                      kMuted, col(0), row(20), 60 * kCharW);

  g_h.w_forecast = mkLabel(scr,
      "forecast --5d\n"
      "  -\n  -\n  -\n  -\n  -",
      kAccent, col(60), row(15), 50 * kCharW, 6);
}

void buildUsageBlock(lv_obj_t* scr, int r,
                     lv_obj_t** id_out, lv_obj_t** session_out, lv_obj_t** weekly_out,
                     const char* placeholder_id) {
  *id_out      = mkLabel(scr, placeholder_id, kAccent,
                         col(0), row(r), 110 * kCharW);
  *session_out = mkLabel(scr,
      "  session  [....................]    -    reset      -",
      kVal, col(0), row(r + 1), 110 * kCharW);
  *weekly_out  = mkLabel(scr,
      "  weekly   [....................]    -    reset      -",
      kVal, col(0), row(r + 2), 110 * kCharW);
}

void buildUsage(lv_obj_t* scr) {
  mkPrompt(scr, 22, "usage --watch");
  buildUsageBlock(scr, 23, &g_h.u_claude_id, &g_h.u_claude_session, &g_h.u_claude_weekly,
                  "claude  waiting for /api/claude...");
  buildUsageBlock(scr, 26, &g_h.u_codex_id,  &g_h.u_codex_session,  &g_h.u_codex_weekly,
                  "codex   waiting for /api/codex...");
}

void buildTail(lv_obj_t* scr) {
  mkPrompt(scr, 30, "tail -f /var/log/poller");
  for (int i = 0; i < kTailRows; ++i) {
    g_h.t_lines[i] = mkLabel(scr, "  (waiting for events)",
                             i == 0 ? kVal : kMuted,
                             col(2), row(31 + i), 122 * kCharW);
  }
}

// ─── build / callbacks ───────────────────────────────────────────────────
void buildScreen() {
  g_font_body = lv_font_tamzen_20;

  lv_obj_t* scr = lv_screen_active();
  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  buildBanner(scr);
  buildWeather(scr);
  buildUsage(scr);
  buildTail(scr);

  refreshDevice();

  if (g_status_timer) lv_timer_del(g_status_timer);
  g_status_timer = lv_timer_create(statusTimerCb, 1000, nullptr);
}

void onWeather(const data::WeatherData& d) {
  if (!d.valid || !g_h.w_summary) return;

  char body[256];
  snprintf(body, sizeof(body),
           "city ......... %s\n"
           "temp ......... %d\xC2\xB0""C  %s\n"
           "range ........ %d / %d\xC2\xB0""C\n"
           "humidity ..... %d %%\n"
           "wind ......... %d km/h",
           d.city, (int)d.tempC, d.condition,
           (int)d.highC, (int)d.lowC,
           (int)d.humidityPct, (int)d.windKmh);
  setText(g_h.w_summary, body, kVal);

  if (d.air.present) {
    char aqi[80];
    snprintf(aqi, sizeof(aqi), "aqi .......... %u  %s  /  %s",
             (unsigned)d.air.aqi, d.air.category, d.air.primary);
    uint32_t hex = ((uint32_t)d.air.r << 16) |
                   ((uint32_t)d.air.g << 8) |
                   (uint32_t)d.air.b;
    setText(g_h.w_aqi, aqi, hex);
  } else {
    setText(g_h.w_aqi, "aqi .......... -", kDim);
  }

  char fc[256];
  snprintf(fc, sizeof(fc),
           "forecast --5d\n"
           "  %-3s  %3d / %-3d   %s\n"
           "  %-3s  %3d / %-3d   %s\n"
           "  %-3s  %3d / %-3d   %s\n"
           "  %-3s  %3d / %-3d   %s\n"
           "  %-3s  %3d / %-3d   %s",
           d.forecast[0].day, (int)d.forecast[0].highC, (int)d.forecast[0].lowC, d.forecast[0].glyph,
           d.forecast[1].day, (int)d.forecast[1].highC, (int)d.forecast[1].lowC, d.forecast[1].glyph,
           d.forecast[2].day, (int)d.forecast[2].highC, (int)d.forecast[2].lowC, d.forecast[2].glyph,
           d.forecast[3].day, (int)d.forecast[3].highC, (int)d.forecast[3].lowC, d.forecast[3].glyph,
           d.forecast[4].day, (int)d.forecast[4].highC, (int)d.forecast[4].lowC, d.forecast[4].glyph);
  setText(g_h.w_forecast, fc, kAccent);

  char ev[80];
  snprintf(ev, sizeof(ev), "weather  %s  %d\xC2\xB0""C  %s",
           d.city, (int)d.tempC, d.condition);
  logEvent(ev);
}

void renderUsageLine(lv_obj_t* lbl, const char* label, int pct, const char* reset) {
  if (!lbl) return;
  char bar[32];
  renderBar(bar, sizeof(bar), pct, 20);
  char line[160];
  snprintf(line, sizeof(line), "  %-7s  %s  %3d %%  reset %s",
           label, bar, pct, reset && reset[0] ? reset : "-");
  setText(lbl, line, pctColor(pct));
}

void renderUsageIdLine(lv_obj_t* lbl, const char* who, const data::ClaudeData& d) {
  if (!lbl) return;
  const char* email = d.emailMasked[0] ? d.emailMasked : (d.email[0] ? d.email : "-");
  const char* plan  = d.plan[0] ? d.plan : "-";
  char line[120];
  snprintf(line, sizeof(line), "%-7s  %-22s  %-8s  extra: %s",
           who, email, plan, d.extraEnabled ? "on" : "off");
  setText(lbl, line, kAccent);
}

void onClaude(const data::ClaudeData& d) {
  if (!d.valid || !g_h.u_claude_id) return;
  renderUsageIdLine(g_h.u_claude_id, "claude", d);
  if (d.session.present) {
    renderUsageLine(g_h.u_claude_session, "session",
                    d.session.utilizationPct, d.session.resetIn);
  }
  if (d.weekly.present) {
    renderUsageLine(g_h.u_claude_weekly, "weekly",
                    d.weekly.utilizationPct, d.weekly.resetIn);
  }
  char ev[96];
  snprintf(ev, sizeof(ev), "claude   session %d %%   weekly %d %%",
           (int)d.session.utilizationPct, (int)d.weekly.utilizationPct);
  logEvent(ev);
}

void onCodex(const data::CodexData& d) {
  if (!d.valid || !g_h.u_codex_id) return;
  renderUsageIdLine(g_h.u_codex_id, "codex", d);
  if (d.session.present) {
    renderUsageLine(g_h.u_codex_session, "session",
                    d.session.utilizationPct, d.session.resetIn);
  }
  if (d.weekly.present) {
    renderUsageLine(g_h.u_codex_weekly, "weekly",
                    d.weekly.utilizationPct, d.weekly.resetIn);
  }
  char ev[96];
  snprintf(ev, sizeof(ev), "codex    session %d %%   weekly %d %%",
           (int)d.session.utilizationPct, (int)d.weekly.utilizationPct);
  logEvent(ev);
}

}  // namespace

const Layout kTerminalLayout = {
  "terminal",
  buildScreen,
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
