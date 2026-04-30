# M5Stack Tab5 — Developer Reference

> Living reference for the M5Stack Tab5 (ESP32-P4) device. Built for the
> **M5Dashboard** project but intended to be reused / forked into any future
> Tab5 project. Update this file whenever we learn something non-obvious.
>
> Last verified: 2026-04-29

---

## 1. Hardware at a glance

The Tab5 is a **two-chip system** — main MCU + radio coprocessor — not a typical single-SoC ESP32 board.

| Component | Part / spec | Notes |
|---|---|---|
| Main MCU | **ESP32-P4** (RISC-V dual-core 360 MHz HP + 40 MHz LP) | RISC-V, **no built-in radio** |
| Radio | **ESP32-C6-MINI-1U** | Wi-Fi 6, BLE 5, Thread, Zigbee. Hosted-network bridge over internal bus to P4. |
| Flash | 16 MB | |
| PSRAM | **32 MB Octal** | Plenty for 720p framebuffers + LVGL double-buffer |
| Display | 5" IPS, 1280×720, MIPI-DSI | **ST7123** integrated display+touch driver (since Oct 2025) |
| Camera | SC2356 2 MP, MIPI-CSI | 1600×1200, edge-AI capable |
| Audio | ES8388 codec + ES7210 AEC, dual mic, 1 W speaker, 3.5 mm jack | |
| IMU | BMI270 (6-axis accel + gyro, wake-on-interrupt) | |
| RTC | RX8130CE | Survives power-off if battery present |
| Power | INA226 monitor, IP2326 charger (must be ON to charge), MP4560 buck-boost | |
| Battery | NP-F550 7.4 V 2000 mAh, ~6 h @ 50 % brightness | Removable Sony-compatible camcorder cell |
| USB | USB-A host + USB-C OTG (USB 2.0) | |
| Other I/O | microSD, RS485 (SIT3088), M5-Bus (rear), HY2.0-4P, GPIO header, stamp pads | |
| Wireless options | Built-in 3D antenna + 2× MMCX external (switchable) | |
| Mount | 1/4″-20 tripod nut on side | |
| Dimensions | 128 × 80 × 12 mm (118 g) — Kit w/ battery: 128 × 80 × 26.7 mm (217 g) | |
| Operating temp | 0–40 °C | |

**Schematic / block diagram:**
- [Block diagram (PDF)](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1132/Tab5_Overall_Design_Block_Diagram.pdf)
- [Schematics (PDF)](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1132/Tab5_Schematics_PDF.pdf)

### Boot / flash mode
Hold **Reset** ~2 s until the internal **green LED blinks rapidly** → release → device is in download mode. The USB-C port enumerates as a native USB CDC serial device (no CP210x/CH340 — the P4 has built-in USB-Serial-JTAG).

---

## 2. The October 2025 ST7123 transition (critical gotcha)

In Oct 2025 M5Stack swapped the display+touch chips:

| | Before (pre-Oct 2025) | After |
|---|---|---|
| Display driver | ILI9881C | **ST7123** (integrated) |
| Touch driver | GT911 | **ST7123** (same chip) |

**Always check the sticker on the back** of the device to confirm which controller you have.

**Library implications** — old libraries don't drive ST7123:

| Stack | Min version with ST7123 |
|---|---|
| M5Unified (Arduino) | Latest from M5Stack repo (post-Oct 2025) |
| M5GFX (Arduino) | Latest from M5Stack repo (post-Oct 2025) |
| M5Tab5-UserDemo (ESP-IDF) | `main` branch HEAD |
| ESPHome `mipi_dsi` Tab5 | ≥ 2025.10.3 |
| espp BSP (`esp-cpp/espp`) | Some docs still reference GT911 — verify before trusting |

If display is black or touch is dead: first suspect a stale library, not bad hardware.

---

## 3. Development paths — comparison

| Path | Toolchain | Pros | Cons | Best for |
|---|---|---|---|---|
| **ESP-IDF 5.4.2 + LVGL + M5Tab5 BSP** | ESP-IDF, idf.py, CMake | Canonical, full hardware support, used by factory firmware, robust HTTPS, Mooncake app framework available | ~2 GB install, CMake/menuconfig learning curve | **Production dashboards.** Recommended for M5Dashboard. |
| **PlatformIO + Arduino-ESP32-P4 + M5Unified + LVGL** | pip install platformio | Faster ramp-up, Arduino-friendly, `lib_deps` for libraries | No dedicated Tab5 board profile yet — must select generic `ESP32P4 Dev Module`; some libraries lag IDF | **Quick prototypes**, hello-world tests. |
| **Arduino IDE + M5Unified + M5GFX** | Arduino IDE GUI | Easiest "first sketch" experience | M5GFX is graphics-only — no widget toolkit; you'd reinvent dashboard layouts | Sensor demos, learning. |
| **ESPHome (YAML)** | ESPHome / Home Assistant | Trivial Wi-Fi + HA integration; official `tab5-ha-hmi.yaml` exists | Locked to HA's entity model — won't fit non-HA data sources cleanly | Home-Assistant dashboards only. |
| **MicroPython + LVGL** | mpy-cross + custom firmware | Fast iteration | P4 port still maturing | Experimentation, not products. |

