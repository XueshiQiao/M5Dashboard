# M5Dashboard — Architecture & Operations Guide

Snapshot of the project as of 2026-04-30. Hardware-side reference (chip
IDs, pin maps, toolchain pitfalls) lives next door in
[`M5Stack-Tab5-Reference.md`](./M5Stack-Tab5-Reference.md); this file
covers everything we built on top of it.

---

## 1. What it is

A personal information dashboard running on the **M5Stack Tab5** (ESP32-P4
+ ESP32-C6, 5″ 1280×720 MIPI-DSI touchscreen, landscape). The device
shows four cards on a single grid screen:

| Card           | Source                                        |
| -------------- | --------------------------------------------- |
| Weather (TL)   | QWeather (HeFeng), real-time + 7-day forecast |
| Claude Code (TR) | Anthropic OAuth Usage API via macOS keychain |
| Codex (BL)     | OpenAI Codex CLI's local `app-server`         |
| Inbox (BR)     | Placeholder (no data source yet)              |

The device polls a local **dashboard server** running on the user's Mac.
That server fans out to every upstream and shapes the data into a single
JSON envelope per card. The device never holds OAuth tokens or upstream
API keys.

---

## 2. High-level architecture

```
                    ┌─────────────────────────────────────────────┐
                    │   YOUR MAC (m5dashboard-server, port 8787)  │
                    │                                             │
                    │  Fastify ── bearer auth ── memo cache       │
                    │     │                                       │
                    │     ├── /api/weather       ──> QWeather     │
                    │     │   /api/weather/icon  ──> resvg PNG    │
                    │     ├── /api/claude        ──> macOS        │
                    │     │                         keychain →    │
                    │     │                         Anthropic API │
                    │     └── /api/codex         ──> codex        │
                    │                               app-server    │
                    │                               (JSON-RPC)    │
                    └──────────────────┬──────────────────────────┘
                                       │ HTTP (LAN, bearer auth)
                                       ▼
                    ┌─────────────────────────────────────────────┐
                    │  M5Stack Tab5 (firmware/)                   │
                    │                                             │
                    │  loop:        m5io::update + lv_timer       │
                    │  poller task: HTTP fetch every 60 s/15 min, │
                    │               stage under mutex             │
                    │  drain:       LVGL widget updates           │
                    └─────────────────────────────────────────────┘
```

Polling cadence (defaults, defined in `data_poller.cpp`):

| Endpoint     | Device cadence | Server memo |
| ------------ | -------------- | ----------- |
| `/api/weather` | 15 min       | 15 min      |
| `/api/claude`  | 60 s         | 60 s        |
| `/api/codex`   | 60 s         | 60 s        |
| `/api/weather/icon/:code` | only when code changes | 24 h |

---

## 3. Repo layout

```
M5Dashboard/
├── docs/
│   ├── M5Stack-Tab5-Reference.md      hardware reference
│   └── ARCHITECTURE.md                this file
├── ed25519-private.pem                QWeather JWT signing key (gitignored)
├── ed25519-public.pem                 (uploaded to QWeather console)
├── server/                            Node + TS Fastify backend
│   ├── package.json
│   ├── .env / .env.example
│   ├── assets/qweather-icons/         vendored qwd/Icons SVGs (MIT)
│   └── src/
│       ├── index.ts                   Fastify entry, route registration
│       ├── config.ts                  env-var parsing
│       ├── auth.ts                    bearer-token preHandler
│       ├── cache.ts                   TTL memo + stale-while-error
│       ├── claude.ts                  Anthropic Usage API
│       ├── codex.ts                   codex app-server JSON-RPC
│       ├── qweather.ts                QWeather REST + JWT signing
│       ├── weather.ts                 /api/weather shaper (calls qweather)
│       └── weatherIcon.ts             SVG → PNG via @resvg/resvg-js
└── firmware/                         ESP32-P4 firmware (LVGL 9.2)
    ├── platformio.ini
    ├── include/
    │   ├── config.h                   Wi-Fi creds, server URL, AUTH_TOKEN (gitignored)
    │   ├── config.example.h
    │   └── lv_conf.h
    ├── scripts/
    │   └── skip_lvgl_arm_asm.py       pre-build: stub Helium ARM .S, dedupe lv_font
    └── src/
        ├── main.cpp                   setup/loop entry — registers + activates UI layout
        ├── hw/
        │   ├── m5_io.{h,cpp}          M5Unified / WiFi facade
        │   ├── lvgl_bridge.{h,cpp}    LVGL ↔ M5GFX flush/touch/tick
        │   ├── data.{h,cpp}           value types + JSON parsers
        │   └── data_poller.{h,cpp}    FreeRTOS task + drain (uses ui::deliver*)
        └── ui/
            ├── layout.h               Layout function-pointer interface
            ├── registry.{h,cpp}       register/activate/deliver helpers
            └── layouts/
                ├── grid/                  cockpit grid layout (kGridLayout)
                ├── terminal/              fastfetch-style (kTerminalLayout)
                └── vibehub/               rainbow header (kVibehubLayout)
```

