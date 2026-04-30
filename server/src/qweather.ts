// qweather.ts — QWeather (HeFeng) provider with EdDSA-JWT auth.
//
// Auth ref: https://dev.qweather.com/docs/configuration/authentication/
//   header  { alg:"EdDSA", kid:<credential id>, typ:"JWT" }
//   payload { sub:<project id>, iat:<now-30s>, exp:<≤now+86400s> }
//   transport: Authorization: Bearer <jwt>
//
// API ref: https://dev.qweather.com/docs/api/weather/
//   GET https://{host}/v7/weather/now?location={id|lon,lat}
//   GET https://{host}/v7/weather/{3d|7d}?location=...
//   GET https://{host}/geo/v2/city/lookup?location=<query>

import { readFile } from "node:fs/promises";

import { SignJWT, importPKCS8 } from "jose";

import { config } from "./config.js";
import { memo } from "./cache.js";

interface NowResp {
  code:       string;
  updateTime: string;
  now: {
    obsTime:    string;
    temp:       string;
    feelsLike:  string;
    icon:       string;
    text:       string;
    wind360:    string;
    windDir:    string;
    windScale:  string;
    windSpeed:  string;
    humidity:   string;
    precip:     string;
    pressure:   string;
    vis:        string;
    cloud?:     string;
    dew?:       string;
  };
}

interface DailyEntry {
  fxDate:       string;
  sunrise:      string;
  sunset:       string;
  tempMax:      string;
  tempMin:      string;
  iconDay:      string;
  textDay:      string;
  iconNight:    string;
  textNight:    string;
  windDirDay:   string;
  windScaleDay: string;
  windSpeedDay: string;
  humidity:     string;
  precip:       string;
  pressure:     string;
  vis:          string;
  cloud?:       string;
  uvIndex?:     string;
}

interface DailyResp {
  code:       string;
  updateTime: string;
  daily:      DailyEntry[];
}

interface GeoCity {
  id:      string;
  name:    string;
  lat:     string;
  lon:     string;
  country: string;
}

interface GeoResp {
  code:     string;
  location: GeoCity[];
}

// QWeather quirk: the API returns code:"200" for success on weather/geo
// endpoints. Anything else is a soft failure with a message.
function ensureOk(code: string, where: string): void {
  if (code !== "200") {
    throw new Error(`qweather ${where} returned code=${code}`);
  }
}

let cachedJwt:    string | null = null;
let cachedJwtExp: number        = 0;
let cachedKey:    CryptoKey | null = null;

async function loadKey(): Promise<CryptoKey> {
  if (cachedKey) return cachedKey;
  const pem = await readFile(config.qweather.privateKeyPath, "utf-8");
  cachedKey = await importPKCS8(pem, "EdDSA");
  return cachedKey;
}

async function getJwt(): Promise<string> {
  const now = Math.floor(Date.now() / 1000);
  // Refresh ~1h before the 24h expiry.
  if (cachedJwt && cachedJwtExp - now > 3600) return cachedJwt;

  const key = await loadKey();
  const exp = now + 23 * 3600;        // 23 h validity
  cachedJwt = await new SignJWT({})
    .setProtectedHeader({ alg: "EdDSA", kid: config.qweather.kid, typ: "JWT" })
    .setSubject(config.qweather.projectId)
    .setIssuedAt(now - 30)
    .setExpirationTime(exp)
    .sign(key);
  cachedJwtExp = exp;
  return cachedJwt;
}

async function qfetch<T>(path: string): Promise<T> {
  const jwt = await getJwt();
  // Append lang=en — QWeather defaults to zh and returns CJK text in
  // `now.text` / `daily.textDay`, which the device's Latin-only Montserrat
  // fonts can't render.
  const sep = path.includes("?") ? "&" : "?";
  const res = await fetch(`https://${config.qweather.host}${path}${sep}lang=en`, {
    headers: {
      Authorization:    `Bearer ${jwt}`,
      "Accept-Encoding": "identity",   // skip gzip — easier to debug; payloads are tiny
    },
  });
  if (!res.ok) {
    const body = await res.text().catch(() => "");
    throw new Error(`qweather http ${res.status} ${path}: ${body.slice(0, 200)}`);
  }
  return (await res.json()) as T;
}

