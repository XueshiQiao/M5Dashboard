// firmware — entry point.
//
// IMPORTANT: this file does NOT include <M5Unified.h>. M5GFX bundles its
// own mini-LVGL types whose guards differ from LVGL's, so mixing the two
// in one translation unit collides. All hardware access goes through the
// m5_io facade (src/m5_io.{h,cpp}).

#include <Arduino.h>
#include <lvgl.h>

#include "config.h"
#include "data_poller.h"
#include "lvgl_bridge.h"
#include "m5_io.h"
#include "screen_grid.h"

void setup() {
  m5io::begin();

  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=== M5Dashboard LVGL ===");

  m5io::setRotation(1);
  m5io::setBrightness(cfg::BRIGHTNESS_ACTIVE);
  m5io::fillScreen(0x0A, 0x0E, 0x12);   // dark base layer before LVGL takes over

  Serial.printf("Display: %dx%d  Touch: %s\n",
                m5io::width(), m5io::height(),
                m5io::touchEnabled() ? "yes" : "no");

  lvgl_bridge_init();
  ui::buildGridScreen();

  poller_start();

  m5io::beep(880, 60);
  Serial.println("[ui] grid screen built; poller task started");
}

void loop() {
  m5io::update();
  poller_drain();
  uint32_t next = lv_timer_handler();
  if (next > 50) next = 50;
  delay(next ? next : 5);
}
