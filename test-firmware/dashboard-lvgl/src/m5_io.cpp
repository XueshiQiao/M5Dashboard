// m5_io.cpp — only place that includes M5Unified. Keeps M5GFX's mini-LVGL
// types from colliding with full LVGL elsewhere in the build.

#include "m5_io.h"

#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>

namespace m5io {

namespace {
// Tab5's SDIO2 link to the ESP32-C6 uses non-default GPIO pins. arduino-esp32
// defaults target the ESP32-P4 EvalBoard, which is wired differently — without
// this override `WiFi.begin()` fails with H_SDIO_DRV: card init failed.
// Pin map: docs.m5stack.com/en/arduino/m5tab5/wifi
constexpr int8_t TAB5_SDIO2_CLK = 12;
constexpr int8_t TAB5_SDIO2_CMD = 13;
constexpr int8_t TAB5_SDIO2_D0  = 11;
constexpr int8_t TAB5_SDIO2_D1  = 10;
constexpr int8_t TAB5_SDIO2_D2  =  9;
constexpr int8_t TAB5_SDIO2_D3  =  8;
constexpr int8_t TAB5_SDIO2_RST = 15;
}  // namespace

void begin() {
  auto cfg = M5.config();
  M5.begin(cfg);
  // Tell lgfx that subsequent pushImage(uint16_t*) calls should byte-swap
  // before pushing to the panel. The Tab5's ST7123 (MIPI-DSI) wants
  // big-endian RGB565; ESP32 is little-endian; LVGL produces little-endian
  // (LV_COLOR_16_SWAP=0). lgfx handles the conversion when this is true.
  M5.Display.setSwapBytes(true);
}

void update()                  { M5.update(); }
void setRotation(int r)        { M5.Display.setRotation(r); }
void setBrightness(int level)  { M5.Display.setBrightness(level); }
int  width()                   { return M5.Display.width(); }
int  height()                  { return M5.Display.height(); }
bool touchEnabled()            { return M5.Touch.isEnabled(); }

void fillScreen(uint8_t r, uint8_t g, uint8_t b) {
  M5.Display.fillScreen(M5.Display.color565(r, g, b));
}

void blit(int x, int y, int w, int h, const uint16_t* pixels) {
  // pushImage is a single atomic call that handles the addr-window setup,
  // the byte-swap (controlled by setSwapBytes()), and the panel's "memory
  // write" command sequence as one transaction. Splitting that into
  // setAddrWindow + writePixels was producing per-flush misalignment on
  // ST7123 — small partial flushes (e.g. on a button tap) showed it most.
  M5.Display.pushImage(x, y, w, h, pixels);
}

bool readTouch(int* x, int* y) {
  if (M5.Touch.getCount() == 0) return false;
  auto t = M5.Touch.getDetail(0);
  *x = t.x;
  *y = t.y;
  return true;
}

void beep(int freq_hz, int duration_ms) {
  M5.Speaker.tone(freq_hz, duration_ms);
}

int   batteryPct()    { return M5.Power.getBatteryLevel(); }
float batteryVolts()  { return M5.Power.getBatteryVoltage() / 1000.0f; }

void wifiBegin(const char* ssid, const char* password) {
  WiFi.setPins(TAB5_SDIO2_CLK, TAB5_SDIO2_CMD,
               TAB5_SDIO2_D0, TAB5_SDIO2_D1, TAB5_SDIO2_D2, TAB5_SDIO2_D3,
               TAB5_SDIO2_RST);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

bool wifiConnected() { return WiFi.status() == WL_CONNECTED; }
int  wifiRssi()      { return wifiConnected() ? (int)WiFi.RSSI() : 0; }

bool httpGetJson(const char* url, const char* bearer,
                 char* out_buf, size_t out_cap) {
  if (!out_buf || out_cap == 0) return false;
  out_buf[0] = '\0';
  if (!wifiConnected()) return false;

  HTTPClient http;
  if (!http.begin(url)) return false;
  if (bearer && bearer[0]) {
    String hdr = String("Bearer ") + bearer;
    http.addHeader("Authorization", hdr);
  }
  http.addHeader("Accept", "application/json");
  http.setTimeout(8000);

  int code = http.GET();
  if (code != 200) {
    log_w("[http] GET %s -> %d", url, code);
    http.end();
    return false;
  }

  String body = http.getString();
  http.end();
  if (body.length() + 1 > out_cap) {
    log_w("[http] body %u > buf %u for %s",
          (unsigned)body.length(), (unsigned)out_cap, url);
    return false;
  }
  memcpy(out_buf, body.c_str(), body.length());
  out_buf[body.length()] = '\0';
  return true;
}

bool httpGetBinary(const char* url, const char* bearer,
                   uint8_t* out_buf, size_t out_cap, size_t* out_len) {
  if (out_len) *out_len = 0;
  if (!out_buf || out_cap == 0) return false;
  if (!wifiConnected()) return false;

  HTTPClient http;
  if (!http.begin(url)) return false;
  if (bearer && bearer[0]) {
    String hdr = String("Bearer ") + bearer;
    http.addHeader("Authorization", hdr);
  }
  http.setTimeout(10000);

  int code = http.GET();
  if (code != 200) {
    log_w("[http] GET %s -> %d", url, code);
    http.end();
    return false;
  }

  // Stream into the caller's buffer instead of going through HTTPClient's
  // internal String — the body is binary, and getString() would copy
  // through a String first (extra allocation, NUL-truncation surprises).
  WiFiClient* stream = http.getStreamPtr();
  int contentLen = http.getSize();   // -1 if chunked
  size_t total = 0;

  while (http.connected()) {
    size_t avail = stream->available();
    if (avail == 0) {
      if (contentLen > 0 && total >= (size_t)contentLen) break;
      if (contentLen < 0 && total > 0) {
        // Chunked: bail when the connection idles after we've drained.
        delay(5);
        if (stream->available() == 0) break;
        continue;
      }
      delay(5);
      continue;
    }
    size_t want = avail;
    if (total + want > out_cap) {
      log_w("[http] binary body > buf (%u + %u > %u) for %s",
            (unsigned)total, (unsigned)want, (unsigned)out_cap, url);
      http.end();
      return false;
    }
    int got = stream->readBytes(out_buf + total, want);
    if (got <= 0) break;
    total += (size_t)got;
    if (contentLen > 0 && total >= (size_t)contentLen) break;
  }
  http.end();
  if (out_len) *out_len = total;
  return total > 0;
}

}  // namespace m5io
