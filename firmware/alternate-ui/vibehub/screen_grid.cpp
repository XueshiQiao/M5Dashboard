// screen_grid.cpp - VIBEHUB terminal cockpit alternate UI.
//
// Pure LVGL plus the m5_io facade. This file deliberately avoids M5Unified
// and M5GFX includes so their bundled mini-LVGL types do not collide with
// the real LVGL headers.

#include "screen_grid.h"

#include "m5_io.h"

#include <esp_heap_caps.h>
#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

struct StatusHandles {
  lv_obj_t* wifi;
  lv_obj_t* battery;
};

struct WeatherHandles {
  lv_obj_t* city;
  lv_obj_t* temp;
  lv_obj_t* condition;
  lv_obj_t* details;
  lv_obj_t* aqi;
  lv_obj_t* icon;
  lv_obj_t* forecast[5];
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

struct NewsHandles {
  lv_obj_t* count;
  lv_obj_t* items[5];
};

StatusHandles  g_status = {};
WeatherHandles g_wx = {};
UsageHandles   g_claude = {};
UsageHandles   g_codex = {};
NewsHandles    g_news = {};
constexpr int kNewsRows = 5;

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
  lv_obj_t* header = makePanel(scr, 14, 12, 1252, 94, "", kDim);
  lv_obj_set_style_border_color(header, lv_color_hex(0x3B4544), 0);

  lv_obj_t* prompt = makeLabel(header, "user@m5stack-tab5:~$ vibehub",
                               &lv_font_montserrat_20, kText);
  lv_obj_set_size(prompt, 430, 28);
  lv_obj_align(prompt, LV_ALIGN_TOP_LEFT, 22, 16);

