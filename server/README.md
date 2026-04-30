# M5Dashboard server

Node + TypeScript + Fastify HTTP API that the M5Stack Tab5 dashboard polls. Runs on the same Mac as Claude Code so it can read the OAuth token out of the macOS keychain.

## Endpoints

| Route | Auth | Cache | Source |
|---|---|---|---|
| `GET /api/health`  | none   | –      | local |
| `GET /api/weather` | bearer | 15 min | Open-Meteo (no API key) |
| `GET /api/claude`  | bearer | 60 s   | macOS keychain (`Claude Code-credentials`) → Anthropic OAuth Usage API |

All authenticated routes require `Authorization: Bearer <AUTH_TOKEN>` matching the value in `.env`.

## Run

```bash
cd server
npm install
cp .env.example .env
# edit .env — at minimum set AUTH_TOKEN to:
#   $(openssl rand -hex 24)
npm run dev
```

The server binds `0.0.0.0:8787`. Find your Mac's LAN IP with `ipconfig getifaddr en0` (Wi-Fi) or `en1` (Ethernet) and use that from the Tab5.

### First run on macOS

The first call to `/api/claude` triggers macOS to ask whether `node` may read the `Claude Code-credentials` keychain entry. Click **"Always Allow"** — subsequent requests won't prompt.

## Smoke test

```bash
TOK=$(grep AUTH_TOKEN .env | cut -d= -f2)
curl -s localhost:8787/api/health | jq
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/weather | jq
curl -s -H "Authorization: Bearer $TOK" localhost:8787/api/claude  | jq
```

## Layout

```
server/
├── package.json
├── tsconfig.json
├── .env.example         (template; copy to .env)
└── src/
    ├── index.ts         (Fastify entry, route registration)
    ├── config.ts        (env-var parsing)
    ├── auth.ts          (bearer-token preHandler)
    ├── cache.ts         (TTL memo helper)
    ├── weather.ts       (Open-Meteo fetcher + WMO code mapping)
    └── claude.ts        (keychain read + Anthropic Usage API)
```

## Notes

- **Why bind to 0.0.0.0 instead of localhost?** The Tab5 needs to reach this server over the LAN. Bearer auth keeps random LAN devices out.
- **Why not use the Claude CLI's `auth status`?** Reading `~/.claude.json` and the keychain directly is faster and avoids spawning the CLI on every request.
- **Token refresh**: if Anthropic returns 401, the OAuth token expired. Claude Code itself handles refresh on its next interaction — this server doesn't refresh tokens, just reads the latest one from the keychain.