**Gitignore highlights:**
- `**/include/config.h` (per-project secrets)
- `ed25519-*.pem` (QWeather JWT keypair at repo root)
- `**/.pio/`, `node_modules/`, `.env`

---

## 4. Server (Node + TS + Fastify)

### 4.1. Build & run

```bash
cd server
npm install
cp .env.example .env       # then fill in real values
npm run dev                # tsx watch mode
npm run typecheck          # tsc --noEmit
```

Listens on `0.0.0.0:8787`. Find the Mac's LAN IP with
`ipconfig getifaddr en0` and put it in the device's `config.h` as
`SERVER_URL`.

### 4.2. Auth

`server/src/auth.ts` adds a `preHandler` to the root Fastify instance.
Every route except `/api/health` requires
`Authorization: Bearer <AUTH_TOKEN>`. The token is set in `.env`:

```env
AUTH_TOKEN=...openssl rand -hex 24...
```

The same token must be hard-coded in the device's `include/config.h`.

### 4.3. Cache

`server/src/cache.ts` exports a `memo(key, ttlMs, fetcher)` helper:

- TTL hit → return cached.
- TTL miss + fetch succeeds → store + return.
- TTL miss + fetch throws → if a previous good value exists, **return
  that** and bump expiry by 60 s ("stale-while-error"). If no prior
  value, rethrow.

This is what keeps the Claude card showing the last good values when
Anthropic 429s us during dev cycles.

### 4.4. Endpoints

#### `GET /api/health`

Liveness check, no auth. Returns `{ ok: true, ts: <unix> }`.

#### `GET /api/weather`

Memo: 15 min. Backed by `qweather.ts` + `weather.ts`. Sample:

```json
{
  "city": "Beijing",
  "current": {
    "tempC": 25, "condition": "Sunny",
    "glyph": "SUN", "iconCode": "100",
    "windKmh": 2, "humidityPct": 28,
    "highC": 28, "lowC": 12
  },
  "air": {
    "aqi": 48, "level": "1", "category": "Excellent",
    "color": { "r": 0, "g": 228, "b": 0 },
    "primary": "PM 2.5"
  },
  "forecast": [
    { "day": "WED", "glyph": "SUN",   "iconCode": "100", "highC": 28, "lowC": 12 },
    { "day": "THU", "glyph": "PCLDY", "iconCode": "101", "highC": 25, "lowC": 14 },
    …5 entries…
  ],
  "fetchedAt": "…ISO…"
}
```

`glyph` is a coarse 6-bucket mapping (SUN/PCLDY/RAIN/SNOW/STORM/FOG)
kept for backward compat with the device's text-based forecast strip.
`iconCode` is the actual QWeather 3-digit code that drives the icon.

#### `GET /api/weather/icon/:code?size=128&fill=F2C94C&style=fill`

Memo: 24 h. Renders one of the vendored QWeather SVGs to PNG.

- `:code` — QWeather icon code (`100`, `101`, `300`, …). Validated to
  3–4 digits.
- `size` — output square dimension, 16–512 px.
- `fill` — 6-hex RGB substituted for `currentColor` in the SVG. Defaults
  per leading digit (yellow for sun-family, blue for rain, etc.).
