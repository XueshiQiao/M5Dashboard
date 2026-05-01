// VIBEHUB terminal cockpit layout — registers as ui::kGridLayout.
//
// Pure LVGL plus the m5_io facade. This file deliberately avoids M5Unified
// and M5GFX includes so their bundled mini-LVGL types do not collide with
// the real LVGL headers.

#include "ui/layouts/grid/grid_layout.h"

#include "m5_io.h"
#include "ui/fonts/fonts.h"

#include <esp_heap_caps.h>
#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

constexpr int kWeatherIconPx  = 128;
constexpr int kBrandIconPx    = 34;
constexpr int kForecastIconPx = 64;

struct StatusHandles {
  lv_obj_t* wifi;
  lv_obj_t* battery;
};

struct WeatherHandles {
  lv_obj_t* city;
  lv_obj_t* temp;
  lv_obj_t* condition;
  lv_obj_t* range;          // "H 18°C / L 12°C"
  lv_obj_t* aqi;
  lv_obj_t* humidity;
  lv_obj_t* wind;
  lv_obj_t* icon;
  lv_obj_t* forecast_day[5];
  lv_obj_t* forecast_temp[5];
  lv_obj_t* forecast_icon[5];
};

struct UsageHandles {
  lv_obj_t* icon;
  lv_obj_t* user;
  lv_obj_t* plan_label;
  lv_obj_t* session_bar;
  lv_obj_t* session_pct;
  lv_obj_t* session_reset;
  lv_obj_t* weekly_bar;
  lv_obj_t* weekly_pct;
  lv_obj_t* weekly_reset;
  lv_obj_t* extra;
};

StatusHandles  g_status = {};
WeatherHandles g_wx = {};
UsageHandles   g_claude = {};
UsageHandles   g_codex = {};

// Header has to fit the 4× Tamzen "DASHBOARD" hero (80 px tall).
constexpr int kHeaderX = 14;
constexpr int kHeaderY = 12;
constexpr int kHeaderW = 1252;
constexpr int kHeaderH = 130;

// Two-column grid: weather on the left, claude/codex stacked on the right.
// Adjust kLeftColPct to shift the split (50 = even, 67 = weather-favoured).
constexpr int kLeftColPct  = 67;
constexpr int kCardsX      = 14;
constexpr int kCardsY      = kHeaderY + kHeaderH + 12;
constexpr int kCardsW      = 1252;
constexpr int kCardsH      = 720 - kCardsY - 12;
constexpr int kColGap      = 12;
constexpr int kLeftColW    = (kCardsW - kColGap) * kLeftColPct / 100;
constexpr int kRightColW   = kCardsW - kColGap - kLeftColW;
constexpr int kRightColX   = kCardsX + kLeftColW + kColGap;
constexpr int kRightCardH  = (kCardsH - kColGap) / 2;

constexpr uint32_t kBg        = 0x020404;
constexpr uint32_t kPanel     = 0x050807;
constexpr uint32_t kPanelAlt  = 0x08100F;
constexpr uint32_t kGrid      = 0x14332D;
constexpr uint32_t kText      = 0xE7E2D8;
constexpr uint32_t kMuted     = 0x8B8A84;
constexpr uint32_t kDim       = 0x4A5553;
constexpr uint32_t kCyan      = 0x20E8FF;
constexpr uint32_t kGreen     = 0x67FF1E;
constexpr uint32_t kYellow    = 0xFFD21A;
constexpr uint32_t kOrange    = 0xFF9D00;
constexpr uint32_t kPink      = 0xFF1C7D;
constexpr uint32_t kPurple    = 0xD63BFF;
constexpr uint32_t kBlue      = 0x36A3FF;
constexpr uint32_t kRed       = 0xFF5B3E;

constexpr size_t kIconBufBytes = 14 * 1024;
uint8_t* g_weather_icon_buf = nullptr;
size_t g_weather_icon_len = 0;
lv_image_dsc_t g_weather_icon_dsc = {};

