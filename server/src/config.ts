// config.ts — env-var parsing + minimal validation.

const PLACEHOLDER_TOKEN = "change-me-set-via-openssl-rand-hex-24";

function num(name: string, fallback: number): number {
  const raw = process.env[name];
  if (!raw) return fallback;
  const n = Number(raw);
  if (!Number.isFinite(n)) {
    throw new Error(`env ${name} is not a number: ${raw}`);
  }
  return n;
}

function str(name: string, fallback: string): string {
  return process.env[name] ?? fallback;
}

export const config = {
  port:      num("PORT", 8787),
  authToken: str("AUTH_TOKEN", ""),
  weather: {
    // Free-text location query — sent to QWeather's GeoAPI to resolve a
    // LocationID. Accepts a city name, an "adm,name" pair, or "lon,lat".
    locationQuery: str("WEATHER_LOCATION", "Singapore"),
    // Display label on the device when we want a city string. Falls back
    // to the resolved name if unset.
    city:          str("WEATHER_CITY", "Singapore"),
  },
  qweather: {
    host:           str("QWEATHER_HOST",       ""),
    projectId:      str("QWEATHER_PROJECT_ID", ""),
    kid:            str("QWEATHER_KID",        ""),
    privateKeyPath: str("QWEATHER_KEY_PATH",   "../ed25519-private.pem"),
  },
  news: {
    // Kagi News RSS feed slug (https://news.kagi.com/{slug}.xml). The
    // tech feed is broader; ai is more focused.
    feedSlug: str("NEWS_FEED_SLUG", "ai"),
  },
} as const;

export function assertConfigSane(): void {
  if (!config.authToken || config.authToken === PLACEHOLDER_TOKEN) {
    throw new Error(
      "AUTH_TOKEN is unset or still the placeholder. Set it in .env to a real value " +
      "(suggestion: AUTH_TOKEN=$(openssl rand -hex 24)) and restart.",
    );
  }
  const q = config.qweather;
  if (!q.host || !q.projectId || !q.kid) {
    throw new Error(
      "QWEATHER_HOST / QWEATHER_PROJECT_ID / QWEATHER_KID must all be set in .env. " +
      "Sign up at https://dev.qweather.com, create a project + credential, and " +
      "copy the values from the console.",
    );
  }
}
