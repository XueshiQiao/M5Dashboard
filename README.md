# M5Dashboard

A personal information dashboard for the **M5Stack Tab5** (ESP32-P4, 5″ 1280×720 touchscreen). Four cards, one screen, real data.

| Card        | Source                                                                               |
| ----------- | ------------------------------------------------------------------------------------ |
| Weather     | [QWeather](https://dev.qweather.com) — current + 5-day forecast + AQI tinted by category |
| Claude Code | Anthropic OAuth Usage API (read from the local macOS keychain Claude Code installs)  |
| Codex       | OpenAI Codex CLI's local `app-server` (`account/rateLimits/read`)                    |
| Inbox       | [Kagi News](https://news.kagi.com) RSS — curated AI headlines                        |

## Architecture in one paragraph

The device firmware (LVGL 9 on top of M5Unified) polls a local Node.js server running on your Mac. The server fans out to every upstream — QWeather, the Anthropic Usage API, the local Codex CLI, Kagi News — and shapes each response into a single JSON envelope. The device never holds OAuth tokens or upstream API keys; only the LAN-bound bearer token shared with the server. Weather and brand icons are SVGs vendored on the server, rasterized to PNG via `@resvg/resvg-js`, and decoded on-device by LVGL's bundled lodepng.

For the full design — module breakdown, polling cadences, JWT auth flow, on-device data pipeline, gotchas — see [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

## Repo layout

```
M5Dashboard/
├── docs/                              hardware reference + architecture guide
├── server/                            Node + TS + Fastify backend (runs on your Mac)
│   ├── assets/qweather-icons/         vendored qwd/Icons SVGs (MIT)
│   ├── assets/brand-icons/            Claude / Codex logo SVGs
│   └── src/                           Fastify routes + provider modules
└── firmware/                         device firmware (LVGL 9, real data)
```

## Quick start

### 1. Server (your Mac)

```bash
cd server
npm install
cp .env.example .env
# edit .env — set AUTH_TOKEN (openssl rand -hex 24) and your QWeather creds
npm run dev
```

The server binds `0.0.0.0:8787`. Find your Mac's LAN IP with
`ipconfig getifaddr en0` and use it from the device.

### 2. Device firmware

```bash
cd firmware
cp include/config.example.h include/config.h
# edit include/config.h — Wi-Fi creds, server URL, AUTH_TOKEN matching server/.env
pio run --target upload --upload-port /dev/cu.usbmodem<your-tty>
```

First flash takes ~5 min (ESP32-P4 toolchain + LVGL build); subsequent builds are seconds.

## Hardware

- **M5Stack Tab5** (ESP32-P4 + ESP32-C6, 5″ 1280×720 MIPI-DSI touchscreen)
- USB-C cable to the host Mac (for flashing and powering)
- Mac on the same LAN as the Tab5 (Wi-Fi 6 capable)

## Required external accounts

- [QWeather](https://dev.qweather.com) — free tier is enough for personal use. You'll need an Ed25519 keypair (one OpenSSL command) and a project + credential ID.
- Claude Code installed and logged in (the server reads its OAuth token from your macOS keychain).
- OpenAI Codex CLI installed and logged in (the server spawns its `app-server` JSON-RPC).
- A Kagi subscription is **not** required for the news endpoint — `https://news.kagi.com/{slug}.xml` is public.

## What's NOT in this repo

Per `.gitignore`:

- `ed25519-private.pem` / `ed25519-public.pem` — your QWeather signing keypair
- `server/.env` — your AUTH_TOKEN and QWeather IDs
- `firmware/include/config.h` — your Wi-Fi creds, server URL, AUTH_TOKEN

Templates for each are checked in (`*.example.h`, `.env.example`).

## License

MIT — see [`LICENSE`](LICENSE).

## Acknowledgments

- [QWeather Icons (`qwd/Icons`)](https://github.com/qwd/Icons) — MIT-licensed weather SVG set.
- [LVGL](https://lvgl.io) — embedded GUI framework.
- [M5Unified](https://github.com/m5stack/M5Unified) / [M5GFX](https://github.com/m5stack/M5GFX) — Tab5 hardware abstraction.
- [pioarduino](https://github.com/pioarduino) — ESP32-P4 Arduino core.
