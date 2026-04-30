// brandIcon.ts — rasterize multi-color brand SVGs to PNG.
//
// Unlike the QWeather icons (single `currentColor` substitution), these
// SVGs are full-color logos (linearGradient + per-path fills). We pass
// them straight through resvg without color rewriting.
//
// Lives under server/assets/brand-icons/{name}-color.svg. Memo'd
// indefinitely per (name, size) — these don't change.

import { readFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

import { Resvg } from "@resvg/resvg-js";

const __dirname  = dirname(fileURLToPath(import.meta.url));
const ICONS_DIR  = join(__dirname, "..", "assets", "brand-icons");

interface CacheEntry { png: Buffer }
const cache = new Map<string, CacheEntry>();

function sanitizeName(name: string): string {
  // Whitelist: lowercase letters, digits, hyphen. Keeps the path
  // join out of trouble.
  if (!/^[a-z0-9-]{1,32}$/.test(name)) {
    throw new Error(`bad brand icon name '${name}'`);
  }
  return name;
}

export async function getBrandIconPng(name: string, size: number): Promise<Buffer> {
  const safe = sanitizeName(name);
  const px   = Math.max(16, Math.min(512, Math.floor(size)));
  const key  = `${safe}:${px}`;
  const hit  = cache.get(key);
  if (hit) return hit.png;

  const svg = await readFile(join(ICONS_DIR, `${safe}-color.svg`), "utf-8");
  const png = new Resvg(svg, {
    fitTo: { mode: "width", value: px },
    background: "rgba(0,0,0,0)",
  }).render().asPng();
  cache.set(key, { png });
  return png;
}