export interface ResolvedLocation {
  id:   string;
  name: string;
  lat:  number;
  lon:  number;
}

export async function resolveLocation(query: string): Promise<ResolvedLocation> {
  return memo(`qw:loc:${query}`, 24 * 3600 * 1000, async () => {
    const j = await qfetch<GeoResp>(
      `/geo/v2/city/lookup?location=${encodeURIComponent(query)}&number=1`,
    );
    ensureOk(j.code, "geo/lookup");
    const top = j.location?.[0];
    if (!top) throw new Error(`qweather geo: no match for '${query}'`);
    return {
      id:   top.id,
      name: top.name,
      lat:  Number(top.lat),
      lon:  Number(top.lon),
    };
  });
}

export interface QWeatherCurrent {
  iconCode:    string;
  text:        string;
  tempC:       number;
  windKmh:     number;
  humidityPct: number;
}

export interface QWeatherDaily {
  fxDate:    string;
  iconDay:   string;
  textDay:   string;
  tempMaxC:  number;
  tempMinC:  number;
}

export interface QWeatherAir {
  aqi:       number;        // numeric, US-EPA index (0–500 scale)
  level:     string;        // "1".."6" — QWeather's level bucket
  category:  string;        // "Good" / "Moderate" / "Unhealthy" / etc.
  color:     { r: number; g: number; b: number };  // recommended badge color
  primary:   string;        // primary pollutant short name, e.g. "PM 2.5"
}

interface AirResp {
  indexes: Array<{
    code:        string;
    name:        string;
    aqi:         number;
    aqiDisplay?: string;
    level:       string;
    category:    string;
    color: { red: number; green: number; blue: number; alpha: number };
    primaryPollutant: { code: string; name: string; fullName: string };
  }>;
}

export async function getNow(locationId: string): Promise<QWeatherCurrent> {
  const j = await qfetch<NowResp>(`/v7/weather/now?location=${locationId}`);
  ensureOk(j.code, "weather/now");
  return {
    iconCode:    j.now.icon,
    text:        j.now.text,
    tempC:       Math.round(Number(j.now.temp)),
    windKmh:     Math.round(Number(j.now.windSpeed)),
    humidityPct: Math.round(Number(j.now.humidity)),
  };
}

export async function getDaily7d(locationId: string): Promise<QWeatherDaily[]> {
  const j = await qfetch<DailyResp>(`/v7/weather/7d?location=${locationId}`);
  ensureOk(j.code, "weather/7d");
  return j.daily.map(d => ({
    fxDate:   d.fxDate,
    iconDay:  d.iconDay,
    textDay:  d.textDay,
    tempMaxC: Math.round(Number(d.tempMax)),
    tempMinC: Math.round(Number(d.tempMin)),
  }));
}

// Air quality lives under a different URL family than the weather APIs:
//   /airquality/v1/current/{lat}/{lon}
// — the legacy /v7/air/now endpoint we tried first returned 403 ("No
// permission to request this data") on this project's plan, while the
// new global /airquality endpoint works.
//
// The response carries multiple AQI indexes (qaqi global 0–5, us-epa
// 0–500, plus per-country standards). We surface US-EPA because it's the
// most universally familiar number; the device shows that with the
// `category` text and QWeather's recommended display color.
export async function getAirNow(lat: number, lon: number): Promise<QWeatherAir | null> {
  // Endpoint requires up to 2 decimal places; rounding here matches what
  // resolveLocation also returns.
  const round = (n: number) => Math.round(n * 100) / 100;
  const j = await qfetch<AirResp>(
    `/airquality/v1/current/${round(lat)}/${round(lon)}`,
  );
  if (!Array.isArray(j.indexes) || j.indexes.length === 0) return null;

  // Prefer us-epa, fall back to qaqi, otherwise first index returned.
  const pick =
    j.indexes.find(i => i.code === "us-epa") ??
    j.indexes.find(i => i.code === "qaqi") ??
    j.indexes[0];
  if (!pick) return null;

  return {
    aqi:      Math.round(pick.aqi),
    level:    pick.level,
    category: pick.category,
    color: {
      r: pick.color.red,
      g: pick.color.green,
      b: pick.color.blue,
    },
    primary:  pick.primaryPollutant?.name ?? "—",
  };
}
