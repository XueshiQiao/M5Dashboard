// weatherIcon.ts — rasterizes QWeather SVG icons to PNG at request time.
//
// The QWeather icon set (qwd/Icons, MIT) is vendored under
// server/assets/qweather-icons. SVGs are 16×16 monochrome with
// `fill="currentColor"`. We substitute the actual color in the SVG text
// before handing it to resvg, then return a PNG sized to the device's
// requested pixel dimensions.
//
// Result is cached in-memory keyed by (code, size, fill) so repeated
// device fetches are free.

import { readFile } from "node:fs/promises";
import { join, dirname } from "node:path";
import { fileURLToPath } from "node:url";

import { Resvg } from "@resvg/resvg-js";

const __dirname  = dirname(fileURLToPath(import.meta.url));
const ICONS_DIR  = join(__dirname, "..", "assets", "qweather-icons");

interface CacheEntry { png: Buffer; mtime: number }
const cache = new Map<string, CacheEntry>();

function cacheKey(code: string, size: number, fill: string, fillVariant: boolean) {
  return `${code}:${fillVariant ? "f" : "o"}:${size}:${fill}`;
}

// Validate user input cheaply — these flow into a filesystem path.
function sanitizeCode(code: string): string {
  if (!/^[0-9]{3,4}$/.test(code)) throw new Error(`bad icon code '${code}'`);
  return code;
}
function sanitizeFill(fill: string): string {
  if (!/^[0-9a-fA-F]{6}$/.test(fill)) throw new Error(`bad fill '${fill}'`);
  return fill.toUpperCase();
}

export interface IconOpts {
  code:        string;       // QWeather icon code, e.g. "100"
  size:        number;       // output pixel dimension (square)
  fill:        string;       // 6-hex RGB without leading '#', e.g. "F2C94C"
  fillVariant: boolean;      // true → use the -fill.svg silhouette variant
}

export async function getWeatherIconPng(opts: IconOpts): Promise<Buffer> {
  const code = sanitizeCode(opts.code);
  const fill = sanitizeFill(opts.fill);
  const size = Math.max(16, Math.min(512, Math.floor(opts.size)));
  const key  = cacheKey(code, size, fill, opts.fillVariant);

  const hit = cache.get(key);
  if (hit) return hit.png;

  const fileName = opts.fillVariant ? `${code}-fill.svg` : `${code}.svg`;
  let svg: string;
  try {
    svg = await readFile(join(ICONS_DIR, fileName), "utf-8");
  } catch {
    // Fall back to the opposite variant so callers always get something.
    svg = await readFile(
      join(ICONS_DIR, opts.fillVariant ? `${code}.svg` : `${code}-fill.svg`),
      "utf-8",
    );
  }

  // currentColor substitution. The vendored SVGs use either
  //   fill="currentColor" or stroke="currentColor"
  // — replace both so multi-stroke/fill paths render correctly.
  const tinted = svg
    .replace(/currentColor/g, `#${fill}`);

  const png = new Resvg(tinted, {
    fitTo: { mode: "width", value: size },
    background: "rgba(0,0,0,0)",
  }).render().asPng();

  cache.set(key, { png, mtime: Date.now() });
  return png;
}

// Default fill color per leading digit, so a request without an explicit
// fill still gets readable accents (yellow sun, blue rain, etc.).
export function defaultFillFor(code: string): string {
  const lead = code?.charAt(0) ?? "";
  switch (lead) {
    case "1": return "F2C94C";  // sun / cloud → yellow accent
    case "2": return "FF7676";  // extreme → red
    case "3": return "6FCBFF";  // rain → blue
    case "4": return "E0F2FF";  // snow → near-white blue
    case "5": return "9CA8B5";  // fog/haze → grey
    case "9": return "E6EDF3";  // misc → soft white
    default:  return "E6EDF3";
  }
}
