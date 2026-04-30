// lvgl_bridge.cpp — bridges LVGL 9.x to the M5 hardware via m5_io.
//
// This file deliberately does NOT include <M5Unified.h>. M5GFX bundles its
// own mini-LVGL headers (lgfx/v1/lv_font/area.h etc.) whose guards differ
// from LVGL's, so combining the two stacks in one translation unit collides.
// We talk to the hardware through m5_io instead.

#include "lvgl_bridge.h"

#include <Arduino.h>      // millis()
#include <esp_heap_caps.h>
#include <lvgl.h>

#include "m5_io.h"

namespace {

constexpr int kBufLines = 60;     // partial-render strip height

uint8_t* g_buf1 = nullptr;
uint8_t* g_buf2 = nullptr;

void flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  const uint32_t w = lv_area_get_width(area);
  const uint32_t h = lv_area_get_height(area);
  m5io::blit(area->x1, area->y1, w, h,
             reinterpret_cast<const uint16_t*>(px_map));
  lv_display_flush_ready(disp);
}

void touch_cb(lv_indev_t* /*indev*/, lv_indev_data_t* data) {
  int x, y;
  if (m5io::readTouch(&x, &y)) {
    data->state   = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

uint32_t tick_cb() { return millis(); }

}  // namespace

void lvgl_bridge_init() {
  lv_init();
  lv_tick_set_cb(tick_cb);

  const int    w        = m5io::width();
  const int    h        = m5io::height();
  const size_t buf_bytes = static_cast<size_t>(w) * kBufLines * sizeof(uint16_t);

  // Allocate from PSRAM. Internal RAM is tight on ESP32-P4 once
  // arduino-esp32 + LVGL is in. PSRAM is fine for these buffers — M5GFX
  // copies through the MIPI-DSI driver, no direct-DMA constraint.
  g_buf1 = static_cast<uint8_t*>(
      heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  g_buf2 = static_cast<uint8_t*>(
      heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  if (!g_buf1 || !g_buf2) {
    Serial.println("[lvgl] FATAL: could not allocate render buffers");
    return;
  }
  Serial.printf("[lvgl] buffers: %u bytes each (%dx%d)\n",
                static_cast<unsigned>(buf_bytes), w, kBufLines);

  lv_display_t* disp = lv_display_create(w, h);
  lv_display_set_buffers(disp, g_buf1, g_buf2, buf_bytes,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(disp, flush_cb);

  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touch_cb);

  Serial.println("[lvgl] bridge up");
}
