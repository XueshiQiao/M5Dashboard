# hello-tab5 — minimal toolchain + device verification

Smallest possible firmware that exercises the full Tab5 stack: build → flash → display → touch → audio → battery telemetry → serial.

If this works on your unit, the toolchain and hardware are healthy and we can start scaffolding the real M5Dashboard app.

## What it does

- Boots, calls `M5.begin()` (initializes ST7123 display+touch, ES8388 audio, BMI270 IMU, INA226 power monitor)
- Prints chip/display/battery info to USB serial
- Draws "Hello, M5Dashboard!" centered on screen
- Plays a 1 kHz chirp through the speaker
- Reads touch events and draws cyan dots where you tap
- Once per second, logs uptime + battery % + voltage + current to serial

## Build & flash

### Prerequisites
- PlatformIO Core ≥ 6.x. If not installed:
  ```bash
  python3 -m venv ~/.pio-venv
  ~/.pio-venv/bin/pip install -U platformio
  # then either alias or symlink: ln -s ~/.pio-venv/bin/pio /opt/homebrew/bin/pio
  ```
- A connected Tab5 (USB-C). Confirm with `ls /dev/cu.usbmodem*`.

### First build (downloads ~400–500 MB ESP32-P4 toolchain)
```bash
cd test-firmware/hello-tab5
pio run                    # build only
```

### Flash

1. Hold **Reset** on the Tab5 for ~2 s until the green LED rapid-blinks → release. (Some pioarduino versions can flash without manual download mode via the native USB-JTAG, but manual mode is the safe default.)
2. Run:
   ```bash
   pio run --target upload
   ```
3. After "Hard resetting…" the device reboots into the new firmware automatically.

### Watch serial output
```bash
pio device monitor
```
Expected log:
```
=== M5Dashboard hello-tab5 ===
Display: 1280x720
Has touch: yes
Battery: 87%  voltage: 8.12V
uptime=1s  batt=87%  V=8.12  I=-120mA
...
```

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `Could not open /dev/cu.usbmodem*` | Device not in download mode | Hold Reset 2s until green LED rapid-blinks |
| Build fails on M5Unified | Older library lacks ST7123 | `pio pkg update` (we pin git HEAD already) |
| Black screen, build/flash succeeded | ST7123 lib mismatch | Update M5Unified/M5GFX again; see `docs/M5Stack-Tab5-Reference.md` §2 |
| `no memory for frame buffer` | PSRAM not enabled | Already set in `platformio.ini`; if you forked, re-check `BOARD_HAS_PSRAM` |
| Build target rejected (board not found) | pioarduino branch moved | Pin a specific commit instead of `#develop` |

## Why not ESP-IDF for the test firmware?

ESP-IDF (~2 GB install) is the recommended path for the **production** dashboard — see `docs/M5Stack-Tab5-Reference.md` §4. But for "does my toolchain talk to my device" verification, PlatformIO + Arduino + M5Unified is significantly lighter and good enough. We'll switch to ESP-IDF when we start scaffolding the real app.
