// Todos layout — registers as ui::kTodosLayout.
//
// Hardcoded list (no server endpoint yet). Each row is a clickable
// label that toggles its [x] / [ ] checkbox on tap. Tap = single press
// + release without significant drag, so screen-level swipes still
// trigger the layout-cycle gesture handler unchanged.

#include "ui/layouts/todos/todos_layout.h"

#include "ui/fonts/fonts.h"

#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

constexpr uint32_t kBg     = 0x000805;
constexpr uint32_t kAccent = 0xFF7AB6;
constexpr uint32_t kDone   = 0x6B8478;
constexpr uint32_t kOpen   = 0xE7E7E0;
constexpr uint32_t kRule   = 0x402034;

constexpr int kPadL       = 24;
constexpr int kPadR       = 24;
constexpr int kListStartY = 90;
constexpr int kRowH       = 56;        // Tamzen 40b line ~36 + padding

struct Todo {
  const char* text;
  bool        done;
};

Todo g_todos[] = {
  { "Setup Claude Code CLI",            true  },
  { "Wire weather endpoint",            true  },
  { "Add Codex usage polling",          true  },
  { "Multi-layout swipe navigation",    true  },
  { "Tamzen pixel font pipeline",       true  },
  { "News layout with wrapping items",  false },
  { "Todos persistence (NVS)",          false },
  { "Sunrise / sunset on weather card", false },
};
constexpr int kTodoCount = sizeof(g_todos) / sizeof(g_todos[0]);

struct Handles {
  lv_obj_t* count_label;
  lv_obj_t* row[kTodoCount];
};
Handles g_h = {};

lv_font_t g_font_body;
const lv_font_t* kBodyFont   = nullptr;
const lv_font_t* kHeaderFont = nullptr;

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

void renderRow(int idx) {
  if (idx < 0 || idx >= kTodoCount || !g_h.row[idx]) return;
  char buf[224];
  snprintf(buf, sizeof(buf), "%s  %s",
           g_todos[idx].done ? "[x]" : "[ ]",
           g_todos[idx].text);
  lv_label_set_text(g_h.row[idx], buf);
  lv_obj_set_style_text_color(g_h.row[idx],
      lv_color_hex(g_todos[idx].done ? kDone : kOpen), 0);
}

void renderCount() {
  if (!g_h.count_label) return;
  int done = 0;
  for (int i = 0; i < kTodoCount; ++i) if (g_todos[i].done) ++done;
  char buf[32];
  snprintf(buf, sizeof(buf), "%d / %d done", done, kTodoCount);
  lv_label_set_text(g_h.count_label, buf);
}

void rowClickedCb(lv_event_t* e) {
  // Index is encoded in user_data as (intptr_t).
  intptr_t idx = (intptr_t)lv_event_get_user_data(e);
  if (idx < 0 || idx >= kTodoCount) return;
  g_todos[idx].done = !g_todos[idx].done;
  renderRow((int)idx);
  renderCount();
}

// ─── build / destroy ────────────────────────────────────────────────────
void destroy() {
  memset(&g_h, 0, sizeof(g_h));
}

void buildScreen() {
  g_font_body = lv_font_tamzen_40b;
  kBodyFont   = &g_font_body;
  kHeaderFont = &lv_font_montserrat_28;

  lv_obj_t* scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  // Header.
  lv_obj_t* title = mkLabel(scr, "$ todo --list", kHeaderFont, kAccent);
  lv_obj_set_pos(title, kPadL, 22);
  lv_obj_set_width(title, 600);

  g_h.count_label = mkLabel(scr, "", kHeaderFont, kAccent, LV_TEXT_ALIGN_RIGHT);
  lv_obj_set_pos(g_h.count_label, 1280 - kPadR - 240, 22);
  lv_obj_set_width(g_h.count_label, 240);

  lv_obj_t* rule = lv_obj_create(scr);
  lv_obj_remove_style_all(rule);
  lv_obj_set_pos(rule, kPadL, 78);
  lv_obj_set_size(rule, 1280 - kPadL - kPadR, 1);
  lv_obj_set_style_bg_color(rule, lv_color_hex(kRule), 0);
  lv_obj_set_style_bg_opa(rule, LV_OPA_COVER, 0);

  // Rows. Each row is a clickable label spanning the full content width.
  for (int i = 0; i < kTodoCount; ++i) {
    g_h.row[i] = mkLabel(scr, "", kBodyFont, kOpen);
    lv_obj_set_pos(g_h.row[i], kPadL, kListStartY + i * kRowH);
    lv_obj_set_size(g_h.row[i], 1280 - kPadL - kPadR, kRowH - 4);
    lv_obj_add_flag(g_h.row[i], LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(g_h.row[i], rowClickedCb, LV_EVENT_CLICKED,
                        (void*)(intptr_t)i);
    renderRow(i);
  }
  renderCount();
}

}  // namespace

const Layout kTodosLayout = {
  "todos",
  buildScreen,
  destroy,
  nullptr, nullptr, nullptr, nullptr,    // no data callbacks
  nullptr, nullptr, nullptr, nullptr,
  { 0, 0, 0 },
};

}  // namespace ui