constexpr size_t kForecastIconBufBytes = 8 * 1024;
struct ForecastIconBuf {
  uint8_t*       buf = nullptr;
  size_t         len = 0;
  lv_image_dsc_t dsc = {};
};
ForecastIconBuf g_forecast_icon[5];

constexpr size_t kBrandBufBytes = 8 * 1024;
struct BrandIcon {
  uint8_t*       buf = nullptr;
  size_t         len = 0;
  lv_image_dsc_t dsc = {};
};
BrandIcon g_brand_claude;
BrandIcon g_brand_codex;
lv_timer_t* g_status_timer = nullptr;

int clampPct(int value) {
  if (value < 0) return 0;
  if (value > 100) return 100;
  return value;
}

lv_obj_t* makeLabel(lv_obj_t* parent, const char* text,
                    const lv_font_t* font, uint32_t color,
                    lv_text_align_t align = LV_TEXT_ALIGN_LEFT) {
  lv_obj_t* label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
  lv_obj_set_style_text_align(label, align, 0);
  lv_obj_set_style_text_letter_space(label, 0, 0);
  lv_obj_set_style_text_line_space(label, 5, 0);
  return label;
}

lv_obj_t* makeRule(lv_obj_t* parent, int x, int y, int w, uint32_t color) {
  lv_obj_t* rule = lv_obj_create(parent);
  lv_obj_remove_style_all(rule);
  lv_obj_set_pos(rule, x, y);
  lv_obj_set_size(rule, w, 1);
  lv_obj_set_style_bg_color(rule, lv_color_hex(color), 0);
  lv_obj_set_style_bg_opa(rule, LV_OPA_COVER, 0);
  return rule;
}

lv_obj_t* makePanel(lv_obj_t* parent, int x, int y, int w, int h,
                    const char* title, uint32_t accent) {
  lv_obj_t* panel = lv_obj_create(parent);
  lv_obj_remove_style_all(panel);
  lv_obj_set_pos(panel, x, y);
  lv_obj_set_size(panel, w, h);
  lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(panel, lv_color_hex(kPanel), 0);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(panel, 1, 0);
  lv_obj_set_style_border_color(panel, lv_color_hex(accent), 0);
  lv_obj_set_style_radius(panel, 0, 0);

  lv_obj_t* top = lv_obj_create(panel);
  lv_obj_remove_style_all(top);
  lv_obj_set_pos(top, 0, 0);
  lv_obj_set_size(top, w, 3);
  lv_obj_set_style_bg_color(top, lv_color_hex(accent), 0);
  lv_obj_set_style_bg_opa(top, LV_OPA_COVER, 0);

  if (title && title[0]) {
    lv_obj_t* title_label = makeLabel(panel, title, &lv_font_montserrat_18, accent);
    lv_obj_set_size(title_label, w - 28, 24);
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 14, 12);
    makeRule(panel, 14, 44, w - 28, kGrid);
  }

  return panel;
}

