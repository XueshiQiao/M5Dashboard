# M5Dashboard — Notes for Claude

Project-specific guidance for AI assistants working in this repo. Read [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) for the full system design; this file is the short version of "what to keep in mind while writing code here."

---

## What this project is

A two-piece system:

- **`server/`** — Node + TypeScript + Fastify. Runs on the developer's Mac. Aggregates QWeather, Anthropic's OAuth Usage API, the local Codex CLI's `app-server` JSON-RPC, and Kagi News. Bearer-authed.
- **`firmware/`** — ESP32-P4 firmware (pioarduino + arduino-esp32 + M5Unified + LVGL 9). Polls the server, renders four cards (Weather, Claude Code, Codex, Inbox).

---

## Where the active code lives

```
server/src/
  index.ts        Fastify entry + route registration
  config.ts       env parsing
  auth.ts         bearer-token preHandler
  cache.ts        TTL memo + stale-while-error
  qweather.ts     QWeather REST + EdDSA-JWT signing
  weather.ts      /api/weather shaper (calls qweather)
  weatherIcon.ts  QWeather SVG → PNG via @resvg/resvg-js
  brandIcon.ts    Claude / Codex brand SVG → PNG
  claude.ts       macOS keychain → Anthropic Usage API
  codex.ts        codex app-server JSON-RPC subprocess
  news.ts         Kagi News RSS parser

firmware/src/
  main.cpp           setup/loop entry
  m5_io.{h,cpp}      M5Unified / WiFi / HTTP facade — ONLY TU that includes <M5Unified.h>
  lvgl_bridge.{h,cpp}  LVGL ↔ M5GFX flush + touch + tick
  data.{h,cpp}       JSON value types + ArduinoJson parsers
  data_poller.{h,cpp}  FreeRTOS poll task (core 0) + drain (LVGL thread)
  screen_grid.{h,cpp}  the 4-card UI
```

---

## Hard rules (non-obvious things that will trip you up)

### Privacy

The repo is open-source. Do **not** commit:
- `ed25519-private.pem` / `ed25519-public.pem` (QWeather signing key)
- `server/.env` (real `AUTH_TOKEN`, real QWeather IDs)
- `firmware/include/config.h` (real Wi-Fi creds, server URL, AUTH_TOKEN)

All four are in `.gitignore`. Templates for them are checked in (`*.example.h`, `.env.example`).

If you find personal identifiers in committed code (real emails, the user's QWeather IDs `n97mda5jxn` / `4E2DWYEPVN` / `TFB4P8A4QP`, the Wi-Fi SSID `Joey`, etc.), **stop and flag it** before committing.

### Device-side: never include `<M5Unified.h>` outside `m5_io.cpp`

M5GFX bundles its own miniature LVGL types in `lgfx/v1/lv_font/{area,color,draw_buf}.h`. They use different include guards than full LVGL, so combining `<M5Unified.h>` and `<lvgl.h>` in the same translation unit produces "conflicting declaration" errors. The `m5_io` facade is the only place these two stacks meet — its public surface uses POD types only.

### Wi-Fi requires `WiFi.setPins(12,13,11,10,9,8,15)` before `WiFi.begin`

arduino-esp32's default SDIO GPIOs target the ESP32-P4 EvalBoard; the Tab5 wires the C6 differently. Without this override, `H_SDIO_DRV: card init failed` cascade and `WiFi.begin` fails. `m5io::wifiBegin()` hides this; don't bypass it.

### Render byte-swap

`LV_COLOR_16_SWAP=0` (LVGL emits little-endian RGB565). `m5_io::begin()` calls `M5.Display.setSwapBytes(true)` so lgfx swaps for the panel. Setting both, or neither, produces vertical-streak artifacts.

### LVGL widget calls live on the LVGL thread

The poller task runs on FreeRTOS core 0. It stages snapshots under a mutex; `poller_drain()` runs from the LVGL thread (loop task) and is the only path that calls `lv_*` APIs. Don't call LVGL widget functions from the poller task.

### Codex CLI is alpha (`0.125.0-alpha.3` at integration time)

Field names *can* break across releases. If `/api/codex` parse fails after a `codex` upgrade, run `codex app-server generate-json-schema` and diff against `server/src/codex.ts`'s typed interfaces.

### Server in a Linux container won't work

`/api/claude` reads the macOS keychain via the `security` CLI. `/api/codex` spawns the `codex` macOS-native binary and reads `~/.codex/auth.json`. Both are fundamentally tied to the host Mac. Use launchd if you want auto-start/restart.

---

## Workflow expectations

These are durable preferences from prior sessions:

- **Don't guess.** Verify state by reading files, hitting endpoints, or capturing serial output. If you can't verify, say so explicitly. Pattern-matching from training data has bitten this project multiple times.
- **Verify on the device after each change** that affects the UI or data path. The user can't see results otherwise. Reset the Tab5 (DTR/RTS pulse) and capture serial via a pyserial reader; `pio device monitor` doesn't work from non-TTY shells.
- **No backwards-compat hacks.** Delete unused code completely. Don't leave `// removed` comments or aliases for renamed things.
- **No comments explaining what the code does.** Identifiers do that. Comments are for *why* — non-obvious constraints, hidden invariants, workarounds with a reference.

---

## Common commands

### Server

```bash
cd server
npm install
cp .env.example .env       # then fill in real values (gitignored)
npm run dev                # tsx watch mode
npm run typecheck

# probe endpoints
TOK=$(grep AUTH_TOKEN .env | cut -d= -f2)
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/weather | jq
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/claude  | jq
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/codex   | jq
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/news    | jq
```

### Firmware

```bash
cd firmware
~/.platformio/penv/bin/pio run                                         # build
~/.platformio/penv/bin/pio run --target upload --upload-port /dev/cu.usbmodem<NN>
```

### Read serial without jamming the device into download mode

```bash
~/.platformio/penv/bin/python -c "
import serial, time, sys
s = serial.Serial('/dev/cu.usbmodem<NN>', 115200, timeout=0.5)
end = time.time() + 20
while time.time() < end:
    d = s.read(4096)
    if d: sys.stdout.write(d.decode('utf-8','replace')); sys.stdout.flush()
"
```

To trigger a fresh boot for capturing the banner, pulse DTR/RTS:

```python
s.setDTR(False); s.setRTS(True);  time.sleep(0.1)
s.setDTR(True);  s.setRTS(False); time.sleep(0.1)
s.setDTR(False); s.setRTS(False)
```

### Polling cadences (in `data_poller.cpp`)

| Endpoint              | Device cadence | Server memo |
| --------------------- | -------------- | ----------- |
| `/api/weather`        | 15 min         | 15 min      |
| `/api/claude`         | 60 s           | 60 s        |
| `/api/codex`          | 60 s           | 60 s        |
| `/api/news`           | 30 min         | 30 min      |
| `/api/weather/icon/:code` | on change   | 24 h        |
| `/api/icon/:name`     | once / session | 24 h        |

---

## Known TODOs / open ground

- Inbox card uses Kagi's AI feed; long titles can clip on the BR card (no `lv_label_set_long_mode` set yet).
- Per-day forecast icons in the strip still render as text glyphs; `data::ForecastDay` already carries `iconCode`, so it's a screen_grid-only swap.
- Custom font for the big temperature (>48 pt would need `lv_font_conv` to generate a C font from a TTF).
- Multi-page support (`lv_tileview`) was discussed and evaluated but not implemented — see the conversation log if revisiting.
