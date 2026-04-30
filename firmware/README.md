# firmware

ESP32-P4 firmware for the M5Stack Tab5. pioarduino + M5Unified for hardware; **LVGL 9.2** layered on top via a small bridge.

## Why LVGL

- Real widget toolkit (cards, charts, bars, scroll views, dropdowns)
- Anti-aliased rendering
- Built-in dark theme + animations
- Image support (PNG/JPG, embedded or from filesystem)
- Used by every reference Tab5 project (M5Tab5-UserDemo, HA HMI, etc.)

## Architecture

```
+------------------+
|   Application    |  <- src/main.cpp (screens & widgets)
+------------------+
|      LVGL 9      |  <- via lib_deps (lvgl/lvgl@^9.2.0)
+------------------+
|  LVGL bridge     |  <- src/lvgl_bridge.{h,cpp}
|   - flush_cb     |     LVGL → M5.Display.writePixels
|   - touch_cb     |     M5.Touch → LVGL pointer events
|   - tick_cb      |     millis() → LVGL tick
+------------------+
| M5Unified/M5GFX  |  <- hardware (display, touch, audio, IMU, power)
+------------------+
|   ESP32-P4 P4    |
+------------------+
```

## Build & flash

```bash
cd firmware
~/Code/M5Dashboard/.venv/bin/pio run
~/Code/M5Dashboard/.venv/bin/pio run -t upload --upload-port /dev/cu.usbmodem111301
```

First build is slow (LVGL is large). Subsequent builds are cached.
