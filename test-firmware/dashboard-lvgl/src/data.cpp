// data.cpp — ArduinoJson 7 parsers for /api/weather and /api/claude.

#include "data.h"

#include <ArduinoJson.h>
#include <string.h>

namespace data {

namespace {

void copyStr(char* dst, size_t cap, const char* src) {
  if (!src) { dst[0] = '\0'; return; }
  size_t n = strnlen(src, cap - 1);
  memcpy(dst, src, n);
  dst[n] = '\0';
}

}  // namespace

bool parseWeather(const char* json, WeatherData* out) {
  out->valid = false;
  // /api/weather is well-bounded: ~600 bytes typical. 4KB doc has plenty
  // of headroom for ArduinoJson 7's variable-size internals.
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) return false;

  copyStr(out->city, sizeof(out->city), doc["city"] | "");

  JsonObjectConst cur = doc["current"];
  out->tempC       = cur["tempC"]       | 0;
  copyStr(out->condition, sizeof(out->condition), cur["condition"] | "");
  copyStr(out->glyph,     sizeof(out->glyph),     cur["glyph"]     | "PCLDY");
  copyStr(out->iconCode,  sizeof(out->iconCode),  cur["iconCode"]  | "999");
  out->windKmh     = cur["windKmh"]     | 0;
  out->humidityPct = cur["humidityPct"] | 0;
  out->highC       = cur["highC"]       | 0;
  out->lowC        = cur["lowC"]        | 0;

  // Air block — null on the wire when the upstream call failed; the
  // device just hides the AQI line in that case.
  JsonVariantConst airV = doc["air"];
  if (!airV.isNull()) {
    out->air.present = true;
    out->air.aqi     = (uint16_t)(airV["aqi"] | 0);
    copyStr(out->air.category, sizeof(out->air.category), airV["category"] | "—");
    copyStr(out->air.primary,  sizeof(out->air.primary),  airV["primary"]  | "—");
    out->air.r = (uint8_t)(airV["color"]["r"] | 0);
    out->air.g = (uint8_t)(airV["color"]["g"] | 0);
    out->air.b = (uint8_t)(airV["color"]["b"] | 0);
  } else {
    out->air = {};
    out->air.present = false;
  }

  JsonArrayConst fc = doc["forecast"];
  for (int i = 0; i < 5; ++i) {
    auto& d = out->forecast[i];
    if (i < (int)fc.size()) {
      JsonObjectConst row = fc[i];
      copyStr(d.day,      sizeof(d.day),      row["day"]      | "—");
      copyStr(d.glyph,    sizeof(d.glyph),    row["glyph"]    | "PCLDY");
      copyStr(d.iconCode, sizeof(d.iconCode), row["iconCode"] | "999");
      d.highC = row["highC"] | 0;
      d.lowC  = row["lowC"]  | 0;
    } else {
      copyStr(d.day,      sizeof(d.day),      "—");
      copyStr(d.glyph,    sizeof(d.glyph),    "PCLDY");
      copyStr(d.iconCode, sizeof(d.iconCode), "999");
      d.highC = 0;
      d.lowC  = 0;
    }
  }

  out->valid = true;
  return true;
}

bool parseClaude(const char* json, ClaudeData* out) {
  out->valid = false;
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) return false;

  copyStr(out->email,       sizeof(out->email),       doc["email"]       | "unknown");
  copyStr(out->emailMasked, sizeof(out->emailMasked), doc["emailMasked"] | "");
  copyStr(out->plan,        sizeof(out->plan),        doc["plan"]        | "Pro");

  JsonVariantConst sess = doc["session"];
  if (!sess.isNull()) {
    out->session.present        = true;
    out->session.utilizationPct = sess["utilizationPct"] | 0;
    copyStr(out->session.resetIn, sizeof(out->session.resetIn),
            sess["resetIn"] | "—");
  } else {
    out->session.present        = false;
    out->session.utilizationPct = 0;
    copyStr(out->session.resetIn, sizeof(out->session.resetIn), "—");
  }

  JsonVariantConst wk = doc["weekly"];
  if (!wk.isNull()) {
    out->weekly.present        = true;
    out->weekly.utilizationPct = wk["utilizationPct"] | 0;
    copyStr(out->weekly.resetIn, sizeof(out->weekly.resetIn),
            wk["resetIn"] | "—");
  } else {
    out->weekly.present        = false;
    out->weekly.utilizationPct = 0;
    copyStr(out->weekly.resetIn, sizeof(out->weekly.resetIn), "—");
  }

  out->extraEnabled = doc["extraUsage"]["enabled"] | false;

  out->valid = true;
  return true;
}

bool parseCodex(const char* json, CodexData* out) {
  return parseClaude(json, out);
}

bool parseNews(const char* json, NewsData* out) {
  out->valid = false;
  out->count = 0;
  for (int i = 0; i < 3; ++i) {
    out->items[i].title[0] = '\0';
    out->items[i].score    = 0;
  }
  // Items are short titles; 4 KB doc has plenty of room for the
  // first 3 entries we keep. Use a filter so we don't allocate the
  // long URL fields we never read.
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) return false;

  JsonArrayConst arr = doc["items"];
  for (JsonObjectConst row : arr) {
    if (out->count >= 3) break;
    auto& it = out->items[out->count];
    copyStr(it.title, sizeof(it.title), row["title"] | "—");
    it.score = row["score"] | 0;
    out->count++;
  }
  out->valid = true;
  return true;
}

}  // namespace data