lv_obj_t* makePlanPill(lv_obj_t* parent, uint32_t accent) {
  lv_obj_t* pill = lv_obj_create(parent);
  lv_obj_remove_style_all(pill);
  lv_obj_set_size(pill, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_radius(pill, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_bg_color(pill, lv_color_hex(accent), 0);
  lv_obj_set_style_bg_opa(pill, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(pill, 0, 0);
  lv_obj_set_style_pad_hor(pill, 14, 0);
  lv_obj_set_style_pad_ver(pill, 5, 0);
  lv_obj_set_scrollbar_mode(pill, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(pill, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* label = makeLabel(pill, "-", &lv_font_montserrat_18, 0xFFFFFF,
                              LV_TEXT_ALIGN_CENTER);
  lv_obj_center(label);
  return label;
}

lv_obj_t* makeBar(lv_obj_t* parent, int w, int h, uint32_t fill) {
  lv_obj_t* bar = lv_bar_create(parent);
  lv_obj_remove_style_all(bar);
  lv_obj_set_size(bar, w, h);
  lv_obj_set_style_bg_color(bar, lv_color_hex(0x111819), 0);
  lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(bar, 1, 0);
  lv_obj_set_style_border_color(bar, lv_color_hex(fill), 0);
  lv_obj_set_style_radius(bar, 0, 0);
  lv_obj_set_style_bg_color(bar, lv_color_hex(fill), LV_PART_INDICATOR);
  lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);
  lv_obj_set_style_radius(bar, 0, LV_PART_INDICATOR);
  lv_bar_set_range(bar, 0, 100);
  lv_bar_set_value(bar, 0, LV_ANIM_OFF);
  return bar;
}

void initBrandIcon(BrandIcon& b) {
  if (b.buf) return;
  b.buf = static_cast<uint8_t*>(
      heap_caps_malloc(kBrandBufBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  b.dsc.header.magic = LV_IMAGE_HEADER_MAGIC;
  b.dsc.header.cf = LV_COLOR_FORMAT_RAW_ALPHA;
  b.dsc.header.w = kBrandIconPx;
  b.dsc.header.h = kBrandIconPx;
  b.dsc.data = b.buf;
  b.dsc.data_size = 0;
}

void applyBrandIcon(BrandIcon& b, lv_obj_t* img,
                    const uint8_t* png, size_t len) {
  if (!img || !b.buf) return;
  if (len > kBrandBufBytes) return;
  if (len == 0) {
    lv_image_set_src(img, nullptr);
    b.len = 0;
    return;
  }
  memcpy(b.buf, png, len);
  b.len = len;
  b.dsc.data = b.buf;
  b.dsc.data_size = len;
  lv_image_set_src(img, nullptr);
  lv_image_set_src(img, &b.dsc);
}

void updateStatus() {
  if (g_status.wifi) {
    lv_label_set_text(g_status.wifi, m5io::wifiConnected() ? LV_SYMBOL_WIFI : "");
    lv_obj_set_style_text_color(g_status.wifi, lv_color_hex(kGreen), 0);
  }
  if (g_status.battery) {
    const m5io::PowerState power = m5io::powerState();
    if (power == m5io::PowerState::BatteryCable ||
        power == m5io::PowerState::BatteryOnly) {
      const int pct = m5io::batteryPct();
      const char* icon = pct >= 85 ? LV_SYMBOL_BATTERY_FULL
                       : pct >= 60 ? LV_SYMBOL_BATTERY_3
                       : pct >= 35 ? LV_SYMBOL_BATTERY_2
                       : pct >= 12 ? LV_SYMBOL_BATTERY_1
                                   : LV_SYMBOL_BATTERY_EMPTY;
      char text[32];
      snprintf(text, sizeof(text), "%s %d%%",
               power == m5io::PowerState::BatteryCable ? LV_SYMBOL_CHARGE : icon,
               pct);
      lv_label_set_text(g_status.battery, text);
    } else if (power == m5io::PowerState::NoBatteryCable) {
      lv_label_set_text(g_status.battery, LV_SYMBOL_USB);
    } else {
      lv_label_set_text(g_status.battery, "");
    }
  }
}

void statusTimerCb(lv_timer_t*) {
  updateStatus();
}

void buildHeader(lv_obj_t* scr) {
  lv_obj_t* header = makePanel(scr, kHeaderX, kHeaderY, kHeaderW, kHeaderH, "", kDim);
  lv_obj_set_style_border_color(header, lv_color_hex(0x3B4544), 0);

  lv_obj_t* prompt = makeLabel(header, "user@m5stack-tab5:~$ vibehub",
                               &lv_font_montserrat_20, kText);
  lv_obj_set_size(prompt, 430, 28);
  lv_obj_align(prompt, LV_ALIGN_TOP_LEFT, 22, 12);

  // Hero "DASHBOARD" — 4× Tamzen Bold (40 px advance, ~72 px tall).
  const char* logo_text = "DASHBOARD";
  const uint32_t colors[] = {
      kYellow, kOrange, kGreen, kCyan, kBlue, kPurple, kPink, kOrange, kGreen,
  };
  constexpr int kLetterW = 40;     // Tamzen 80b advance width
  int logo_x = 24;
  for (int i = 0; i < 9; ++i) {
    char ch[2] = { logo_text[i], '\0' };
    lv_obj_t* letter = makeLabel(header, ch, &lv_font_tamzen_80b, colors[i]);
    lv_obj_set_size(letter, kLetterW, 80);
    lv_obj_set_pos(letter, logo_x, 44);
    logo_x += kLetterW;
  }

  g_status.wifi = makeLabel(header, "", &lv_font_montserrat_24, kGreen,
                            LV_TEXT_ALIGN_CENTER);
  lv_obj_set_size(g_status.wifi, 36, 30);
  lv_obj_align(g_status.wifi, LV_ALIGN_RIGHT_MID, -26, 0);

  g_status.battery = makeLabel(header, "", &lv_font_montserrat_18, kGreen,
                               LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(g_status.battery, 96, 26);
  lv_obj_align(g_status.battery, LV_ALIGN_RIGHT_MID, -72, 0);
}

void buildWeather(lv_obj_t* scr) {
  lv_obj_t* panel = makePanel(scr, kCardsX, kCardsY, kLeftColW, kCardsH, "", kCyan);

  if (!g_weather_icon_buf) {
    g_weather_icon_buf = static_cast<uint8_t*>(
        heap_caps_malloc(kIconBufBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    g_weather_icon_dsc.header.magic = LV_IMAGE_HEADER_MAGIC;
    g_weather_icon_dsc.header.cf = LV_COLOR_FORMAT_RAW_ALPHA;
    g_weather_icon_dsc.header.w = kWeatherIconPx;
    g_weather_icon_dsc.header.h = kWeatherIconPx;
    g_weather_icon_dsc.data = g_weather_icon_buf;
    g_weather_icon_dsc.data_size = 0;
  }

  // ── Top half: hero on left (icon + city/temp side-by-side), stats on right.
  constexpr int kTopH      = 280;
  constexpr int kIconX     = 30;
  constexpr int kIconY     = 30;
  const     int statsX     = kLeftColW * 55 / 100;
  // city/temp stacked vertically and centered against the icon's vertical span.
  constexpr int kCityH     = 36;
  constexpr int kTempH     = 60;
  constexpr int kStackGap  = 8;
  constexpr int kStackH    = kCityH + kStackGap + kTempH;
  constexpr int kHeroTextX = kIconX + kWeatherIconPx + 18;
  constexpr int kStackY    = kIconY + (kWeatherIconPx - kStackH) / 2;

  g_wx.icon = lv_image_create(panel);
  lv_obj_set_pos(g_wx.icon, kIconX, kIconY);
  lv_obj_set_size(g_wx.icon, kWeatherIconPx, kWeatherIconPx);

  g_wx.city = makeLabel(panel, "-", &lv_font_montserrat_28, kText);
  lv_obj_set_size(g_wx.city, statsX - kHeroTextX - 8, kCityH);
  lv_obj_set_pos(g_wx.city, kHeroTextX, kStackY);

  g_wx.temp = makeLabel(panel, "--" "\xC2\xB0" "C", &lv_font_montserrat_48, kCyan);
  lv_obj_set_size(g_wx.temp, statsX - kHeroTextX - 8, kTempH);
  lv_obj_set_pos(g_wx.temp, kHeroTextX, kStackY + kCityH + kStackGap);

  // Stats column (range / aqi / wind / humidity), evenly spaced.
  const int kStatsW    = kLeftColW - statsX - 24;
  const int kStatsRowH = 50;
  const int kStatsTop  = 40;
  g_wx.range    = makeLabel(panel, "range  -",    &lv_font_montserrat_24, kText);
  lv_obj_set_size(g_wx.range, kStatsW, kStatsRowH);
  lv_obj_set_pos(g_wx.range, statsX, kStatsTop);
  g_wx.aqi      = makeLabel(panel, "AQI  -",      &lv_font_montserrat_24, kYellow);
  lv_obj_set_size(g_wx.aqi, kStatsW, kStatsRowH);
  lv_obj_set_pos(g_wx.aqi, statsX, kStatsTop + kStatsRowH);
  g_wx.wind     = makeLabel(panel, "wind  -",     &lv_font_montserrat_24, kText);
  lv_obj_set_size(g_wx.wind, kStatsW, kStatsRowH);
  lv_obj_set_pos(g_wx.wind, statsX, kStatsTop + kStatsRowH * 2);
  g_wx.humidity = makeLabel(panel, "humidity  -", &lv_font_montserrat_24, kText);
  lv_obj_set_size(g_wx.humidity, kStatsW, kStatsRowH);
  lv_obj_set_pos(g_wx.humidity, statsX, kStatsTop + kStatsRowH * 3);

  g_wx.condition = makeLabel(panel, "waiting", &lv_font_montserrat_22, kMuted,
                             LV_TEXT_ALIGN_CENTER);
  lv_obj_set_size(g_wx.condition, kLeftColW - 48, 28);
  lv_obj_set_pos(g_wx.condition, 24, kTopH);

  // Divider between top and bottom halves.
  lv_obj_t* divider = lv_obj_create(panel);
  lv_obj_remove_style_all(divider);
  lv_obj_set_pos(divider, 24, kTopH + 36);
  lv_obj_set_size(divider, kLeftColW - 48, 1);
  lv_obj_set_style_bg_color(divider, lv_color_hex(kGrid), 0);
  lv_obj_set_style_bg_opa(divider, LV_OPA_COVER, 0);

  // ── Bottom half: 5-day forecast — centered horizontally and vertically.
  constexpr int kFcDayH   = 28;
  constexpr int kFcGap1   = 6;
  constexpr int kFcGap2   = 8;
  constexpr int kFcTempH  = 32;
  constexpr int kFcBlockH = kFcDayH + kFcGap1 + kForecastIconPx + kFcGap2 + kFcTempH;

  // Available bottom region runs from just below the divider to the panel bottom.
  const int kBottomTop    = kTopH + 36;          // y of the divider
  const int kBottomH      = kCardsH - kBottomTop;
  const int kFcAreaY      = kBottomTop + (kBottomH - kFcBlockH) / 2;

  // Five evenly-sized columns, each ~144 px wide. Total block width spans
  // 5 * kFcColW; center it inside (kLeftColW) by offsetting from the left edge.
  constexpr int kFcColW   = 144;
  const int kFcStripX     = (kLeftColW - 5 * kFcColW) / 2;

  const int kFcDayY  = kFcAreaY;
  const int kFcIconY = kFcDayY  + kFcDayH  + kFcGap1;
  const int kFcTempY = kFcIconY + kForecastIconPx + kFcGap2;

  for (int i = 0; i < 5; ++i) {
    const int x = kFcStripX + i * kFcColW;

    g_wx.forecast_day[i] = makeLabel(panel, "—", &lv_font_montserrat_22, kText,
                                     LV_TEXT_ALIGN_CENTER);
    lv_obj_set_size(g_wx.forecast_day[i], kFcColW, 28);
    lv_obj_set_pos(g_wx.forecast_day[i], x, kFcDayY);

    g_wx.forecast_icon[i] = lv_image_create(panel);
    lv_obj_set_size(g_wx.forecast_icon[i], kForecastIconPx, kForecastIconPx);
    lv_obj_set_pos(g_wx.forecast_icon[i],
                   x + (kFcColW - kForecastIconPx) / 2, kFcIconY);

    g_wx.forecast_temp[i] = makeLabel(panel, "-/-", &lv_font_montserrat_22, kCyan,
                                      LV_TEXT_ALIGN_CENTER);
    lv_obj_set_size(g_wx.forecast_temp[i], kFcColW, 32);
    lv_obj_set_pos(g_wx.forecast_temp[i], x, kFcTempY);

    if (!g_forecast_icon[i].buf) {
      g_forecast_icon[i].buf = static_cast<uint8_t*>(
          heap_caps_malloc(kForecastIconBufBytes,
                           MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
      g_forecast_icon[i].dsc.header.magic = LV_IMAGE_HEADER_MAGIC;
      g_forecast_icon[i].dsc.header.cf    = LV_COLOR_FORMAT_RAW_ALPHA;
      g_forecast_icon[i].dsc.header.w     = kForecastIconPx;
      g_forecast_icon[i].dsc.header.h     = kForecastIconPx;
      g_forecast_icon[i].dsc.data         = g_forecast_icon[i].buf;
      g_forecast_icon[i].dsc.data_size    = 0;
    }
  }
}

void buildUsage(lv_obj_t* scr, int x, int y, int w, int h, const char* title,
                uint32_t accent, UsageHandles* handles) {
  lv_obj_t* panel = makePanel(scr, x, y, w, h, title, accent);

  handles->icon = lv_image_create(panel);
  lv_obj_set_pos(handles->icon, 18, 24);
  lv_obj_set_size(handles->icon, kBrandIconPx, kBrandIconPx);

  handles->user = makeLabel(panel, "-", &lv_font_montserrat_22, kText);
  lv_obj_set_size(handles->user, w - 154, 34);
  lv_obj_set_pos(handles->user, 62, 24);

  handles->plan_label = makePlanPill(panel, accent);
  lv_obj_align(lv_obj_get_parent(handles->plan_label), LV_ALIGN_TOP_RIGHT, -16, 24);

  lv_obj_t* session = makeLabel(panel, "Session", &lv_font_montserrat_16, kMuted);
  lv_obj_set_size(session, 100, 22);
  lv_obj_set_pos(session, 16, 82);
  handles->session_reset = makeLabel(panel, "-", &lv_font_montserrat_14, kMuted,
                                     LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(handles->session_reset, 160, 22);
  lv_obj_align(handles->session_reset, LV_ALIGN_TOP_RIGHT, -16, 84);
  handles->session_bar = makeBar(panel, w - 86, 16, accent);
  lv_obj_set_pos(handles->session_bar, 16, 110);
  handles->session_pct = makeLabel(panel, "-%", &lv_font_montserrat_16, accent,
                                   LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(handles->session_pct, 54, 22);
  lv_obj_align(handles->session_pct, LV_ALIGN_TOP_RIGHT, -16, 107);

  lv_obj_t* weekly = makeLabel(panel, "Weekly", &lv_font_montserrat_16, kMuted);
  lv_obj_set_size(weekly, 100, 22);
  lv_obj_set_pos(weekly, 16, 148);
  handles->weekly_reset = makeLabel(panel, "-", &lv_font_montserrat_14, kMuted,
                                    LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(handles->weekly_reset, 160, 22);
  lv_obj_align(handles->weekly_reset, LV_ALIGN_TOP_RIGHT, -16, 150);
  handles->weekly_bar = makeBar(panel, w - 86, 16, accent);
  lv_obj_set_pos(handles->weekly_bar, 16, 176);
  handles->weekly_pct = makeLabel(panel, "-%", &lv_font_montserrat_16, accent,
                                  LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(handles->weekly_pct, 54, 22);
  lv_obj_align(handles->weekly_pct, LV_ALIGN_TOP_RIGHT, -16, 173);

  handles->extra = makeLabel(panel, "extra usage: -", &lv_font_montserrat_14, kDim);
  lv_obj_set_size(handles->extra, w - 32, 22);
  lv_obj_align(handles->extra, LV_ALIGN_BOTTOM_LEFT, 16, -2);
}

void applyUsage(const UsageHandles& h, const data::ClaudeData& d) {
  if (!d.valid || !h.user) return;
  updateStatus();

  const char* email = d.emailMasked[0] ? d.emailMasked : d.email;
  lv_label_set_text(h.user, email);
  lv_label_set_text(h.plan_label, d.plan[0] ? d.plan : "-");
  lv_obj_update_layout(h.plan_label);
  lv_obj_center(h.plan_label);
  lv_obj_update_layout(lv_obj_get_parent(h.plan_label));
  lv_obj_align(lv_obj_get_parent(h.plan_label), LV_ALIGN_TOP_RIGHT, -16, 24);

  if (d.session.present) {
    int pct = clampPct(d.session.utilizationPct);
    lv_bar_set_value(h.session_bar, pct, LV_ANIM_OFF);
    char p[12];
    snprintf(p, sizeof(p), "%d%%", pct);
    lv_label_set_text(h.session_pct, p);
    lv_label_set_text(h.session_reset, d.session.resetIn);
  } else {
    lv_bar_set_value(h.session_bar, 0, LV_ANIM_OFF);
    lv_label_set_text(h.session_pct, "-");
    lv_label_set_text(h.session_reset, "-");
  }

  if (d.weekly.present) {
    int pct = clampPct(d.weekly.utilizationPct);
    lv_bar_set_value(h.weekly_bar, pct, LV_ANIM_OFF);
    char p[12];
    snprintf(p, sizeof(p), "%d%%", pct);
    lv_label_set_text(h.weekly_pct, p);
    lv_label_set_text(h.weekly_reset, d.weekly.resetIn);
  } else {
    lv_bar_set_value(h.weekly_bar, 0, LV_ANIM_OFF);
    lv_label_set_text(h.weekly_pct, "-");
    lv_label_set_text(h.weekly_reset, "-");
  }

  lv_label_set_text(h.extra, d.extraEnabled ? "extra: enabled" : "extra: disabled");
  lv_obj_set_style_text_color(h.extra,
      lv_color_hex(d.extraEnabled ? kGreen : kDim), 0);
}

void destroyGridScreen() {
  if (g_status_timer) {
    lv_timer_del(g_status_timer);
    g_status_timer = nullptr;
  }
  g_status = {};
  g_wx = {};
  g_claude = {};
  g_codex = {};
}

void buildGridScreen() {
  lv_obj_t* scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  buildHeader(scr);
  buildWeather(scr);
  buildUsage(scr, kRightColX, kCardsY,
             kRightColW, kRightCardH, "", kOrange, &g_claude);
  buildUsage(scr, kRightColX, kCardsY + kRightCardH + kColGap,
             kRightColW, kRightCardH, "", kPurple, &g_codex);

  initBrandIcon(g_brand_claude);
  initBrandIcon(g_brand_codex);
  updateStatus();
  g_status_timer = lv_timer_create(statusTimerCb, 2000, nullptr);
}

void updateWeatherCard(const data::WeatherData& d) {
  if (!d.valid || !g_wx.city) return;
  updateStatus();

  lv_label_set_text(g_wx.city, d.city);

  char buf[64];
  snprintf(buf, sizeof(buf), "%d" "\xC2\xB0" "C", (int)d.tempC);
  lv_label_set_text(g_wx.temp, buf);
  lv_label_set_text(g_wx.condition, d.condition);

  snprintf(buf, sizeof(buf), "range  %d / %d" "\xC2\xB0" "C", (int)d.highC, (int)d.lowC);
  lv_label_set_text(g_wx.range, buf);

  snprintf(buf, sizeof(buf), "wind  %d km/h", (int)d.windKmh);
  lv_label_set_text(g_wx.wind, buf);

  snprintf(buf, sizeof(buf), "humidity  %d %%", (int)d.humidityPct);
  lv_label_set_text(g_wx.humidity, buf);

  if (d.air.present) {
    snprintf(buf, sizeof(buf), "AQI  %u  %s", (unsigned)d.air.aqi, d.air.category);
    lv_label_set_text(g_wx.aqi, buf);
    uint32_t hex = ((uint32_t)d.air.r << 16) |
                   ((uint32_t)d.air.g << 8) |
                   (uint32_t)d.air.b;
    lv_obj_set_style_text_color(g_wx.aqi, lv_color_hex(hex), 0);
  } else {
    lv_label_set_text(g_wx.aqi, "AQI  -");
    lv_obj_set_style_text_color(g_wx.aqi, lv_color_hex(kYellow), 0);
  }

  for (int i = 0; i < 5; ++i) {
    if (g_wx.forecast_day[i])  lv_label_set_text(g_wx.forecast_day[i], d.forecast[i].day);
    if (g_wx.forecast_temp[i]) {
      char fc[24];
      snprintf(fc, sizeof(fc), "%d / %d", (int)d.forecast[i].highC, (int)d.forecast[i].lowC);
      lv_label_set_text(g_wx.forecast_temp[i], fc);
    }
  }
}

void setForecastIconPng(int idx, const uint8_t* png, size_t len) {
  if (idx < 0 || idx >= 5) return;
  if (!g_wx.forecast_icon[idx] || !g_forecast_icon[idx].buf) return;
  if (len > kForecastIconBufBytes) return;
  if (len == 0) {
    lv_image_set_src(g_wx.forecast_icon[idx], nullptr);
    g_forecast_icon[idx].len = 0;
    return;
  }
  memcpy(g_forecast_icon[idx].buf, png, len);
  g_forecast_icon[idx].len = len;
  g_forecast_icon[idx].dsc.data      = g_forecast_icon[idx].buf;
  g_forecast_icon[idx].dsc.data_size = len;
  lv_image_set_src(g_wx.forecast_icon[idx], nullptr);
  lv_image_set_src(g_wx.forecast_icon[idx], &g_forecast_icon[idx].dsc);
}

void updateClaudeCard(const data::ClaudeData& d) { applyUsage(g_claude, d); }
void updateCodexCard(const data::CodexData& d) { applyUsage(g_codex, d); }

void setWeatherIconPng(const uint8_t* png, size_t len) {
  if (!g_wx.icon || !g_weather_icon_buf) return;
  if (len > kIconBufBytes) return;
  if (len == 0) {
    lv_image_set_src(g_wx.icon, nullptr);
    g_weather_icon_len = 0;
    return;
  }

  memcpy(g_weather_icon_buf, png, len);
  g_weather_icon_len = len;
  g_weather_icon_dsc.data = g_weather_icon_buf;
  g_weather_icon_dsc.data_size = len;
  lv_image_set_src(g_wx.icon, nullptr);
  lv_image_set_src(g_wx.icon, &g_weather_icon_dsc);
}

void setClaudeIconPng(const uint8_t* png, size_t len) {
  applyBrandIcon(g_brand_claude, g_claude.icon, png, len);
}

void setCodexIconPng(const uint8_t* png, size_t len) {
  applyBrandIcon(g_brand_codex, g_codex.icon, png, len);
}

}  // namespace

const Layout kGridLayout = {
  "grid",
  buildGridScreen,
  destroyGridScreen,
  updateWeatherCard,
  updateClaudeCard,
  updateCodexCard,
  nullptr,                  // grid no longer renders news
  setWeatherIconPng,
  setClaudeIconPng,
  setCodexIconPng,
  setForecastIconPng,
  { kWeatherIconPx, kBrandIconPx, kForecastIconPx },
};

}  // namespace ui
