// layouts.h — three full-screen content layouts (A/B/C). Each draws into
// the content area (0,0)-(1280,640). Tab bar is rendered separately by main.

#pragma once

namespace ui {

enum class Layout : uint8_t {
  GLANCE_GRID = 0,   // Plan A — 2x2 of equal tiles
  HERO_STACK  = 1,   // Plan B — left hero + right rail of 3 cards
  CONSOLE     = 2,   // Plan C — status bar + 2 big tiles + ticker
};

struct StatusSnapshot {
  bool     wifi_connected;
  int8_t   wifi_rssi_dbm;
  int      battery_pct;
  float    battery_volts;
  bool     time_synced;
  // hh,mm — if !time_synced these are computed from millis() since boot.
  uint8_t  clock_hh;
  uint8_t  clock_mm;
  uint8_t  clock_dow;       // 0=Sun..6=Sat (only meaningful when time_synced)
  uint8_t  clock_month;     // 1..12
  uint8_t  clock_day;       // 1..31
};

void drawLayoutA(const StatusSnapshot& s);
void drawLayoutB(const StatusSnapshot& s);
void drawLayoutC(const StatusSnapshot& s);

// Repaint just the 64px top status strip for Plan C in-place. Used for
// minute/Wi-Fi updates so the whole content area doesn't flicker.
void drawStatusBarC(const StatusSnapshot& s);

}  // namespace ui
