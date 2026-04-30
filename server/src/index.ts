// index.ts — Fastify entry. Wires routes, applies bearer auth, starts
// the HTTP listener on 0.0.0.0 so the Tab5 on the LAN can reach it.

import Fastify from "fastify";

import { assertConfigSane, config } from "./config.js";
import { attachBearerAuth } from "./auth.js";
import { getWeather } from "./weather.js";
import { getClaude  } from "./claude.js";
import { getCodex   } from "./codex.js";
import { getNews    } from "./news.js";
import { getWeatherIconPng, defaultFillFor } from "./weatherIcon.js";
import { getBrandIconPng } from "./brandIcon.js";

assertConfigSane();

// Default pino JSON logger — no pino-pretty dep needed.
const app = Fastify({ logger: true });

attachBearerAuth(app);

app.get("/api/health", async () => ({
  ok: true,
  ts: Math.floor(Date.now() / 1000),
}));

app.get("/api/weather", async (_req, reply) => {
  try {
    return await getWeather();
  } catch (e) {
    reply.code(502).send({
      error:  "weather_upstream_failed",
      detail: (e as Error).message,
    });
  }
});

// QWeather icon → PNG. `code` is the QWeather 3-digit (or 4-digit) icon
// code; `size` is the output square dimension in pixels; `fill` is a
// 6-char hex RGB (no '#'); `style` is `fill` (silhouette) or `outline`.
// Defaults give a readable per-condition tint, e.g. yellow for sun.
app.get<{
  Params: { code: string };
  Querystring: { size?: string; fill?: string; style?: string };
}>("/api/weather/icon/:code", async (req, reply) => {
  try {
    const code = req.params.code;
    const size = req.query.size ? Number(req.query.size) : 128;
    const fill = req.query.fill ?? defaultFillFor(code);
    const fillVariant = (req.query.style ?? "fill") === "fill";
    const png = await getWeatherIconPng({ code, size, fill, fillVariant });
    reply
      .header("Content-Type", "image/png")
      .header("Cache-Control", "public, max-age=86400")
      .send(png);
  } catch (e) {
    reply.code(400).send({
      error:  "icon_failed",
      detail: (e as Error).message,
    });
  }
});

// Brand logo (Claude / Codex / future) → PNG. The SVGs under
// server/assets/brand-icons/{name}-color.svg keep their built-in
// gradients and colors; we only rasterize.
app.get<{
  Params: { name: string };
  Querystring: { size?: string };
}>("/api/icon/:name", async (req, reply) => {
  try {
    const size = req.query.size ? Number(req.query.size) : 64;
    const png  = await getBrandIconPng(req.params.name, size);
    reply
      .header("Content-Type", "image/png")
      .header("Cache-Control", "public, max-age=86400")
      .send(png);
  } catch (e) {
    reply.code(404).send({
      error:  "icon_not_found",
      detail: (e as Error).message,
    });
  }
});

app.get("/api/claude", async (_req, reply) => {
  try {
    return await getClaude();
  } catch (e) {
    reply.code(502).send({
      error:  "claude_failed",
      detail: (e as Error).message,
    });
  }
});

app.get("/api/codex", async (_req, reply) => {
  try {
    return await getCodex();
  } catch (e) {
    reply.code(502).send({
      error:  "codex_failed",
      detail: (e as Error).message,
    });
  }
});

app.get("/api/news", async (_req, reply) => {
  try {
    return await getNews();
  } catch (e) {
    reply.code(502).send({
      error:  "news_failed",
      detail: (e as Error).message,
    });
  }
});

await app.listen({ host: "0.0.0.0", port: config.port });
app.log.info(
  `m5dashboard-server up on :${config.port}  (weather=${config.weather.city})`,
);
