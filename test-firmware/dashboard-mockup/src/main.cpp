// M5Dashboard mockup — three switchable layouts (A/B/C) with fake panel
// data, real Wi-Fi + battery + clock in the status indicators.
//
// Architecture:
//   setup() initializes display in landscape, kicks off Wi-Fi (non-blocking),
//   draws the initial layout. loop() polls touch (tab-bar hit-test, idle wake),
//   refreshes the status snapshot once a second, and triggers a full repaint
//   when the layout changes or the visible status changed (clock minute,
//   Wi-Fi state, battery bucket).

#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <time.h>

#include "config.h"
#include "theme.h"
#include "layouts.h"

using namespace ui;

namespace {

// --- state ---------------------------------------------------------------
Layout         g_layout                 = Layout::CONSOLE;   // start on Plan C
uint32_t       g_last_activity_ms       = 0;
bool           g_dimmed                 = false;
bool           g_need_full_repaint      = true;              // layout/content+tabbar
bool           g_need_status_only       = false;             // Plan C status strip only
uint32_t       g_last_status_ms         = 0;
StatusSnapshot g_status                 = {};
StatusSnapshot g_status_painted         = {};                // last values we drew
int            g_battery_pct_filtered   = -1;                // glitch-rejected EMA

// --- tab bar -------------------------------------------------------------
struct TabBtn { int x; int w; const char* label; Layout layout; };
constexpr TabBtn kTabs[] = {
  {   0, 426, "GRID",    Layout::GLANCE_GRID },
  { 426, 426, "HERO",    Layout::HERO_STACK  },
  { 852, 428, "CONSOLE", Layout::CONSOLE     },
};

void drawTabBar() {
  auto& d = M5.Display;
  d.fillRect(0, TABBAR_Y0, SCREEN_W, TABBAR_H, ui::card());
  d.drawFastHLine(0, TABBAR_Y0, SCREEN_W, ui::border());

  for (size_t i = 0; i < sizeof(kTabs) / sizeof(kTabs[0]); ++i) {
    const auto& b = kTabs[i];
    const bool selected = (b.layout == g_layout);
    const uint16_t bg_c = selected ? ui::card_alt() : ui::card();
    const uint16_t fg_c = selected ? ui::text_hi()  : ui::text_md();

    d.fillRect(b.x + 1, TABBAR_Y0 + 1, b.w - 2, TABBAR_H - 2, bg_c);
    if (selected) {
      d.fillRect(b.x + 1, TABBAR_Y0 + 1, b.w - 2, 3, ui::accent_claude());
    }
    d.setTextColor(fg_c, bg_c);
    d.setTextDatum(middle_center);
    d.setFont(&fonts::FreeSansBold18pt7b);
    d.drawString(b.label, b.x + b.w / 2, TABBAR_Y0 + TABBAR_H / 2 + 2);

    if (i > 0) d.drawFastVLine(b.x, TABBAR_Y0 + 12, TABBAR_H - 24, ui::divider());
  }
}

// --- networking & status -------------------------------------------------

// Tab5's SDIO2 link to the ESP32-C6 uses non-default GPIO pins. arduino-esp32
// defaults target the ESP32-P4 EvalBoard, which is wired differently — without
// this override, `WiFi.begin()` fails with H_SDIO_DRV: card init failed.
// Pin map: docs.m5stack.com/en/arduino/m5tab5/wifi
constexpr int8_t TAB5_SDIO2_CLK = 12;
constexpr int8_t TAB5_SDIO2_CMD = 13;
constexpr int8_t TAB5_SDIO2_D0  = 11;
constexpr int8_t TAB5_SDIO2_D1  = 10;
constexpr int8_t TAB5_SDIO2_D2  =  9;
constexpr int8_t TAB5_SDIO2_D3  =  8;
constexpr int8_t TAB5_SDIO2_RST = 15;

void connectWifi() {
  WiFi.setPins(TAB5_SDIO2_CLK, TAB5_SDIO2_CMD,
               TAB5_SDIO2_D0, TAB5_SDIO2_D1, TAB5_SDIO2_D2, TAB5_SDIO2_D3,
               TAB5_SDIO2_RST);
  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg::WIFI_SSID, cfg::WIFI_PASSWORD);
  Serial.printf("[wifi] connecting to '%s' ...\n", cfg::WIFI_SSID);
}