- `style` — `fill` (silhouette `*-fill.svg`) or `outline` (line
  `*.svg`). Falls back to the other if the file's missing.

Returns `image/png` with `Cache-Control: public, max-age=86400`.

#### `GET /api/claude`

Memo: 60 s. Reads OAuth token from the macOS keychain entry
`Claude Code-credentials`, hits `https://api.anthropic.com/api/oauth/usage`
with `anthropic-beta: oauth-2025-04-20`. Pulls email from
`~/.claude.json`.

```json
{
  "email": "you@example.com",
  "emailMasked": "you*@*.com",
  "plan": "Max",
  "session": { "utilizationPct": 27, "resetsAt": "…", "resetIn": "31m to reset" },
  "weekly":  { "utilizationPct": 18, "resetsAt": "…", "resetIn": "13h 11m to reset" },
  "extraUsage": { "enabled": false },
  "fetchedAt": "…"
}
```

First call on a fresh server triggers the macOS Keychain Access prompt.
Click **Always Allow** once.

#### `GET /api/codex`

Memo: 60 s. Driven by `codex.ts`:

1. Lazy-spawn a long-lived `codex app-server` subprocess (the same
   JSON-RPC channel the codex-tui uses).
2. Send `initialize`, then `account/rateLimits/read`.
3. Decode email + `chatgpt_plan_type` from the `id_token` in
   `~/.codex/auth.json` (no JWT verification — just metadata extraction).
4. Shape into the **same envelope** as `/api/claude` so the device
   reuses `ClaudeData` for both.

```json
{
  "email": "you@example.com",
  "emailMasked": "you*@*.com",
  "plan": "Plus",
  "session": { "utilizationPct": 0, "resetIn": "4h 59m to reset", … },
  "weekly":  { "utilizationPct": 0, "resetIn": "Wed 21:17 reset", … },
  "extraUsage": { "enabled": false },
  "fetchedAt": "…"
}
```

The `codex` subprocess auto-respawns on exit (e.g. user runs
`codex logout`, codex CLI updates).

### 4.5. QWeather specifics

- **Auth**: EdDSA-JWT. Header `{ alg:"EdDSA", kid:<credential id>, typ:"JWT" }`,
  payload `{ sub:<projectId>, iat:now-30, exp:≤now+86400 }`. Sent as
  `Authorization: Bearer <jwt>`. JWT cached for 23 h to stay under the
  24 h cap.
- **Keypair**: PKCS#8 PEM at repo root (`ed25519-private.pem`,
  `ed25519-public.pem`). Public key uploaded to the QWeather console;
  server reads the private key via `QWEATHER_KEY_PATH`.
- **Endpoints used**:
  - `/geo/v2/city/lookup` — resolves a free-text query to a LocationID. Cached 24 h.
  - `/v7/weather/now` — current conditions, includes 3-digit `icon` + `text`.
  - `/v7/weather/7d` — daily forecast with `iconDay` / `tempMax` / etc.
  - `/airquality/v1/current/{lat}/{lon}` — air quality. The legacy
    `/v7/air/now` 403s on this project's plan; the new global
    `/airquality/v1/...` endpoint works. Returns multiple AQI standards
    (US-EPA, QAQI, CN-MEE, …) — we prefer US-EPA, fall back to QAQI,
    fall back to whichever is first.
- **`lang=en`**: appended to all weather/air calls. The default zh
  responses contain CJK in `text` / `category` and the device's
  Latin-only Montserrat fonts can't render them.

### 4.6. Icon vendoring

`server/assets/qweather-icons/` contains all 507 SVGs from
`github.com/qwd/Icons` (MIT) plus the upstream LICENSE. Files named
`{code}.svg` (outline) and `{code}-fill.svg` (silhouette). The SVGs use
`fill="currentColor"`, which `weatherIcon.ts` replaces with the
requested hex before handing to resvg.

To bump to a newer icon set:

```bash
cd /tmp && git clone --depth 1 https://github.com/qwd/Icons.git
cp Icons/icons/*.svg server/assets/qweather-icons/
cp Icons/LICENSE      server/assets/qweather-icons/LICENSE
```

