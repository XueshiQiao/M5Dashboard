// weather.ts — /api/weather route shaper. Backed by QWeather (qweather.ts).
//
// Output shape is intentionally compatible with the running Tab5 firmware
// (city, current{tempC, condition, glyph, windKmh, humidityPct, highC, lowC},
// forecast[{day, glyph, highC, lowC}], fetchedAt) — the device keeps working
// with no flash. We additionally emit `iconCode` (QWeather 3-digit code) on
// the current block and on each forecast entry, for the upcoming icon-asset
// upgrade on the device side.

import { config } from "./config.js";
import { memo } from "./cache.js";
import {
  resolveLocation, getNow, getDaily7d, getAirNow,
  type QWeatherAir,
} from "./qweather.js";

interface WMO { phrase: string; glyph: string }

// Map QWeather's 3-digit icon codes to the 6 glyphs the device knows.
// Reference: https://dev.qweather.com/docs/resource/icons/
//   1xx  clear/cloudy   (1xx + 15x night)
//   2xx  wind/extreme
//   3xx  rain (302/303/304 + several others = thunderstorm flavors)
//   4xx  snow
//   5xx  fog/haze
//   9xx  unknown / hot / cold
const THUNDER_CODES = new Set([
  "302", "303", "304",       // thunder + thunder w/ hail
  "311", "318",              // heavy thunderstorm variants
]);

function qwToGlyph(code: string): WMO {
  const c   = code ?? "";
  const lead = c.charAt(0);
  if (c === "100" || c === "150") return { phrase: "Clear",         glyph: "SUN"   };
  if (lead === "1")               return { phrase: "Cloudy",        glyph: "PCLDY" };
  if (THUNDER_CODES.has(c))       return { phrase: "Thunderstorm",  glyph: "STORM" };
  if (lead === "3")               return { phrase: "Rain",          glyph: "RAIN"  };
  if (lead === "4")               return { phrase: "Snow",          glyph: "SNOW"  };
  if (lead === "5")               return { phrase: "Fog",           glyph: "FOG"   };
  if (lead === "2")               return { phrase: "Extreme",       glyph: "STORM" };
  return { phrase: "Unknown", glyph: "PCLDY" };
}

const DOW = ["SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"] as const;
function dayKey(fxDate: string): string {
  const d = new Date(`${fxDate}T00:00:00`);
  return DOW[d.getDay()] ?? "—";
}

export interface WeatherResp {
  city: string;
  current: {
    tempC:       number;
    condition:   string;
    glyph:       string;
    iconCode:    string;
    windKmh:     number;
    humidityPct: number;
    highC:       number;
    lowC:        number;
  };
  air: QWeatherAir | null;
  forecast: {
    day:      string;
    glyph:    string;
    iconCode: string;
    highC:    number;
    lowC:     number;
  }[];
  fetchedAt: string;
}

export async function getWeather(): Promise<WeatherResp> {
  return memo("weather", 15 * 60 * 1000, async () => {
    const loc = await resolveLocation(config.weather.locationQuery);
    // Run all three upstream calls in parallel. Air quality has its own
    // failure mode (the /airquality endpoint requires a different plan
    // permission than /v7/weather and can 403); fall back to null so
    // the rest of the card still renders.
    const [now, daily, air] = await Promise.all([
      getNow(loc.id),
      getDaily7d(loc.id),
      getAirNow(loc.lat, loc.lon).catch(e => {
        console.warn(`[weather] air fetch failed: ${(e as Error).message}`);
        return null;
      }),
    ]);

    const today = daily[0];
    const todayHi = today?.tempMaxC ?? now.tempC;
    const todayLo = today?.tempMinC ?? now.tempC;

    const forecast = daily.slice(0, 5).map(d => {
      const m = qwToGlyph(d.iconDay);
      return {
        day:      dayKey(d.fxDate),
        glyph:    m.glyph,
        iconCode: d.iconDay,
        highC:    d.tempMaxC,
        lowC:     d.tempMinC,
      };
    });

    const cur = qwToGlyph(now.iconCode);

    return {
      city: config.weather.city || loc.name,
      current: {
        tempC:       now.tempC,
        condition:   now.text || cur.phrase,
        glyph:       cur.glyph,
        iconCode:    now.iconCode,
        windKmh:     now.windKmh,
        humidityPct: now.humidityPct,
        highC:       todayHi,
        lowC:        todayLo,
      },
      air,
      forecast,
      fetchedAt: new Date().toISOString(),
    };
  });
}
