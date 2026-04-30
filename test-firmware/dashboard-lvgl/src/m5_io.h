// m5_io.h — thin C++ facade over M5Unified, with NO M5Unified/M5GFX types
// in its public surface.
//
// Why:
//   M5GFX bundles its own miniature LVGL-compat headers under
//   lgfx/v1/lv_font/{area,color,draw_buf}.h. Those define lv_area_t,
//   LV_COLOR_FORMAT_*, etc. with different guard names than LVGL itself.
//   Including <M5Unified.h> AND <lvgl.h> in the same translation unit
//   causes "conflicting declaration" errors.
//
//   This facade lets the rest of the code (LVGL bridge + app screens)
//   stay free of M5Unified entirely. Only m5_io.cpp pulls M5Unified in.

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace m5io {

void  begin();
void  update();
void  setRotation(int r);
void  setBrightness(int level);
int   width();
int   height();
bool  touchEnabled();

// Wipe the panel to a solid 8-bit RGB color. Call before LVGL takes over
// so any residual content (factory firmware or previous build) doesn't
// peek through if LVGL's first refresh misses an area.
void  fillScreen(uint8_t r, uint8_t g, uint8_t b);

// Push a rectangular RGB565 buffer to the panel. (Used by LVGL flush.)
void  blit(int x, int y, int w, int h, const uint16_t* pixels);

// Returns true if a touch is currently active and writes its coordinates
// into *x,*y. (Used by LVGL pointer indev.)
bool  readTouch(int* x, int* y);

void  beep(int freq_hz, int duration_ms);

int   batteryPct();
float batteryVolts();

// --- Wi-Fi -----------------------------------------------------------------
// Hidden behind the facade so <WiFi.h> stays out of LVGL translation units.
// Internally calls WiFi.setPins(12,13,11,10,9,8,15) — the Tab5 wires its
// ESP32-C6 SDIO2 bus on different GPIOs than the arduino-esp32 default
// (which targets the ESP32-P4 EvalBoard). Without that override WiFi.begin
// fails with "H_SDIO_DRV: card init failed".
void  wifiBegin(const char* ssid, const char* password);
bool  wifiConnected();
int   wifiRssi();

// --- HTTP ------------------------------------------------------------------
// Blocking GET. Writes UTF-8 body into out_buf (NUL-terminated, truncated to
// out_cap-1). bearer may be nullptr/"" to skip the Authorization header.
// Returns true on HTTP 200 with a body that fits.
bool  httpGetJson(const char* url, const char* bearer,
                  char* out_buf, size_t out_cap);

// Blocking GET for raw bytes (PNG, JPEG, etc.). Writes the body into
// out_buf (no NUL terminator) and writes the byte count into *out_len.
// Returns true on HTTP 200 with a body that fits in out_cap.
bool  httpGetBinary(const char* url, const char* bearer,
                    uint8_t* out_buf, size_t out_cap, size_t* out_len);

}  // namespace m5io