---

## 5. Device firmware (`firmware/`)

### 5.1. Build & flash

```bash
cd firmware
~/.platformio/penv/bin/pio run
~/.platformio/penv/bin/pio run --target upload --upload-port /dev/cu.usbmodem111301
```

PIO config: `pioarduino` develop branch + arduino-esp32 master,
`board = esp32-p4-evboard`. M5Unified + M5GFX from upstream master,
`lvgl@^9.2.0`, `bblanchon/ArduinoJson@^7.2.1`. `LV_USE_LODEPNG` enabled
for PNG icon decode.

### 5.2. Wi-Fi: the Tab5 SDIO pin override

The Tab5 wires its ESP32-C6 over different GPIOs than the ESP32-P4
EvalBoard that arduino-esp32 defaults to. Without an explicit
`WiFi.setPins(...)` call **before** `WiFi.begin`, you get a cascade of
`H_SDIO_DRV: card init failed` and the C6 never responds.

```cpp
WiFi.setPins(/*CLK*/12, /*CMD*/13, /*D0*/11, /*D1*/10, /*D2*/9, /*D3*/8, /*RST*/15);
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
```

This is hidden behind `m5io::wifiBegin()`. Verified: `H_SDIO_DRV: card
init failed` (×16) before, `[wifi] up: RSSI -61 dBm` after.

### 5.3. The `m5_io` facade — why it exists

M5GFX bundles its own miniature LVGL-compat headers under
`lgfx/v1/lv_font/{area,color,draw_buf}.h` that define `lv_area_t`,
`LV_COLOR_FORMAT_*`, etc. with different guard names than full LVGL.
Including `<M5Unified.h>` *and* `<lvgl.h>` in the same translation unit
is a hard "conflicting declaration" error.

**Solution:** `src/m5_io.{h,cpp}` is the only TU that includes
`<M5Unified.h>` (and `<WiFi.h>`, `<HTTPClient.h>`). Its public surface
exposes only POD types (`int`, `uint8_t*`, …). Everything else
(`main.cpp`, `lvgl_bridge.cpp`, `screen_grid.cpp`, `data_poller.cpp`)
includes `<lvgl.h>` cleanly.

Public API summary:

```cpp
namespace m5io {
  void  begin();                                  // M5.begin + setSwapBytes(true)
  void  update();                                 // M5.update
  void  setRotation/setBrightness/width/height/touchEnabled();
  void  fillScreen(r, g, b);
  void  blit(x, y, w, h, pixels);                 // pushImage with byte-swap
  bool  readTouch(int* x, int* y);
  void  beep(freq, dur);
  int   batteryPct();  float batteryVolts();

  // Wi-Fi (hides setPins)
  void  wifiBegin(ssid, pass);
  bool  wifiConnected();  int wifiRssi();

  // HTTP
  bool  httpGetJson  (url, bearer, out_buf, out_cap);
  bool  httpGetBinary(url, bearer, out_buf, out_cap, *out_len);
}
```

### 5.4. LVGL bridge

`src/lvgl_bridge.{h,cpp}`:

- Allocates two PSRAM buffers (1280 × 60 px × 2 bytes ≈ 153 600 each)
  for partial-render double-buffering. Internal RAM on the P4 is too
  tight once arduino-esp32 + LVGL are linked; `LV_USE_STDLIB_MALLOC`
  routes the rest of LVGL's allocations through the system heap (which
  spills into the 32 MB PSRAM).
- `flush_cb` calls `m5io::blit` (a `pushImage` with `setSwapBytes(true)`
  — the ST7123 wants big-endian RGB565, ESP32 is little-endian, LVGL
  emits little-endian with `LV_COLOR_16_SWAP=0`; lgfx does the swap).
- `touch_cb` polls `m5io::readTouch` and feeds LVGL's pointer indev.
- `tick_cb` returns `millis()`.

