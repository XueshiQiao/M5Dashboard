// lvgl_bridge.h — wires LVGL into M5Unified's display + touch.
//
// Call lvgl_bridge_init() once from setup(), AFTER M5.begin() and AFTER
// M5.Display.setRotation() (the rotation must already be applied so the
// display reports the dimensions LVGL will allocate buffers for).
//
// Then in loop() call lv_timer_handler() once per iteration.

#pragma once

void lvgl_bridge_init();
