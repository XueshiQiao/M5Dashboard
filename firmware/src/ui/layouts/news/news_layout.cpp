// News layout — registers as ui::kNewsLayout.
//
// Full-screen list of news items. Each item is a wrapping label whose
// height is determined per-update by LVGL's measurement of the wrapped
// text, so short headlines occupy one row and long ones expand to two.

#include "ui/layouts/news/news_layout.h"

#include "ui/fonts/fonts.h"

#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

constexpr uint32_t kBg     = 0x000805;
constexpr uint32_t kAccent = 0x55FF7F;
constexpr uint32_t kTitle  = 0x77DDFF;
constexpr uint32_t kIndex  = 0xFFC857;
constexpr uint32_t kText   = 0xE7E7E0;
constexpr uint32_t kMuted  = 0x80978B;
constexpr uint32_t kRule   = 0x224030;

const lv_font_t* kHeaderFont = nullptr;   // set in build()
const lv_font_t* kBodyFont   = nullptr;

lv_font_t g_font_body;

// 1280×720 panel. Header eats ~70 px; rest is the scrollable item area.
constexpr int kPadL        = 24;
constexpr int kPadR        = 24;
constexpr int kListStartY  = 90;
constexpr int kListEndY    = 712;
constexpr int kItemGap     = 12;
constexpr int kIndexW      = 64;            // left badge column

struct Handles {
  lv_obj_t* count_label;
  lv_obj_t* row[data::kMaxNewsItems];       // wrapping title labels
  lv_obj_t* badge[data::kMaxNewsItems];     // left index labels
};

Handles g_h = {};

lv_obj_t* mkLabel(lv_obj_t* parent, const char* text, const lv_font_t* font,
                  uint32_t color, lv_text_align_t align = LV_TEXT_ALIGN_LEFT) {
  lv_obj_t* l = lv_label_create(parent);
  lv_label_set_text(l, text);
  lv_obj_set_style_text_font(l, font, 0);
  lv_obj_set_style_text_color(l, lv_color_hex(color), 0);
  lv_obj_set_style_text_align(l, align, 0);
  lv_obj_set_style_text_letter_space(l, 0, 0);
  lv_obj_set_style_text_line_space(l, 4, 0);
  return l;
}

// ─── build / destroy ────────────────────────────────────────────────────
void destroy() {
  memset(&g_h, 0, sizeof(g_h));
}

void buildScreen() {
  g_font_body  = lv_font_tamzen_40b;
  kBodyFont    = &g_font_body;
  kHeaderFont  = &lv_font_montserrat_28;

  lv_obj_t* scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  // Header: title left, count right, dim rule below.
  lv_obj_t* title = mkLabel(scr, "$ news --feed kagi", kHeaderFont, kAccent);
  lv_obj_set_pos(title, kPadL, 22);
  lv_obj_set_width(title, 800);

  g_h.count_label = mkLabel(scr, "0 items", kHeaderFont, kMuted,
                            LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_pos(g_h.count_label, 1280 - kPadR - 240, 22);
  lv_obj_set_width(g_h.count_label, 240);

  lv_obj_t* rule = lv_obj_create(scr);
  lv_obj_remove_style_all(rule);
  lv_obj_set_pos(rule, kPadL, 78);
  lv_obj_set_size(rule, 1280 - kPadL - kPadR, 1);
  lv_obj_set_style_bg_color(rule, lv_color_hex(kRule), 0);
  lv_obj_set_style_bg_opa(rule, LV_OPA_COVER, 0);

  // Pre-create every potential item slot. They're hidden until data
  // arrives; each onNews update positions them in order.
  const int kRowW = 1280 - kPadL - kPadR - kIndexW - 12;
  for (int i = 0; i < data::kMaxNewsItems; ++i) {
    g_h.badge[i] = mkLabel(scr, "", kBodyFont, kIndex, LV_TEXT_ALIGN_RIGHT);
    lv_obj_set_pos(g_h.badge[i], kPadL, kListStartY);
    lv_obj_set_width(g_h.badge[i], kIndexW);
    lv_obj_add_flag(g_h.badge[i], LV_OBJ_FLAG_HIDDEN);

    g_h.row[i] = mkLabel(scr, "", kBodyFont, kText);
    lv_label_set_long_mode(g_h.row[i], LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(g_h.row[i], kPadL + kIndexW + 12, kListStartY);
    lv_obj_set_width(g_h.row[i], kRowW);
    lv_obj_add_flag(g_h.row[i], LV_OBJ_FLAG_HIDDEN);
  }
}

// ─── update ─────────────────────────────────────────────────────────────
void onNews(const data::NewsData& d) {
  if (!g_h.count_label) return;

  char count[24];
  snprintf(count, sizeof(count), "%u items", (unsigned)d.count);
  lv_label_set_text(g_h.count_label, count);

  int y = kListStartY;
  int shown = 0;
  for (int i = 0; i < d.count && i < data::kMaxNewsItems; ++i) {
    if (!g_h.row[i] || !g_h.badge[i]) continue;

    char idx[8];
    snprintf(idx, sizeof(idx), "%02d", i + 1);
    lv_label_set_text(g_h.badge[i], idx);
    lv_label_set_text(g_h.row[i], d.items[i].title);

    lv_obj_clear_flag(g_h.row[i],  LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_h.badge[i], LV_OBJ_FLAG_HIDDEN);
    lv_obj_update_layout(g_h.row[i]);
    const int h = lv_obj_get_height(g_h.row[i]);

    if (y + h > kListEndY) {
      // No more vertical room — hide this and any remaining slots.
      lv_obj_add_flag(g_h.row[i],   LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(g_h.badge[i], LV_OBJ_FLAG_HIDDEN);
      break;
    }

    lv_obj_set_y(g_h.row[i],   y);
    lv_obj_set_y(g_h.badge[i], y);
    y += h + kItemGap;
    shown = i + 1;
  }
  // Hide any leftover slots from a prior larger update.
  for (int i = shown; i < data::kMaxNewsItems; ++i) {
    if (g_h.row[i])   lv_obj_add_flag(g_h.row[i],   LV_OBJ_FLAG_HIDDEN);
    if (g_h.badge[i]) lv_obj_add_flag(g_h.badge[i], LV_OBJ_FLAG_HIDDEN);
  }
}

}  // namespace

const Layout kNewsLayout = {
  "news",
  buildScreen,
  destroy,
  nullptr,           // no weather
  nullptr,           // no claude
  nullptr,           // no codex
  onNews,
  nullptr,
  nullptr,
  nullptr,
  { 0, 0 },
};

}  // namespace ui
