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
#include "ui/registry.h"
#include "ui/layouts/grid/grid_layout.h"
#include "ui/layouts/news/news_layout.h"
#include "ui/layouts/terminal/terminal_layout.h"
#include "ui/layouts/todos/todos_layout.h"
#include "ui/layouts/vibehub/vibehub_layout.h"

namespace {
constexpr const char* kFallbackLayout = "grid";

void gesture_cb(lv_event_t* e) {
  (void)e;
  lv_indev_t* indev = lv_indev_active();
  if (!indev) return;
  const lv_dir_t dir = lv_indev_get_gesture_dir(indev);
  if (dir == LV_DIR_LEFT) {
    ui::activateNextLayout();
    Serial.printf("[ui] swipe → next layout: %s\n",
                  ui::activeLayout() ? ui::activeLayout()->name : "<none>");
  } else if (dir == LV_DIR_RIGHT) {
    ui::activatePreviousLayout();
    Serial.printf("[ui] swipe → previous layout: %s\n",
                  ui::activeLayout() ? ui::activeLayout()->name : "<none>");
  }
}
}  // namespace

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

  ui::registerLayout(&ui::kGridLayout);
  ui::registerLayout(&ui::kNewsLayout);
  ui::registerLayout(&ui::kTodosLayout);
  ui::registerLayout(&ui::kTerminalLayout);
  ui::registerLayout(&ui::kVibehubLayout);

  if (!ui::activateLayout(cfg::UI_LAYOUT)) {
    Serial.printf("[ui] unknown layout '%s', falling back to '%s'\n",
                  cfg::UI_LAYOUT, kFallbackLayout);
    ui::activateLayout(kFallbackLayout);
  }
  Serial.printf("[ui] active layout: %s\n",
                ui::activeLayout() ? ui::activeLayout()->name : "<none>");

  // Persistent screen-level gesture handler — LVGL preserves event
  // callbacks across lv_obj_clean(), so this stays attached even when a
  // layout's build() wipes the screen children.
  lv_obj_add_event_cb(lv_screen_active(), gesture_cb, LV_EVENT_GESTURE, NULL);

  poller_start();

  m5io::beep(880, 60);
  Serial.println("[ui] poller task started");
}

void loop() {
  m5io::update();
  poller_drain();
  uint32_t next = lv_timer_handler();
  if (next > 50) next = 50;
  delay(next ? next : 5);
}