### Toolchain install commands (for reference)

#### ESP-IDF v5.4.2 (macOS / Apple Silicon)
```bash
# Prereqs already on most macs:
brew install cmake ninja dfu-util libusb python3
# IDF itself:
mkdir -p ~/esp && cd ~/esp
git clone -b v5.4.2 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf && ./install.sh esp32p4
. ./export.sh   # source this in every new shell, or alias it
```
Disk: ~2 GB. First-time install: ~15–30 min depending on bandwidth.

#### PlatformIO Core (lighter)
```bash
python3 -m venv ~/.pio-venv
~/.pio-venv/bin/pip install -U platformio
ln -s ~/.pio-venv/bin/pio /opt/homebrew/bin/pio   # optional
```
Disk: ~50 MB tool + ~500 MB on first ESP32-P4 build (toolchain auto-downloads).

#### Just esptool (lightest — for flashing pre-built `.bin` only)
```bash
python3 -m venv ./.venv && ./.venv/bin/pip install esptool
./.venv/bin/esptool.py --chip esp32p4 --port /dev/cu.usbmodemXXXXXX chip_id
```
Disk: ~10 MB. Useful for: device detection, factory firmware restore, OTA-less reflash.

---

## 4. Recommended stack for application development

For most Tab5 apps with a non-trivial UI, use:

```
ESP-IDF 5.4.2
  └── M5Tab5 BSP (display, touch, audio, IMU, SD, power)
       └── LVGL 9.x (widgets, layouts, themes)
            └── Mooncake (lightweight "apps within an app" framework)
                 └── Your application code
```

Why this stack:
1. **BSP** wires up ST7123, ES8388, BMI270, INA226 etc. — saves a week of HAL work.
2. **LVGL 9** has cards, charts, lists, sliders, gauges — all the dashboard primitives.
3. **Mooncake** (used by the official UserDemo) provides clean app-switching/lifecycle for multi-screen products.
4. **ESP-IDF** networking (`esp_http_client` + `esp_tls` + cert bundle) is solid for HTTPS APIs.

Starting template: fork `https://github.com/m5stack/M5Tab5-UserDemo`, strip the bundled apps, keep the platform layer.

---

## 5. Reference projects (priority order)

