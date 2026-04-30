// screen_grid.cpp - Plan A (Glance Grid) built with LVGL widgets.
//
// Pure LVGL: this translation unit deliberately does NOT include M5Unified
// (M5GFX bundles its own mini-LVGL types whose guards differ from LVGL's,
// so mixing them in one .cpp causes "conflicting declaration" errors).

#include "screen_grid.h"

#include <esp_heap_caps.h>
#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

struct WeatherHandles {
  lv_obj_t* city;
  lv_obj_t* temp;
  lv_obj_t* cond;
  lv_obj_t* details;
  lv_obj_t* icon;
  lv_obj_t* aqi;
  lv_obj_t* fc_day[5];
  lv_obj_t* fc_glyph[5];
  lv_obj_t* fc_hilo[5];
};

constexpr size_t kIconBufBytes = 16 * 1024;
uint8_t* g_icon_buf = nullptr;
size_t g_icon_len = 0;
lv_image_dsc_t g_icon_dsc = {};

struct UsageHandles {
  lv_obj_t* icon;          // lv_image (Claude / Codex brand logo)
  lv_obj_t* email;
  lv_obj_t* active_pill;   // re-aligned to the email's right edge after each update
  lv_obj_t* plan_pill;
  lv_obj_t* plan_label;
  lv_obj_t* session_reset;
  lv_obj_t* session_bar;
  lv_obj_t* session_pct;
  lv_obj_t* weekly_reset;
  lv_obj_t* weekly_bar;
  lv_obj_t* weekly_pct;
  lv_obj_t* extra_on;
};

// Brand icon storage — one PSRAM buffer + descriptor per card. PNGs are
// ~1.5–2 KB so 8 KB is plenty of headroom; bytes must outlive the
// widget so lodepng can re-decode if the cache evicts.
constexpr size_t kBrandBufBytes = 8 * 1024;
struct BrandIcon {
  uint8_t*       buf = nullptr;
  size_t         len = 0;
  lv_image_dsc_t dsc = {};
};
BrandIcon g_brand_claude;
BrandIcon g_brand_codex;