void updateStatus(StatusSnapshot& s) {
  const bool was_connected = s.wifi_connected;
  s.wifi_connected = (WiFi.status() == WL_CONNECTED);
  s.wifi_rssi_dbm  = s.wifi_connected ? (int8_t)WiFi.RSSI() : 0;

  // Battery on the Tab5 is read via INA226 but the M5Unified port can return
  // either the USB Vbus rail (~4.3V → ~0%) or the pack rail (~8.4V → ~100%)
  // depending on which it samples — values flip between calls. Reject any
  // sudden swing > 30 points; otherwise lightly smooth.
  int raw = M5.Power.getBatteryLevel();
  if (raw < 0) raw = 0;
  if (raw > 100) raw = 100;
  if (g_battery_pct_filtered < 0) {
    g_battery_pct_filtered = raw;
  } else if (abs(raw - g_battery_pct_filtered) <= 30) {
    g_battery_pct_filtered = (g_battery_pct_filtered * 3 + raw) / 4;
  }
  s.battery_pct   = g_battery_pct_filtered;
  s.battery_volts = M5.Power.getBatteryVoltage() / 1000.0f;

  if (s.wifi_connected && !was_connected) {
    configTime(cfg::GMT_OFFSET_SEC, cfg::DST_OFFSET_SEC, cfg::NTP_SERVER);
    Serial.printf("[wifi] up: RSSI %d dBm  (NTP sync requested)\n", s.wifi_rssi_dbm);
  }

  struct tm tinfo;
  if (s.wifi_connected && getLocalTime(&tinfo, 5)) {
    s.time_synced  = true;
    s.clock_hh     = tinfo.tm_hour;
    s.clock_mm     = tinfo.tm_min;
    s.clock_dow    = tinfo.tm_wday;
    s.clock_month  = tinfo.tm_mon + 1;
    s.clock_day    = tinfo.tm_mday;
  } else {
    s.time_synced = false;
    const uint32_t t = millis() / 1000;
    s.clock_hh = (t / 3600) % 24;
    s.clock_mm = (t / 60)   % 60;
    s.clock_dow = 2;     // placeholder: Tuesday
    s.clock_month = 4;
    s.clock_day = 29;
  }
}

bool statusVisiblyChanged(const StatusSnapshot& a, const StatusSnapshot& b) {
  if (a.clock_hh != b.clock_hh) return true;
  if (a.clock_mm != b.clock_mm) return true;
  if (a.wifi_connected != b.wifi_connected) return true;
  // Battery intentionally NOT compared here: the filtered value still drifts
  // by 1-2 points between samples and that drift would force a repaint
  // every minute. The current value is shown next time another field
  // changes (clock minute, Wi-Fi state).
  return false;
}

void redrawFull() {
  M5.Display.fillRect(0, 0, SCREEN_W, CONTENT_H, ui::bg());
  switch (g_layout) {
    case Layout::GLANCE_GRID: drawLayoutA(g_status); break;
    case Layout::HERO_STACK:  drawLayoutB(g_status); break;
    case Layout::CONSOLE:     drawLayoutC(g_status); break;
  }
  drawTabBar();
  g_status_painted = g_status;
}

void redrawStatusOnly() {
  // Only Plan C has a visible status strip. Plans A & B don't show status.
  if (g_layout == Layout::CONSOLE) {
    drawStatusBarC(g_status);
  }
  g_status_painted = g_status;
}

}  // namespace

// =========================================================================

void setup() {
  auto m5cfg = M5.config();
  M5.begin(m5cfg);

  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=== M5Dashboard mockup ===");

  M5.Display.setRotation(1);                      // landscape; flip to 3 if USB-C wrong side
  M5.Display.setBrightness(cfg::BRIGHTNESS_ACTIVE);
  M5.Display.fillScreen(ui::bg());

  Serial.printf("Display: %dx%d  Touch: %s\n",
                M5.Display.width(), M5.Display.height(),
                M5.Touch.isEnabled() ? "yes" : "no");
  Serial.printf("Battery: %d%%  %.2fV\n",
                M5.Power.getBatteryLevel(),
                M5.Power.getBatteryVoltage() / 1000.0f);

  connectWifi();

  g_last_activity_ms = millis();
  updateStatus(g_status);
  redrawFull();

  M5.Speaker.tone(880, 60);
}

void loop() {
  M5.update();
  const uint32_t now = millis();

  // --- touch ---
  bool touched = false;
  int  tx = -1, ty = -1;
  for (size_t i = 0; i < M5.Touch.getCount(); ++i) {
    auto t = M5.Touch.getDetail(i);
    if (t.wasPressed()) { touched = true; tx = t.x; ty = t.y; break; }
  }

  if (touched) {
    const bool was_dimmed = g_dimmed;
    g_last_activity_ms = now;
    if (g_dimmed) {
      M5.Display.setBrightness(cfg::BRIGHTNESS_ACTIVE);
      g_dimmed = false;
    }
    if (!was_dimmed && ty >= TABBAR_Y0 && ty < SCREEN_H) {
      for (const auto& b : kTabs) {
        if (tx >= b.x && tx < b.x + b.w) {
          if (g_layout != b.layout) {
            g_layout = b.layout;
            g_need_full_repaint = true;
            Serial.printf("[ui] -> %s\n", b.label);
          }
          break;
        }
      }
    }
  }

  // --- idle dim ---
  if (!g_dimmed && (now - g_last_activity_ms) > cfg::IDLE_DIM_MS) {
    M5.Display.setBrightness(cfg::BRIGHTNESS_DIM);
    g_dimmed = true;
    Serial.println("[ui] idle dim");
  }

  // --- status refresh ---
  if (now - g_last_status_ms > 1000) {
    g_last_status_ms = now;
    updateStatus(g_status);
    // Only Plan C displays status visibly. For A & B we don't need any
    // repaint; the filtered battery is just held for next layout draw.
    if (g_layout == Layout::CONSOLE &&
        statusVisiblyChanged(g_status, g_status_painted)) {
      g_need_status_only = true;
    }
  }

  if (g_need_full_repaint) {
    g_need_full_repaint = false;
    g_need_status_only  = false;
    redrawFull();
  } else if (g_need_status_only) {
    g_need_status_only = false;
    redrawStatusOnly();
  }

  delay(20);
}