| # | Project | Stack | Why study it |
|---|---|---|---|
| 1 | [m5stack/M5Tab5-UserDemo](https://github.com/m5stack/M5Tab5-UserDemo) | ESP-IDF + LVGL + Mooncake | Official factory firmware. Canonical project layout. |
| 2 | [Tab5 Home Assistant HMI](https://docs.m5stack.com/en/homeassistant/kit/tab5_ha_hmi) + [tab5-lvgl.yaml](https://github.com/m5stack/esphome-yaml/blob/main/common/tab5-lvgl.yaml) | ESPHome + LVGL | Closest visual reference for sidebar+pages dashboard layout. |
| 3 | [GalusPeres/Tab5-HomeAssistant-Display](https://github.com/GalusPeres/Tab5-HomeAssistant-Display) | Arduino + LVGL + LittleFS web admin | Real "configurable tile dashboard". OTA, captive-portal Wi-Fi, drag-drop tiles. |
| 4 | [nikthefix/M5Stack_Tab5_Arduino_Basic_LVGL_Demo](https://github.com/nikthefix/M5Stack_Tab5_Arduino_Basic_LVGL_Demo) | Arduino + LVGL + SquareLine | Minimal LVGL hello-world. SquareLine Studio project included. |
| 5 | [esp-cpp/espp M5StackTab5 BSP](https://esp-cpp.github.io/espp/m5stack_tab5.html) | ESP-IDF, modern C++ | Cleaner C++ HAL. Note: some docs still reference GT911 (pre-ST7123). |
| 6 | [CNX-Software Review pt 2](https://www.cnx-software.com/2025/05/18/m5stack-tab5-review-getting-started-esp32-p4-esp-idf-framework-arduino-ide/) | — | Hands-on getting-started with exact commands and known workarounds. |

---

## 6. Common pitfalls & workarounds

| Symptom | Cause | Fix |
|---|---|---|
| Black screen / no touch on a freshly-flashed app | Library predates ST7123 | Update M5Unified/M5GFX, or use UserDemo HEAD. |
| `no memory for frame buffer` during build/run | PSRAM not enabled, or wrong partition scheme | Enable Octal PSRAM, partition `3MB app / 9.9MB FATFS`. |
| `'adc_power_acquire' was not declared` on older demos | Legacy ADC driver removed in IDF 5.x | Skip the demo, or migrate to new ADC API. |
| Device not in `/dev/cu.usbmodem*` after Reset | Not in download mode | Hold Reset 2 s until green LED **rapid-blinks**, then release. |
| Device runs on battery but Mac shows no `/dev/cu.usbmodem*` when plugged in | USB-C OTG role / cable issue, or device was battery-powered before the host negotiated the data role | Use a known data-capable cable, preferably USB-A-to-C or a USB-C hub that forces the Mac as host. Connect USB first, then hold Reset ~2 s until the green LED rapid-blinks. If still missing, remove/reseat battery or power-cycle, then repeat. |
| `M5GFX AnalogMeter` won't compile | Type-mismatch in `std::min()` | Cast: `std::min(6, (int)(display.width()) / 40)` |
| Battery won't charge | IP2326 quirk — device must be powered on to charge | Power on first, then plug USB-C. |
| `H_SDIO_DRV: card init failed` + `STA enable failed` | arduino-esp32's default SDIO pins are EvalBoard pins, not Tab5 pins | Add `WiFi.setPins(12,13,11,10,9,8,15)` before `WiFi.begin`. See §8. |

---

## 7. Pinout / peripheral notes

(Fill in as we measure / confirm. Schematic PDF is the authoritative source.)

| Subsystem | Bus / pins | Driver IC | Notes |
|---|---|---|---|
| Display | MIPI-DSI 4-lane | ST7123 | RGB888, 1280×720 @ 60 Hz |
| Touch | Shared with ST7123 (I²C side-channel) | ST7123 | Multi-touch |
| Camera | MIPI-CSI 2-lane | SC2356 | 2 MP |
| Audio out | I²S → ES8388 | ES8388 | Speaker + headphone |
| Audio in | I²S → ES7210 | ES7210 | Dual mic + AEC |
| IMU | I²C | BMI270 | Wake-on-motion supported |
| RTC | I²C | RX8130CE | Battery-backed |
| Power monitor | I²C | INA226 | Voltage / current / power |
| microSD | SDMMC | — | |
| RS485 | UART → SIT3088 | — | 6–24 V supply, switchable 120 Ω terminator |
| C6 link | SDIO / UART (TBD) | ESP32-C6 | Hosted Wi-Fi; needs `esp_hosted` or `esp_wifi_remote` |

---

## 8. Networking notes (ESP32-P4 specifics)

P4 has **no built-in radio** — Wi-Fi/BLE goes through the C6 over an internal hosted-network link (ESP-Hosted protocol over SDIO). In ESP-IDF this is exposed via `esp_wifi_remote` / `esp_hosted`. From application code you still use the standard `esp_wifi_*` and `esp_http_client` APIs — the redirection is handled by the BSP and component config.

Implication: when starting a new IDF project, ensure your `idf_component.yml` includes the M5 Tab5 BSP **or** the `esp_wifi_remote` + `esp_hosted` components, otherwise Wi-Fi won't link.

### **Critical for Arduino / pioarduino**: Tab5 needs explicit SDIO pin override

arduino-esp32's default SDIO pins target the ESP32-P4 *EvalBoard*, which is wired differently than the Tab5. Without an override, `WiFi.begin()` fails with `H_SDIO_DRV: card init failed` — host can't reach the C6 over SDIO at all.

Tab5 pin map (verified on hardware, 2026-04-29; sourced from [docs.m5stack.com/en/arduino/m5tab5/wifi](https://docs.m5stack.com/en/arduino/m5tab5/wifi)):

| Signal | GPIO |
|---|---|
| SDIO2 CLK | 12 |
| SDIO2 CMD | 13 |
| SDIO2 D0  | 11 |
| SDIO2 D1  | 10 |
| SDIO2 D2  |  9 |
| SDIO2 D3  |  8 |
| SDIO2 RST | 15 |

Required call (before any WiFi.mode / WiFi.begin):
```cpp
WiFi.setPins(12, 13, 11, 10, 9, 8, 15);
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
```

Min library versions per M5: board manager ≥ 3.2.2, M5Unified ≥ 0.2.8, M5GFX ≥ 0.2.11.

The C6 ships with ESP-Hosted slave firmware **1.4.1**; arduino-esp32 master ships host **2.12.3**. The host logs "Version on Host is NEWER than version on co-processor" but the link works — backward-compatible enough for STA to associate cleanly.

---

## 9. Power-management notes

- Charge IC IP2326 requires the system to be powered on to charge — schedule a "deep-sleep but-still-charging" mode if running unattended.
- INA226 lets you read battery voltage / current / instantaneous power for a battery widget.
- Battery presence should be inferred from INA226 voltage, not blindly from percentage. On Tab5, M5Unified maps the INA226 bus voltage to a 2S Li-ion percentage. With no NP-F550 pack attached, USB/VBUS can appear around 4.3 V and read as a bogus 0 % battery; a real 2S pack is roughly 6.0-8.4 V.
- In M5Dashboard's LVGL UI, battery/Wi-Fi status refreshes every 2 s via an LVGL timer. The power indicator treats battery presence and USB/external cable presence as separate factors:
  - battery + cable: charging icon + battery percentage
  - battery + no cable: normal battery icon + battery percentage
  - no battery + cable: USB icon, no fake percentage
  - no battery + no cable: no power indicator
- The four-state Tab5 power logic is packaged as a reusable single-file helper at `firmware/include/tab5_power_state.h`. Define `TAB5_POWER_STATE_IMPLEMENTATION` in exactly one `.cpp` file before including it.
- If the Tab5 is already running from battery before the USB-C cable is attached, USB-C OTG role negotiation can fail or choose the wrong role, so macOS may not create `/dev/cu.usbmodem*`. For flashing, connect the data cable first and then enter download mode; USB-A-to-C is the most deterministic because the Mac side is forced to host.
- BMI270 + RX8130CE both support wake-on-interrupt → can build a "tap-to-wake" or "scheduled-wake" power profile.
- Approximate runtimes from M5Stack: 6 h @ 50 % brightness + Wi-Fi on. With aggressive dimming + sleep-between-polls, multi-day standby is feasible.

---

## 10. Useful upstream documentation

- [Tab5 product docs](https://docs.m5stack.com/en/core/Tab5) — start here
- [Arduino quickstart](https://docs.m5stack.com/en/arduino/m5tab5/program)
- [ESP-IDF UserDemo guide](https://docs.m5stack.com/en/esp_idf/m5tab5/userdemo)
- [Home Assistant HMI case](https://docs.m5stack.com/en/homeassistant/kit/tab5_ha_hmi)
- [Factory restore guide](https://docs.m5stack.com/en/guide/restore_factory/m5tab5)
- [ESP32-P4 Espressif datasheet](https://www.espressif.com/en/products/socs/esp32-p4)
- [ESP32-C6 Espressif datasheet](https://www.espressif.com/en/products/socs/esp32-c6)
- [LVGL docs](https://docs.lvgl.io/)

---

## 11. Verified device readings

Recorded from the M5Dashboard project's first hardware test on 2026-04-29.

### From esptool

```
Chip type:          ESP32-P4 (revision v1.3)
Features:           Dual Core + LP Core, 400MHz
Crystal frequency:  40MHz
USB mode:           USB-Serial/JTAG
Flash size:         16MB
Flash mfr/dev:      0x46 / 0x4018
Port (macOS):       /dev/cu.usbmodem<N>   ; native CDC, no driver needed
```

Note: ESP32-P4 has **no chip-id register** — esptool will say *"ESP32-P4 has no chip ID. Reading MAC address instead."* That's normal; the MAC is the unique identifier.

### From the running firmware (M5Unified Arduino + ST7123)

```
ESP-ROM: esp32p4-eco2-20240710           ; ECO2 silicon revision
rst:0x17 CHIP_USB_UART_RESET             ; reset triggered by host DTR/RTS
PSRAM added to heap                      ; Octal PSRAM enabled
M5GFX [Autodetect] board:22 → board_M5Tab5
i2cInit: num=1 sda=31 scl=32 freq=100000 ; Tab5 system I²C bus
LEDC attached to pin 22 (backlight PWM, channel 0, 9-bit resolution)
M5.Display.width()  = 720
M5.Display.height() = 1280               ; default portrait — call setRotation()
M5.Touch.isEnabled() = true
M5.Power: battery 100 % @ 8.38 V (NP-F550 fully charged)
```

### Resetting the P4 over USB-CDC without bootloader mode

Pulse RTS while DTR is high to trigger `CHIP_USB_UART_RESET`. Useful for capturing fresh boot logs without re-flashing or using the physical Reset button:

```python
import serial, time
ser = serial.Serial('/dev/cu.usbmodem111301', 115200, timeout=1)
ser.dtr = False; ser.rts = True; time.sleep(0.1)
ser.dtr = False; ser.rts = False
```

### `pio device monitor` quirk

`pio device monitor` requires an interactive TTY (calls `termios.tcgetattr`); it crashes from a non-TTY shell with `Operation not supported by device`. Use a small pyserial reader script (above) for scripted/automated serial capture.

## 12. Update log

| Date | Change |
|---|---|
| 2026-04-29 | Initial reference compiled from M5 docs + community sources |
| 2026-04-29 | Verified ESP32-P4 v1.3 + 16 MB flash on real Tab5 hardware via esptool |