void initBrandIcon(BrandIcon& b) {
  if (b.buf) return;
  b.buf = static_cast<uint8_t*>(
      heap_caps_malloc(kBrandBufBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  b.dsc.header.magic = LV_IMAGE_HEADER_MAGIC;
  b.dsc.header.cf    = LV_COLOR_FORMAT_RAW_ALPHA;
  b.dsc.header.w     = kBrandIconPx;
  b.dsc.header.h     = kBrandIconPx;
  b.dsc.data         = b.buf;
  b.dsc.data_size    = 0;
}

void applyBrandIcon(BrandIcon& b, lv_obj_t* img,
                    const uint8_t* png, size_t len) {
  if (!img || !b.buf) return;
  if (len > kBrandBufBytes) return;
  if (len == 0) { lv_image_set_src(img, nullptr); b.len = 0; return; }
  memcpy(b.buf, png, len);
  b.len = len;
  b.dsc.data = b.buf;
  b.dsc.data_size = len;
  lv_image_set_src(img, nullptr);
  lv_image_set_src(img, &b.dsc);
}

struct InboxHandles {
  lv_obj_t* count;
  lv_obj_t* items[3];
};

WeatherHandles g_wx = {};
UsageHandles g_claude = {};
UsageHandles g_codex = {};
InboxHandles g_inbox = {};

constexpr uint32_t kBg = 0x0A0E12;
constexpr uint32_t kCard = 0x14191F;
constexpr uint32_t kCardAlt = 0x181E26;
constexpr uint32_t kBorder = 0x2A3140;
constexpr uint32_t kDivider = 0x1F2630;

constexpr uint32_t kTextHi = 0xE6EDF3;
constexpr uint32_t kTextMd = 0x8B98AB;
constexpr uint32_t kTextLo = 0x5C6776;
constexpr uint32_t kWhite = 0xFFFFFF;

constexpr uint32_t kAccentWx = 0xF2C94C;
// Card accents pulled directly from the brand SVGs:
//   - Claude: the icon's single fill, #D97757 (a clay brown).
//   - Codex:  the deepest stop of the icon's blue gradient, #3941FF.
constexpr uint32_t kAccentClaude = 0xD97757;
constexpr uint32_t kAccentCodex  = 0x3941FF;
constexpr uint32_t kAccentInbox = 0xFF6B9D;

constexpr uint32_t kProgressTrack = 0x3A424E;
constexpr uint32_t kProgressFill = 0x4CE07C;

constexpr uint32_t kPillActiveBg = 0x4CC86A;
constexpr uint32_t kPillMaxBg = 0xE57F2D;
constexpr uint32_t kPillPlusBg = 0x10A37B;


lv_obj_t* makeCard(lv_obj_t* parent, int x, int y, int w, int h,
                   uint32_t accent_hex) {
  lv_obj_t* card = lv_obj_create(parent);
  lv_obj_remove_style_all(card);
  lv_obj_set_pos(card, x, y);
  lv_obj_set_size(card, w, h);
  lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(card, lv_color_hex(kCard), 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_radius(card, 14, 0);
  lv_obj_set_style_border_width(card, 1, 0);
  lv_obj_set_style_border_color(card, lv_color_hex(kBorder), 0);
  lv_obj_set_style_clip_corner(card, true, 0);

  lv_obj_t* stripe = lv_obj_create(card);
  lv_obj_remove_style_all(stripe);
  lv_obj_set_size(stripe, w, 3);
  lv_obj_set_pos(stripe, 0, 0);
  lv_obj_set_style_bg_color(stripe, lv_color_hex(accent_hex), 0);
  lv_obj_set_style_bg_opa(stripe, LV_OPA_COVER, 0);
  lv_obj_set_style_radius(stripe, 0, 0);

  return card;
}

lv_obj_t* makeLabel(lv_obj_t* parent, const char* text,
                    const lv_font_t* font, uint32_t color_hex) {
  lv_obj_t* label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, lv_color_hex(color_hex), 0);
  return label;
}

lv_obj_t* makePill(lv_obj_t* parent, const char* text,
                   uint32_t bg_hex, uint32_t fg_hex,
                   const lv_font_t* font) {
  lv_obj_t* pill = lv_obj_create(parent);
  lv_obj_remove_style_all(pill);
  lv_obj_set_size(pill, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_radius(pill, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_bg_color(pill, lv_color_hex(bg_hex), 0);
  lv_obj_set_style_bg_opa(pill, LV_OPA_COVER, 0);
  lv_obj_set_style_pad_hor(pill, 14, 0);
  lv_obj_set_style_pad_ver(pill, 5, 0);
  lv_obj_set_style_border_width(pill, 0, 0);
  lv_obj_set_scrollbar_mode(pill, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(pill, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* label = lv_label_create(pill);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, lv_color_hex(fg_hex), 0);
  lv_obj_center(label);
  return pill;
}

lv_obj_t* makeProgressBar(lv_obj_t* parent, int w, int h, int percent) {
  lv_obj_t* bar = lv_bar_create(parent);
  lv_obj_set_size(bar, w, h);
  lv_obj_set_style_bg_color(bar, lv_color_hex(kProgressTrack), 0);
  lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
  lv_obj_set_style_radius(bar, h / 2, 0);
  lv_obj_set_style_border_width(bar, 0, 0);
  lv_obj_set_style_bg_color(bar, lv_color_hex(kProgressFill), LV_PART_INDICATOR);
  lv_obj_set_style_radius(bar, h / 2, LV_PART_INDICATOR);
  lv_bar_set_range(bar, 0, 100);
  lv_bar_set_value(bar, percent, LV_ANIM_OFF);
  return bar;
}

void buildWeatherCard(lv_obj_t* parent, int x, int y, int w, int h) {
  lv_obj_t* card = makeCard(parent, x, y, w, h, kAccentWx);

  lv_obj_t* hdr = makeLabel(card, "WEATHER", &lv_font_montserrat_20, kAccentWx);
  lv_obj_align(hdr, LV_ALIGN_TOP_LEFT, 22, 18);

  g_wx.city = makeLabel(card, "-", &lv_font_montserrat_16, kTextLo);
  lv_obj_align(g_wx.city, LV_ALIGN_TOP_RIGHT, -22, 22);

  if (!g_icon_buf) {
    g_icon_buf = static_cast<uint8_t*>(
        heap_caps_malloc(kIconBufBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    g_icon_dsc.header.magic = LV_IMAGE_HEADER_MAGIC;
    g_icon_dsc.header.cf = LV_COLOR_FORMAT_RAW_ALPHA;
    g_icon_dsc.header.w = kWeatherIconPx;
    g_icon_dsc.header.h = kWeatherIconPx;
    g_icon_dsc.data = g_icon_buf;
    g_icon_dsc.data_size = 0;
  }
  g_wx.icon = lv_image_create(card);
  lv_obj_set_pos(g_wx.icon, 32, 72);
  lv_obj_set_size(g_wx.icon, kWeatherIconPx, kWeatherIconPx);

  g_wx.temp = makeLabel(card, "-", &lv_font_montserrat_48, kTextHi);
  lv_obj_align(g_wx.temp, LV_ALIGN_TOP_LEFT, 180, 70);

  g_wx.cond = makeLabel(card, "-", &lv_font_montserrat_32, kTextMd);
  lv_obj_align(g_wx.cond, LV_ALIGN_TOP_LEFT, 180, 140);

  g_wx.details = makeLabel(card, "-", &lv_font_montserrat_22, kTextLo);
  lv_obj_set_style_text_line_space(g_wx.details, 8, 0);
  lv_obj_align(g_wx.details, LV_ALIGN_TOP_LEFT, 380, 80);

  g_wx.aqi = makeLabel(card, "", &lv_font_montserrat_22, kTextHi);
  lv_obj_align(g_wx.aqi, LV_ALIGN_TOP_LEFT, 380, 188);

  lv_obj_t* strip = lv_obj_create(card);
  lv_obj_remove_style_all(strip);
  lv_obj_set_size(strip, w - 16, 80);
  lv_obj_align(strip, LV_ALIGN_BOTTOM_MID, 0, -6);
  lv_obj_set_style_bg_color(strip, lv_color_hex(kCardAlt), 0);
  lv_obj_set_style_bg_opa(strip, LV_OPA_COVER, 0);
  lv_obj_set_style_radius(strip, 8, 0);
  lv_obj_set_style_border_width(strip, 1, 0);
  lv_obj_set_style_border_color(strip, lv_color_hex(kBorder), 0);
  lv_obj_set_scrollbar_mode(strip, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(strip, LV_OBJ_FLAG_SCROLLABLE);

  int col_w = (w - 16) / 5;
  for (int i = 0; i < 5; ++i) {
    int cx = i * col_w + col_w / 2;
    g_wx.fc_day[i] = makeLabel(strip, "-", &lv_font_montserrat_18, kTextMd);
    lv_obj_set_pos(g_wx.fc_day[i], cx - 24, 4);

    g_wx.fc_glyph[i] = makeLabel(strip, "-", &lv_font_montserrat_16, kTextLo);
    lv_obj_set_pos(g_wx.fc_glyph[i], cx - 26, 30);

    g_wx.fc_hilo[i] = makeLabel(strip, "- / -", &lv_font_montserrat_18, kTextHi);
    lv_obj_set_pos(g_wx.fc_hilo[i], cx - 24, 56);
  }
}

struct UsageData {
  const char* email;
  const char* plan_label;
  uint32_t plan_bg;
  int session_pct;
  const char* session_reset;
  int weekly_pct;
  const char* weekly_reset;
  bool extra_enabled;
};

void buildUsageCard(lv_obj_t* parent, int x, int y, int w, int h,
                    uint32_t accent, const UsageData& u,
                    UsageHandles* out) {
  lv_obj_t* card = makeCard(parent, x, y, w, h, accent);

  // Brand icon (Claude / Codex). Empty until the first /api/icon/...
  // poll lands; the data path then calls setClaudeIconPng /
  // setCodexIconPng to swap the bytes in.
  lv_obj_t* icon = lv_image_create(card);
  lv_obj_set_pos(icon, 22, 18);
  lv_obj_set_size(icon, kBrandIconPx, kBrandIconPx);

  lv_obj_t* email = makeLabel(card, u.email, &lv_font_montserrat_22, kTextHi);
  lv_obj_align(email, LV_ALIGN_TOP_LEFT, 80, 30);

  lv_obj_t* active = makePill(card, "Active", kPillActiveBg, kWhite,
                              &lv_font_montserrat_16);
  lv_obj_align_to(active, email, LV_ALIGN_OUT_RIGHT_MID, 14, 0);

  lv_obj_t* plan = makePill(card, u.plan_label, u.plan_bg, kWhite,
                            &lv_font_montserrat_18);
  lv_obj_align(plan, LV_ALIGN_TOP_RIGHT, -22, 26);
  lv_obj_t* plan_label = lv_obj_get_child(plan, 0);

  int row1_y = 100;
  lv_obj_t* l1 = makeLabel(card, "Session", &lv_font_montserrat_20, kTextMd);
  lv_obj_align(l1, LV_ALIGN_TOP_LEFT, 22, row1_y);
  lv_obj_t* r1 = makeLabel(card, u.session_reset, &lv_font_montserrat_18, kTextMd);
  lv_obj_align(r1, LV_ALIGN_TOP_RIGHT, -22, row1_y + 2);
  lv_obj_t* b1 = makeProgressBar(card, w - 44, 12, u.session_pct);
  lv_obj_align(b1, LV_ALIGN_TOP_LEFT, 22, row1_y + 32);
  char pct1[8];
  snprintf(pct1, sizeof(pct1), "%d%%", u.session_pct);
  lv_obj_t* p1 = makeLabel(card, pct1, &lv_font_montserrat_18, kProgressFill);
  lv_obj_align(p1, LV_ALIGN_TOP_RIGHT, -22, row1_y + 52);

  int row2_y = 180;
  lv_obj_t* l2 = makeLabel(card, "Weekly", &lv_font_montserrat_20, kTextMd);
  lv_obj_align(l2, LV_ALIGN_TOP_LEFT, 22, row2_y);
  lv_obj_t* r2 = makeLabel(card, u.weekly_reset, &lv_font_montserrat_18, kTextMd);
  lv_obj_align(r2, LV_ALIGN_TOP_RIGHT, -22, row2_y + 2);
  lv_obj_t* b2 = makeProgressBar(card, w - 44, 12, u.weekly_pct);
  lv_obj_align(b2, LV_ALIGN_TOP_LEFT, 22, row2_y + 32);
  char pct2[8];
  snprintf(pct2, sizeof(pct2), "%d%%", u.weekly_pct);
  lv_obj_t* p2 = makeLabel(card, pct2, &lv_font_montserrat_18, kProgressFill);
  lv_obj_align(p2, LV_ALIGN_TOP_RIGHT, -22, row2_y + 52);

  lv_obj_t* fl = makeLabel(card, LV_SYMBOL_REFRESH "  Extra usage",
                           &lv_font_montserrat_18, kTextLo);
  lv_obj_align(fl, LV_ALIGN_BOTTOM_LEFT, 22, -16);
  lv_obj_t* fr = makeLabel(card, u.extra_enabled ? "On" : "Off",
                           &lv_font_montserrat_18,
                           u.extra_enabled ? kProgressFill : kTextLo);
  lv_obj_align(fr, LV_ALIGN_BOTTOM_RIGHT, -22, -16);

  if (out) {
    out->icon = icon;
    out->email = email;
    out->active_pill = active;
    out->plan_pill = plan;
    out->plan_label = plan_label;
    out->session_reset = r1;
    out->session_bar = b1;
    out->session_pct = p1;
    out->weekly_reset = r2;
    out->weekly_bar = b2;
    out->weekly_pct = p2;
    out->extra_on = fr;
  }
}

void buildInboxCard(lv_obj_t* parent, int x, int y, int w, int h) {
  lv_obj_t* card = makeCard(parent, x, y, w, h, kAccentInbox);

  lv_obj_t* hdr = makeLabel(card, "INBOX", &lv_font_montserrat_20, kAccentInbox);
  lv_obj_align(hdr, LV_ALIGN_TOP_LEFT, 22, 18);

  g_inbox.count = makeLabel(card, "3 NEW", &lv_font_montserrat_16, kTextLo);
  lv_obj_align(g_inbox.count, LV_ALIGN_TOP_RIGHT, -22, 22);

  static const char* kItems[3] = {
      "GPT-4o vision rollout",
      "Anthropic ships Sonnet 4.7",
      "Tab5 review pt 3 published",
  };
  int row_y = 70;
  for (int i = 0; i < 3; ++i) {
    lv_obj_t* chev = makeLabel(card, ">", &lv_font_montserrat_22, kAccentInbox);
    lv_obj_align(chev, LV_ALIGN_TOP_LEFT, 26, row_y);

    g_inbox.items[i] = makeLabel(card, kItems[i], &lv_font_montserrat_22, kTextHi);
    lv_obj_align(g_inbox.items[i], LV_ALIGN_TOP_LEFT, 56, row_y);

    if (i < 2) {
      lv_obj_t* sep = lv_obj_create(card);
      lv_obj_remove_style_all(sep);
      lv_obj_set_size(sep, w - 56, 1);
      lv_obj_set_pos(sep, 26, row_y + 36);
      lv_obj_set_style_bg_color(sep, lv_color_hex(kDivider), 0);
      lv_obj_set_style_bg_opa(sep, LV_OPA_COVER, 0);
      lv_obj_set_style_radius(sep, 0, 0);
    }
    row_y += 60;
  }

  lv_obj_t* hint = makeLabel(card, "tap any item to read",
                             &lv_font_montserrat_16, kTextLo);
  lv_obj_align(hint, LV_ALIGN_BOTTOM_RIGHT, -22, -16);
}

void applyUsage(const UsageHandles& h, const data::ClaudeData& d) {
  if (!d.valid || h.email == nullptr) return;

  lv_label_set_text(h.email, d.emailMasked[0] ? d.emailMasked : d.email);
  // The "Active" pill sits to the right of the email. lv_obj_align_to
  // is a one-shot computation against the current label size, so we
  // need to force a layout pass after the text changes and re-anchor
  // the pill — otherwise it overlaps the freshly-grown email label.
  if (h.active_pill) {
    lv_obj_update_layout(h.email);
    lv_obj_align_to(h.active_pill, h.email, LV_ALIGN_OUT_RIGHT_MID, 14, 0);
  }
  lv_label_set_text(h.plan_label, d.plan);

  if (d.session.present) {
    lv_label_set_text(h.session_reset, d.session.resetIn);
    lv_bar_set_value(h.session_bar, d.session.utilizationPct, LV_ANIM_OFF);
    char pct[8];
    snprintf(pct, sizeof(pct), "%d%%", (int)d.session.utilizationPct);
    lv_label_set_text(h.session_pct, pct);
  } else {
    lv_label_set_text(h.session_reset, "-");
    lv_bar_set_value(h.session_bar, 0, LV_ANIM_OFF);
    lv_label_set_text(h.session_pct, "-");
  }

  if (d.weekly.present) {
    lv_label_set_text(h.weekly_reset, d.weekly.resetIn);
    lv_bar_set_value(h.weekly_bar, d.weekly.utilizationPct, LV_ANIM_OFF);
    char pct[8];
    snprintf(pct, sizeof(pct), "%d%%", (int)d.weekly.utilizationPct);
    lv_label_set_text(h.weekly_pct, pct);
  } else {
    lv_label_set_text(h.weekly_reset, "-");
    lv_bar_set_value(h.weekly_bar, 0, LV_ANIM_OFF);
    lv_label_set_text(h.weekly_pct, "-");
  }

  lv_label_set_text(h.extra_on, d.extraEnabled ? "On" : "Off");
  lv_obj_set_style_text_color(h.extra_on,
      lv_color_hex(d.extraEnabled ? kProgressFill : kTextLo), 0);
}

}  // namespace

void buildGridScreen() {
  lv_obj_t* scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  constexpr int kPad = 16;
  constexpr int kCw = (1280 - 3 * kPad) / 2;
  constexpr int kCh = (720 - 3 * kPad) / 2;
  const int x0 = kPad;
  const int x1 = kPad + kCw + kPad;
  const int y0 = kPad;
  const int y1 = kPad + kCh + kPad;

  buildWeatherCard(scr, x0, y0, kCw, kCh);

  // Placeholder values that show on first paint before the first poll
  // returns. updateClaudeCard / updateCodexCard overwrite all of these
  // on the first /api/claude and /api/codex hit.
  const UsageData claude = {
      .email = "-",
      .plan_label = "—",
      .plan_bg = kPillMaxBg,
      .session_pct = 0,
      .session_reset = "—",
      .weekly_pct = 0,
      .weekly_reset = "—",
      .extra_enabled = false,
  };
  buildUsageCard(scr, x1, y0, kCw, kCh, kAccentClaude, claude, &g_claude);

  const UsageData codex = {
      .email = "-",
      .plan_label = "—",
      .plan_bg = kPillPlusBg,
      .session_pct = 0,
      .session_reset = "—",
      .weekly_pct = 0,
      .weekly_reset = "—",
      .extra_enabled = false,
  };
  buildUsageCard(scr, x0, y1, kCw, kCh, kAccentCodex, codex, &g_codex);

  // Allocate the brand-icon PSRAM buffers up front so the poller
  // doesn't have to deal with first-call init.
  initBrandIcon(g_brand_claude);
  initBrandIcon(g_brand_codex);

  buildInboxCard(scr, x1, y1, kCw, kCh);
}

void updateWeatherCard(const data::WeatherData& d) {
  if (!d.valid || g_wx.city == nullptr) return;

  lv_label_set_text(g_wx.city, d.city);

  char buf[32];
  snprintf(buf, sizeof(buf), "%d°C", (int)d.tempC);
  lv_label_set_text(g_wx.temp, buf);

  lv_label_set_text(g_wx.cond, d.condition);

  char details[96];
  snprintf(details, sizeof(details),
           "H %d  L %d\nWind %d km/h\nHumidity %d%%",
           (int)d.highC, (int)d.lowC,
           (int)d.windKmh, (int)d.humidityPct);
  lv_label_set_text(g_wx.details, details);

  if (d.air.present) {
    char aqi_buf[48];
    snprintf(aqi_buf, sizeof(aqi_buf), "AQI %u  %s",
             (unsigned)d.air.aqi, d.air.category);
    lv_label_set_text(g_wx.aqi, aqi_buf);
    uint32_t hex = ((uint32_t)d.air.r << 16) |
                   ((uint32_t)d.air.g << 8) |
                   (uint32_t)d.air.b;
    lv_obj_set_style_text_color(g_wx.aqi, lv_color_hex(hex), 0);
  } else {
    lv_label_set_text(g_wx.aqi, "");
  }

  for (int i = 0; i < 5; ++i) {
    lv_label_set_text(g_wx.fc_day[i], d.forecast[i].day);
    lv_label_set_text(g_wx.fc_glyph[i], d.forecast[i].glyph);
    char hilo[16];
    snprintf(hilo, sizeof(hilo), "%d / %d",
             (int)d.forecast[i].highC, (int)d.forecast[i].lowC);
    lv_label_set_text(g_wx.fc_hilo[i], hilo);
  }
}

void updateClaudeCard(const data::ClaudeData& d) { applyUsage(g_claude, d); }
void updateCodexCard(const data::CodexData& d) { applyUsage(g_codex, d); }

void updateInboxCard(const data::NewsData& d) {
  if (!d.valid || g_inbox.count == nullptr) return;

  char count[16];
  snprintf(count, sizeof(count), "%u NEW", (unsigned)d.count);
  lv_label_set_text(g_inbox.count, count);

  for (int i = 0; i < 3; ++i) {
    if (i < d.count) {
      lv_label_set_text(g_inbox.items[i], d.items[i].title);
    } else {
      lv_label_set_text(g_inbox.items[i], "-");
    }
  }
}

void setWeatherIconPng(const uint8_t* png, size_t len) {
  if (!g_wx.icon || !g_icon_buf) return;
  if (len > kIconBufBytes) return;
  if (len == 0) {
    lv_image_set_src(g_wx.icon, nullptr);
    g_icon_len = 0;
    return;
  }

  memcpy(g_icon_buf, png, len);
  g_icon_len = len;
  g_icon_dsc.data = g_icon_buf;
  g_icon_dsc.data_size = len;
  lv_image_set_src(g_wx.icon, nullptr);
  lv_image_set_src(g_wx.icon, &g_icon_dsc);
}

void setClaudeIconPng(const uint8_t* png, size_t len) {
  applyBrandIcon(g_brand_claude, g_claude.icon, png, len);
}
void setCodexIconPng(const uint8_t* png, size_t len) {
  applyBrandIcon(g_brand_codex, g_codex.icon, png, len);
}

}  // namespace ui