  const char* logo_text = "DASHBOARD";
  const uint32_t colors[] = {
      kYellow, kOrange, kGreen, kCyan, kBlue, kPurple, kPink, kOrange, kGreen,
  };
  int logo_x = 24;
  for (int i = 0; i < 9; ++i) {
    char ch[2] = { logo_text[i], '\0' };
    lv_obj_t* letter = makeLabel(header, ch, &lv_font_montserrat_40, colors[i]);
    lv_obj_set_size(letter, i == 5 ? 44 : 38, 48);
    lv_obj_set_pos(letter, logo_x, 42);
    logo_x += i == 5 ? 44 : 38;
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
  lv_obj_t* panel = makePanel(scr, 14, 120, 458, 246, "", kCyan);

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

  g_wx.city = makeLabel(panel, "-", &lv_font_montserrat_22, kText);
  lv_obj_set_size(g_wx.city, 220, 30);
  lv_obj_set_pos(g_wx.city, 22, 18);

  g_wx.icon = lv_image_create(panel);
  lv_obj_set_pos(g_wx.icon, 24, 78);
  lv_obj_set_size(g_wx.icon, kWeatherIconPx, kWeatherIconPx);

  g_wx.temp = makeLabel(panel, "--" "\xC2\xB0" "C", &lv_font_montserrat_48, kCyan);
  lv_obj_set_size(g_wx.temp, 160, 56);
  lv_obj_set_pos(g_wx.temp, 154, 62);

  g_wx.condition = makeLabel(panel, "waiting", &lv_font_montserrat_18, kText);
  lv_obj_set_size(g_wx.condition, 176, 26);
  lv_obj_set_pos(g_wx.condition, 158, 122);

  g_wx.details = makeLabel(panel, "feels: -\nhumidity: -\nwind: -",
                           &lv_font_montserrat_16, kMuted);
  lv_obj_set_size(g_wx.details, 150, 86);
  lv_obj_set_pos(g_wx.details, 300, 68);

  g_wx.aqi = makeLabel(panel, "AQI -", &lv_font_montserrat_16, kYellow);
  lv_obj_set_size(g_wx.aqi, 150, 24);
  lv_obj_set_pos(g_wx.aqi, 300, 158);

  int col_w = 86;
  for (int i = 0; i < 5; ++i) {
    g_wx.forecast[i] = makeLabel(panel, "--  -/-", &lv_font_montserrat_14, kText,
                                 LV_TEXT_ALIGN_CENTER);
    lv_obj_set_size(g_wx.forecast[i], 78, 38);
    lv_obj_set_pos(g_wx.forecast[i], 18 + i * col_w, 200);
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

void buildNews(lv_obj_t* scr) {
  lv_obj_t* panel = makePanel(scr, 14, 378, 760, 260, "[ VIBECODING NEWS ]", kGreen);

  g_news.count = makeLabel(panel, "0 items", &lv_font_montserrat_16, kMuted);
  lv_obj_set_size(g_news.count, 120, 22);
  lv_obj_align(g_news.count, LV_ALIGN_TOP_RIGHT, -16, 15);

  for (int i = 0; i < kNewsRows; ++i) {
    char timebox[16];
    snprintf(timebox, sizeof(timebox), "%dh ago", 2 + i * 3);
    lv_obj_t* badge = makeLabel(panel, timebox, &lv_font_montserrat_14,
                                i == 0 ? kGreen : (i == 1 ? kBlue : (i == 2 ? kPurple : kOrange)),
                                LV_TEXT_ALIGN_CENTER);
    lv_obj_set_size(badge, 72, 22);
    lv_obj_set_pos(badge, 18, 58 + i * 34);

    g_news.items[i] = makeLabel(panel, "> waiting for server",
                                &lv_font_montserrat_16, kText);
    lv_obj_set_size(g_news.items[i], 620, 28);
    lv_obj_set_pos(g_news.items[i], 104, 55 + i * 34);
  }

  static const char* art =
      "  [bot]\n"
      " <o  o>\n"
      " /|__|\\";
  lv_obj_t* bot = makeLabel(panel, art, &lv_font_montserrat_14, kCyan,
                            LV_TEXT_ALIGN_CENTER);
  lv_obj_set_size(bot, 92, 76);
  lv_obj_align(bot, LV_ALIGN_BOTTOM_RIGHT, -28, -12);
}

void buildTodos(lv_obj_t* scr) {
  lv_obj_t* panel = makePanel(scr, 786, 378, 480, 260, "[ TODOS ]", kPink);

  static const char* todos =
      "[x] Setup Claude Code CLI\n"
      "[x] Wire weather endpoint\n"
      "[x] Add Codex usage polling\n"
      "[ ] Add Redis cache for usage data\n"
      "[ ] Integrate Notion API for tasks";
  lv_obj_t* body = makeLabel(panel, todos, &lv_font_montserrat_18, kText);
  lv_obj_set_size(body, 430, 142);
  lv_obj_set_pos(body, 24, 62);

  lv_obj_t* progress = makeBar(panel, 300, 18, kPink);
  lv_obj_set_pos(progress, 24, 216);
  lv_bar_set_value(progress, 60, LV_ANIM_OFF);

  lv_obj_t* label = makeLabel(panel, "3/5 tasks completed", &lv_font_montserrat_16, kPink);
  lv_obj_set_size(label, 210, 22);
  lv_obj_set_pos(label, 24, 194);

  lv_obj_t* pct = makeLabel(panel, "60%", &lv_font_montserrat_18, kPink,
                            LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(pct, 70, 24);
  lv_obj_set_pos(pct, 340, 213);
}

void buildFooter(lv_obj_t* scr) {
  lv_obj_t* footer = makePanel(scr, 14, 652, 1252, 54, "", kDim);
  lv_obj_set_style_border_color(footer, lv_color_hex(0x3B4544), 0);
  lv_obj_t* tip = makeLabel(footer, "* Tips: Connect. Create. Automate. Vibe. Repeat.",
                            &lv_font_montserrat_18, kText);
  lv_obj_set_size(tip, 700, 26);
  lv_obj_align(tip, LV_ALIGN_LEFT_MID, 22, 0);

  lv_obj_t* version = makeLabel(footer, "v1.0.0", &lv_font_montserrat_18, kMuted,
                                LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_size(version, 100, 26);
  lv_obj_align(version, LV_ALIGN_RIGHT_MID, -22, 0);
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

}  // namespace

void buildGridScreen() {
  lv_obj_t* scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  buildHeader(scr);
  buildWeather(scr);
  buildUsage(scr, 484, 120, 384, 246, "", kOrange, &g_claude);
  buildUsage(scr, 880, 120, 386, 246, "", kPurple, &g_codex);
  buildNews(scr);
  buildTodos(scr);
  buildFooter(scr);

  initBrandIcon(g_brand_claude);
  initBrandIcon(g_brand_codex);
  updateStatus();
  if (!g_status_timer) {
    g_status_timer = lv_timer_create(statusTimerCb, 2000, nullptr);
  }
}

void updateWeatherCard(const data::WeatherData& d) {
  if (!d.valid || !g_wx.city) return;
  updateStatus();

  char city[64];
  snprintf(city, sizeof(city), "%s", d.city);
  lv_label_set_text(g_wx.city, city);

  char temp[16];
  snprintf(temp, sizeof(temp), "%d" "\xC2\xB0" "C", (int)d.tempC);
  lv_label_set_text(g_wx.temp, temp);
  lv_label_set_text(g_wx.condition, d.condition);

  char details[96];
  snprintf(details, sizeof(details),
           "range: %d/%d" "\xC2\xB0" "C\nhumidity: %d%%\nwind: %d km/h",
           (int)d.highC, (int)d.lowC,
           (int)d.humidityPct, (int)d.windKmh);
  lv_label_set_text(g_wx.details, details);

  if (d.air.present) {
    char aqi[54];
    snprintf(aqi, sizeof(aqi), "AQI %u  %s",
             (unsigned)d.air.aqi, d.air.category);
    lv_label_set_text(g_wx.aqi, aqi);
    uint32_t hex = ((uint32_t)d.air.r << 16) |
                   ((uint32_t)d.air.g << 8) |
                   (uint32_t)d.air.b;
    lv_obj_set_style_text_color(g_wx.aqi, lv_color_hex(hex), 0);
  } else {
    lv_label_set_text(g_wx.aqi, "AQI -");
    lv_obj_set_style_text_color(g_wx.aqi, lv_color_hex(kYellow), 0);
  }

  for (int i = 0; i < 5; ++i) {
    char fc[36];
    snprintf(fc, sizeof(fc), "%s\n%d/%d %s",
             d.forecast[i].day,
             (int)d.forecast[i].highC,
             (int)d.forecast[i].lowC,
             d.forecast[i].glyph);
    lv_label_set_text(g_wx.forecast[i], fc);
  }
}

void updateClaudeCard(const data::ClaudeData& d) { applyUsage(g_claude, d); }
void updateCodexCard(const data::CodexData& d) { applyUsage(g_codex, d); }

void updateInboxCard(const data::NewsData& d) {
  if (!d.valid || !g_news.count) return;
  updateStatus();

  char count[24];
  snprintf(count, sizeof(count), "%u items", (unsigned)d.count);
  lv_label_set_text(g_news.count, count);

  for (int i = 0; i < kNewsRows; ++i) {
    if (i < d.count) {
      char line[120];
      snprintf(line, sizeof(line), "> %s", d.items[i].title);
      lv_label_set_text(g_news.items[i], line);
    } else {
      lv_label_set_text(g_news.items[i], "> -");
    }
  }
}

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

}  // namespace ui