Pre-build hack at `scripts/skip_lvgl_arm_asm.py`: empties LVGL's Helium
ARM `.S` files (RISC-V can't assemble them) and the `lv_font/*.c` that
M5GFX duplicates from LVGL.

### 5.5. Polling architecture

`data_poller.cpp` runs a FreeRTOS task pinned to **core 0**. (Arduino's
loopTask is core 1.) The task:

1. `m5io::wifiBegin`, wait up to 30 s for association.
2. Loop forever, every 2 s tick:
   - If due: fetch `/api/weather`, parse, stage; if `iconCode` changed,
     fetch `/api/weather/icon/{code}` and stage the PNG bytes.
   - If due: fetch `/api/claude`, parse, stage.
   - If due: fetch `/api/codex`, parse, stage.

Staging is a `xSemaphoreTake`/`Give` around three snapshot structs and
their dirty flags. `poller_drain()` runs **on the LVGL thread** every
loop tick (cheap when nothing's new): grabs whatever's dirty, hands to
`ui::updateWeatherCard` / `updateClaudeCard` / `updateCodexCard` /
`setWeatherIconPng`. This keeps all LVGL widget calls single-threaded
(LVGL isn't thread-safe by default) without freezing the UI on HTTP.

### 5.6. Weather icon path on device

- `LV_USE_LODEPNG=1` in `lv_conf.h` pulls LVGL's bundled lodepng decoder.
- `screen_grid.cpp` owns a 16 KB PSRAM buffer (`g_icon_buf`) and an
  `lv_image_dsc_t` with `cf = LV_COLOR_FORMAT_RAW_ALPHA`.
- On a fresh PNG: `lv_image_set_src(img, NULL); lv_image_set_src(img, &dsc);`
  forces lodepng to re-decode. (`lv_image_cache_drop` exists but isn't
  pulled in by `<lvgl.h>`; the NULL → dsc dance does the same job since
  the cache is disabled by default.)
- The PNG bytes must outlive the widget — lodepng may re-decode if
  LVGL's draw cache evicts.

### 5.7. UI layout (single 1280×720 grid screen)

```
   x=16 ── kCw=616 ── x=648 ── kCw=616 ── 1280
y=16  ┌──────────────────┬──────────────────┐
      │ Weather (TL)     │ Claude (TR)      │
      │ icon | temp/cond │ avatar email pill│
      │ AQI/H/L/Wind/Hum │ session bar      │
      │ 5-day strip      │ weekly bar       │
y=368 ├──────────────────┼──────────────────┤
      │ Codex (BL)       │ Inbox (BR)       │
      │ same template    │ placeholder list │
      │ as Claude        │                  │
y=720 └──────────────────┴──────────────────┘
```

Card style: 14 px radius, 1 px border, 3 px accent stripe along the top
in the card's brand color (yellow for weather, claude blue, codex green,
inbox pink). Background `#0A0E12`, cards `#14191F`.

Fonts in use (current; +4 px above the original after two rounds of
"bigger"):

| Use                          | Font           |
| ---------------------------- | -------------- |
| Big temperature              | `montserrat_48` (max stock) |
| Condition phrase             | `montserrat_32` |
| Email / inbox item / chev    | `montserrat_22` |
| Details (H/L, Wind, Hum, AQI)| `montserrat_22` |
| Session / Weekly labels      | `montserrat_20` |
| Card header / Inbox header   | `montserrat_20` |
| Plan pill / forecast         | `montserrat_18` |
| Reset right / pct / extra    | `montserrat_18` |
| City / Active pill / hint    | `montserrat_16` |

`LV_FONT_MONTSERRAT_30` and `_22` are explicitly enabled in `lv_conf.h`
(LVGL ships `_22` disabled by default; `_30` always disabled).

Right column (weather card) is **left-aligned at x=380** for H/L → Wind
→ Humidity → AQI. The forecast strip uses tight outer margins (8 px
horizontal, 6 px from the bottom of the card) to free vertical space
for the upper content.

The AQI line is tinted at runtime to QWeather's recommended badge color
(`d.air.r/g/b`) so green = Good, yellow = Moderate, red = Unhealthy
reads at a glance.

---

## 6. Configuration

### 6.1. Server (`server/.env`)

```env
PORT=8787
AUTH_TOKEN=<openssl rand -hex 24>

WEATHER_LOCATION=Beijing       # free-text query for QWeather GeoAPI
WEATHER_CITY=Beijing           # display label on the device

QWEATHER_HOST=your-subdomain.re.qweatherapi.com
QWEATHER_PROJECT_ID=your-project-id
QWEATHER_KID=your-credential-kid
QWEATHER_KEY_PATH=../ed25519-private.pem
```

### 6.2. Device (`firmware/include/config.h`)

```cpp
namespace cfg {
  constexpr const char* WIFI_SSID     = "<your-ssid>";
  constexpr const char* WIFI_PASSWORD = "<your-password>";
  constexpr int   BRIGHTNESS_ACTIVE   = 160;
  constexpr int   BRIGHTNESS_DIM      = 30;
  constexpr uint32_t IDLE_DIM_MS      = 5 * 60 * 1000;

  constexpr const char* SERVER_URL = "http://192.168.8.88:8787";
  constexpr const char* AUTH_TOKEN = "<must match server/.env>";
}
```

Both files are gitignored. Templates exist next to them
(`config.example.h`, `.env.example`).

---

## 7. End-to-end smoke test

Quick ground-truth path when something looks off:

```bash
# 1. Server is healthy
curl -s localhost:8787/api/health

# 2. Bearer auth works
TOK=$(grep AUTH_TOKEN server/.env | cut -d= -f2)
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/weather | jq .city
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/claude  | jq .plan
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/codex   | jq .plan

# 3. Icon route
curl -s -H "Authorization: Bearer $TOK" "localhost:8787/api/weather/icon/100?size=128" -o /tmp/x.png
file /tmp/x.png   # → PNG image data, 128 x 128, 8-bit/color RGBA

# 4. Reset device + read serial
~/.platformio/penv/bin/python -c '
import serial, time, sys
s = serial.Serial("/dev/cu.usbmodem111301", 115200, timeout=0.5)
s.setDTR(False); s.setRTS(True); time.sleep(0.1)
s.setDTR(True);  s.setRTS(False); time.sleep(0.1)
s.setDTR(False); s.setRTS(False)
end = time.time() + 25
while time.time() < end:
    d = s.read(4096)
    if d: sys.stdout.write(d.decode("utf-8","replace")); sys.stdout.flush()
'
# expected: [poller] WiFi up, [poller] weather, [poller] icon, [poller] claude, [poller] codex
```

---

## 8. Known issues & outstanding work

- **Inbox card is placeholder.** No data source decided yet.
- **Claude/Codex avatars are generic silhouettes.** Open question:
  swap to brand logomarks (Anthropic sunburst, OpenAI mark) via the
  same server-served PNG path as the weather icons.
- **Anthropic Usage API rate limits.** During heavy dev cycles their
  `/api/oauth/usage` returns 429. The cache's stale-while-error path
  keeps the screen showing the last-good values until it clears
  (typically minutes, not hours).
- **Codex CLI is alpha (`0.125.0-alpha.3`).** The `account/rateLimits/read`
  shape is stable today but documented as experimental. If we ever
  see a parse failure on `/api/codex` after a `codex` update, run
  `codex app-server generate-json-schema` and diff against
  `server/src/codex.ts`'s typed interfaces.
- **Custom font for big temperature.** The 48 pt stock Montserrat is
  the largest LVGL ships pre-converted. To go bigger (e.g. 72 / 96 pt),
  drop a Montserrat TTF into the repo and run `lv_font_conv` to emit a
  C font. Not blocked on anything technical.
- **Per-day forecast icons.** `data::ForecastDay` already carries
  `iconCode`; the strip currently shows the glyph as text. Swapping
  each row to a small `lv_image` (e.g. 32 px) is a screen_grid-only
  change.

---

## 9. Pointers to other docs

- [`M5Stack-Tab5-Reference.md`](./M5Stack-Tab5-Reference.md) — chip
  IDs, panel driver notes, ESP-Hosted/SDIO pin map, M5Burner
  workflows, every "why-doesn't-this-work" we've already debugged.
- `server/README.md` — server quick-start (older; some endpoints
  omitted — this file is the source of truth for the `air`/`codex`/icon
  additions).
- `firmware/README.md` — short bootstrap notes for the firmware project.
