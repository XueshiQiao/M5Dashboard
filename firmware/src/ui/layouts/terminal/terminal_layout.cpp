// Terminal-style layout — registers as ui::kTerminalLayout.
//
// Pure LVGL: this translation unit deliberately does NOT include M5Unified
// (M5GFX bundles its own mini-LVGL types whose guards differ from LVGL's,
// so mixing them in one .cpp causes "conflicting declaration" errors).

#include "ui/layouts/terminal/terminal_layout.h"

#include <stdio.h>
#include <string.h>
#include <lvgl.h>

namespace ui {
namespace {

struct WeatherHandles {
  lv_obj_t* summary;
  lv_obj_t* forecast;
  lv_obj_t* aqi;
  lv_obj_t* top_weather;
};

struct UsageHandles {
  lv_obj_t* account;
  lv_obj_t* session;
  lv_obj_t* weekly;
  lv_obj_t* extra;
  lv_obj_t* top_line;
};

WeatherHandles g_wx = {};
UsageHandles g_claude = {};
UsageHandles g_codex = {};
lv_obj_t*    g_news_body = nullptr;   // news label inside the news+todo pane

constexpr uint32_t kBg = 0x050807;
constexpr uint32_t kPanel = 0x08100C;
constexpr uint32_t kPanelAlt = 0x0B1510;
constexpr uint32_t kBorder = 0x224431;
constexpr uint32_t kGrid = 0x14271D;

constexpr uint32_t kText = 0xD7FFE6;
constexpr uint32_t kMuted = 0x79A486;
constexpr uint32_t kDim = 0x47634F;
constexpr uint32_t kGreen = 0x6BFF9A;
constexpr uint32_t kCyan = 0x6DEBFF;
constexpr uint32_t kYellow = 0xFFE66B;
constexpr uint32_t kOrange = 0xFFB86B;
constexpr uint32_t kPink = 0xFF7AB6;

const lv_font_t* kTerm = &lv_font_unscii_16;
const lv_font_t* kTermSmall = &lv_font_montserrat_14;
const lv_font_t* kTermTitle = &lv_font_montserrat_20;

lv_obj_t* makeLabel(lv_obj_t* parent, const char* text, const lv_font_t* font,
                    uint32_t color, lv_text_align_t align = LV_TEXT_ALIGN_LEFT) {
  lv_obj_t* label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
  lv_obj_set_style_text_align(label, align, 0);
  lv_obj_set_style_text_letter_space(label, 0, 0);
  lv_obj_set_style_text_line_space(label, 6, 0);
  return label;
}

lv_obj_t* makePane(lv_obj_t* parent, int x, int y, int w, int h,
                   const char* title, uint32_t accent) {
  lv_obj_t* pane = lv_obj_create(parent);
  lv_obj_remove_style_all(pane);
  lv_obj_set_pos(pane, x, y);
  lv_obj_set_size(pane, w, h);
  lv_obj_set_scrollbar_mode(pane, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(pane, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(pane, lv_color_hex(kPanel), 0);
  lv_obj_set_style_bg_opa(pane, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(pane, 1, 0);
  lv_obj_set_style_border_color(pane, lv_color_hex(kBorder), 0);
  lv_obj_set_style_radius(pane, 0, 0);

  lv_obj_t* title_label = makeLabel(pane, title, kTerm, accent);
  lv_obj_set_size(title_label, w - 24, 22);
  lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 12, 10);

  lv_obj_t* rule = lv_obj_create(pane);
  lv_obj_remove_style_all(rule);
  lv_obj_set_pos(rule, 12, 38);
  lv_obj_set_size(rule, w - 24, 1);
  lv_obj_set_style_bg_color(rule, lv_color_hex(kGrid), 0);
  lv_obj_set_style_bg_opa(rule, LV_OPA_COVER, 0);

  return pane;
}

void setBody(lv_obj_t* label, const char* text, uint32_t color = kText) {
  if (!label) return;
  lv_label_set_text(label, text);
  lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
}

void usageText(const data::ClaudeData& d, char* account, size_t account_cap,
               char* session, size_t session_cap, char* weekly, size_t weekly_cap,
               char* extra, size_t extra_cap, char* top, size_t top_cap) {
  const char* email = d.emailMasked[0] ? d.emailMasked : d.email;
  const char* plan = d.plan[0] ? d.plan : "-";
  snprintf(account, account_cap, "$ whoami\nuser: %s\nplan: %s", email, plan);

  if (d.session.present) {
    snprintf(session, session_cap, "$ quota --session\nused: %d%%\nreset: %s",
             (int)d.session.utilizationPct, d.session.resetIn);
  } else {
    snprintf(session, session_cap, "$ quota --session\nused: -\nreset: -");
  }

  if (d.weekly.present) {
    snprintf(weekly, weekly_cap, "$ quota --weekly\nused: %d%%\nreset: %s",
             (int)d.weekly.utilizationPct, d.weekly.resetIn);
  } else {
    snprintf(weekly, weekly_cap, "$ quota --weekly\nused: -\nreset: -");
  }

  snprintf(extra, extra_cap, "$ extra-usage\nstate: %s",
           d.extraEnabled ? "enabled" : "disabled");

  int session_pct = d.session.present ? d.session.utilizationPct : 0;
  int weekly_pct = d.weekly.present ? d.weekly.utilizationPct : 0;
  snprintf(top, top_cap, "%s  %s  S:%d%% W:%d%%", email, plan, session_pct, weekly_pct);
}

void applyUsage(const UsageHandles& h, const data::ClaudeData& d) {
  if (!d.valid || h.account == nullptr) return;

  char account[128];
  char session[128];
  char weekly[128];
  char extra[80];
  char top[96];
  usageText(d, account, sizeof(account), session, sizeof(session),
            weekly, sizeof(weekly), extra, sizeof(extra), top, sizeof(top));

  setBody(h.account, account);
  setBody(h.session, session, d.session.utilizationPct >= 80 ? kOrange : kText);
  setBody(h.weekly, weekly, d.weekly.utilizationPct >= 80 ? kOrange : kText);
  setBody(h.extra, extra, d.extraEnabled ? kGreen : kMuted);
  setBody(h.top_line, top, kMuted);
}

void buildTop(lv_obj_t* scr) {
  lv_obj_t* bar = lv_obj_create(scr);
  lv_obj_remove_style_all(bar);
  lv_obj_set_pos(bar, 20, 16);
  lv_obj_set_size(bar, 1240, 74);
  lv_obj_set_style_bg_color(bar, lv_color_hex(kPanelAlt), 0);
  lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(bar, 1, 0);
  lv_obj_set_style_border_color(bar, lv_color_hex(kBorder), 0);
  lv_obj_set_scrollbar_mode(bar, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* prompt = makeLabel(bar, "joey@m5tab5:~$ fastfetch --dashboard",
                               kTermTitle, kGreen);
  lv_obj_set_size(prompt, 610, 30);
  lv_obj_align(prompt, LV_ALIGN_TOP_LEFT, 18, 12);

  g_wx.top_weather = makeLabel(bar, "weather pending", kTerm, kYellow);
  lv_obj_set_size(g_wx.top_weather, 290, 24);
  lv_obj_align(g_wx.top_weather, LV_ALIGN_TOP_RIGHT, -18, 12);

  g_claude.top_line = makeLabel(bar, "claude pending", kTermSmall, kMuted);
  lv_obj_set_size(g_claude.top_line, 560, 20);
  lv_obj_align(g_claude.top_line, LV_ALIGN_BOTTOM_LEFT, 18, -10);

  g_codex.top_line = makeLabel(bar, "codex pending", kTermSmall, kMuted);
  lv_obj_set_size(g_codex.top_line, 560, 20);
  lv_obj_align(g_codex.top_line, LV_ALIGN_BOTTOM_RIGHT, -18, -10);
}

void buildIdentity(lv_obj_t* scr) {
  lv_obj_t* pane = makePane(scr, 20, 106, 430, 344, "[m5dashboard]", kGreen);

  static const char* kLogo =
      "        ____  _____\n"
      "       / __ \\/ ___/\n"
      "      / / / /\\__ \\ \n"
      "     / /_/ /___/ / \n"
      "    /_____//____/  \n"
      "                    \n"
      "   M5 TAB5 // 720P  \n"
      "   LOCAL INFO NODE  ";

  lv_obj_t* logo = makeLabel(pane, kLogo, kTerm, kGreen);
  lv_obj_set_size(logo, 390, 170);
  lv_obj_align(logo, LV_ALIGN_TOP_LEFT, 20, 58);

  static const char* kMeta =
      "os: m5dashboard\n"
      "host: esp32-p4 tab5\n"
      "res: 1280x720\n"
      "shell: local server :8787\n"
      "theme: terminal";
  lv_obj_t* meta = makeLabel(pane, kMeta, kTerm, kCyan);
  lv_obj_set_size(meta, 390, 110);
  lv_obj_align(meta, LV_ALIGN_BOTTOM_LEFT, 20, -18);
}

void buildWeatherPane(lv_obj_t* scr) {
  lv_obj_t* pane = makePane(scr, 470, 106, 790, 190, "[weather]", kYellow);

  g_wx.summary = makeLabel(pane,
                           "$ weather --now\n"
                           "city: pending\n"
                           "temp: -\n"
                           "wind: -",
                           kTerm, kText);
  lv_obj_set_size(g_wx.summary, 300, 130);
  lv_obj_align(g_wx.summary, LV_ALIGN_TOP_LEFT, 20, 58);

  g_wx.aqi = makeLabel(pane,
                       "$ air-quality\n"
                       "aqi: -\n"
                       "primary: -",
                       kTerm, kMuted);
  lv_obj_set_size(g_wx.aqi, 220, 100);
  lv_obj_align(g_wx.aqi, LV_ALIGN_TOP_LEFT, 340, 58);

  g_wx.forecast = makeLabel(pane,
                            "$ forecast --5d\n"
                            "waiting for server",
                            kTerm, kText);
  lv_obj_set_size(g_wx.forecast, 220, 120);
  lv_obj_align(g_wx.forecast, LV_ALIGN_TOP_RIGHT, -20, 58);
}

void buildUsagePane(lv_obj_t* scr, int x, int y, int w, int h,
                    const char* title, uint32_t accent, UsageHandles* handles) {
  lv_obj_t* pane = makePane(scr, x, y, w, h, title, accent);

  handles->account = makeLabel(pane, "$ whoami\nuser: pending\nplan: -", kTerm, kText);
  lv_obj_set_size(handles->account, w - 40, 78);
  lv_obj_align(handles->account, LV_ALIGN_TOP_LEFT, 20, 56);

  handles->session = makeLabel(pane, "$ quota --session\nused: -\nreset: -", kTerm, kText);
  lv_obj_set_size(handles->session, w - 40, 78);
  lv_obj_align(handles->session, LV_ALIGN_TOP_LEFT, 20, 146);

  handles->weekly = makeLabel(pane, "$ quota --weekly\nused: -\nreset: -", kTerm, kText);
  lv_obj_set_size(handles->weekly, w - 40, 78);
  lv_obj_align(handles->weekly, LV_ALIGN_TOP_LEFT, 20, 236);

  handles->extra = makeLabel(pane, "$ extra-usage\nstate: -", kTerm, kMuted);
  lv_obj_set_size(handles->extra, w - 40, 56);
  lv_obj_align(handles->extra, LV_ALIGN_BOTTOM_LEFT, 20, -14);
}

void buildNewsTodoPane(lv_obj_t* scr) {
  lv_obj_t* pane = makePane(scr, 884, 316, 376, 384, "[news + todo]", kPink);

  // Placeholder until the first /api/news poll lands. The label height
  // is bumped to 150 px so three Kagi-length headlines (which can wrap
  // to 2 lines each) fit without crowding the to-do list below.
  static const char* kNewsBootstrap =
      "$ news --brief\n"
      "> waiting for first poll...";
  lv_obj_t* news = makeLabel(pane, kNewsBootstrap, kTerm, kText);
  lv_obj_set_size(news, 336, 150);
  lv_obj_align(news, LV_ALIGN_TOP_LEFT, 20, 56);
  g_news_body = news;

  lv_obj_t* rule = lv_obj_create(pane);
  lv_obj_remove_style_all(rule);
  lv_obj_set_pos(rule, 20, 215);
  lv_obj_set_size(rule, 336, 1);
  lv_obj_set_style_bg_color(rule, lv_color_hex(kGrid), 0);
  lv_obj_set_style_bg_opa(rule, LV_OPA_COVER, 0);

  static const char* kTodos =
      "$ todo ls\n"
      "[x] wire real inbox source\n"
      "[ ] add agenda endpoint\n"
      "[ ] tune terminal palette\n"
      "[x] weather + usage live";
  lv_obj_t* todos = makeLabel(pane, kTodos, kTerm, kCyan);
  lv_obj_set_size(todos, 336, 140);
  lv_obj_align(todos, LV_ALIGN_TOP_LEFT, 20, 230);
}

void buildGridScreen() {
  lv_obj_t* scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(kBg), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  buildTop(scr);
  buildIdentity(scr);
  buildWeatherPane(scr);
  buildUsagePane(scr, 20, 470, 410, 230, "[claude-code]", kCyan, &g_claude);
  buildUsagePane(scr, 450, 316, 414, 384, "[codex]", kGreen, &g_codex);
  buildNewsTodoPane(scr);
}

void updateWeatherCard(const data::WeatherData& d) {
  if (!d.valid || g_wx.summary == nullptr) return;

  char summary[160];
  snprintf(summary, sizeof(summary),
           "$ weather --now\ncity: %s\ntemp: %dC / %s\nrange: H %d  L %d\nwind: %d km/h\nhumidity: %d%%",
           d.city, (int)d.tempC, d.condition, (int)d.highC, (int)d.lowC,
           (int)d.windKmh, (int)d.humidityPct);
  setBody(g_wx.summary, summary);

  char top[96];
  snprintf(top, sizeof(top), "%s  %dC  %s", d.city, (int)d.tempC, d.condition);
  setBody(g_wx.top_weather, top, kYellow);

  if (d.air.present) {
    char aqi[128];
    snprintf(aqi, sizeof(aqi),
             "$ air-quality\naqi: %u %s\nprimary: %s",
             (unsigned)d.air.aqi, d.air.category, d.air.primary);
    uint32_t hex = ((uint32_t)d.air.r << 16) |
                   ((uint32_t)d.air.g << 8) |
                   (uint32_t)d.air.b;
    setBody(g_wx.aqi, aqi, hex);
  } else {
    setBody(g_wx.aqi, "$ air-quality\naqi: -\nprimary: -", kMuted);
  }

  char forecast[192];
  snprintf(forecast, sizeof(forecast),
           "$ forecast --5d\n%s  %d/%d  %s\n%s  %d/%d  %s\n%s  %d/%d  %s\n%s  %d/%d  %s\n%s  %d/%d  %s",
           d.forecast[0].day, (int)d.forecast[0].highC, (int)d.forecast[0].lowC, d.forecast[0].glyph,
           d.forecast[1].day, (int)d.forecast[1].highC, (int)d.forecast[1].lowC, d.forecast[1].glyph,
           d.forecast[2].day, (int)d.forecast[2].highC, (int)d.forecast[2].lowC, d.forecast[2].glyph,
           d.forecast[3].day, (int)d.forecast[3].highC, (int)d.forecast[3].lowC, d.forecast[3].glyph,
           d.forecast[4].day, (int)d.forecast[4].highC, (int)d.forecast[4].lowC, d.forecast[4].glyph);
  setBody(g_wx.forecast, forecast);
}

void updateClaudeCard(const data::ClaudeData& d) { applyUsage(g_claude, d); }
void updateCodexCard(const data::CodexData& d) { applyUsage(g_codex, d); }

void updateInboxCard(const data::NewsData& d) {
  if (!d.valid || g_news_body == nullptr) return;
  // Build a single multi-line string in terminal style:
  //   $ news --brief
  //   > Item one title (truncated to ~40 chars)
  //   > Item two ...
  //   > Item three ...
  // Truncation matches the unscii-16 char width inside the 336 px label.
  constexpr size_t kMaxLine = 40;
  char body[512];
  size_t off = 0;
  off += snprintf(body + off, sizeof(body) - off, "$ news --brief");
  if (d.count == 0) {
    off += snprintf(body + off, sizeof(body) - off, "\n> (no items)");
  }
  for (uint8_t i = 0; i < d.count && off < sizeof(body) - 4; ++i) {
    const char* t = d.items[i].title;
    char trimmed[kMaxLine + 4];
    size_t tlen = strnlen(t, kMaxLine);
    memcpy(trimmed, t, tlen);
    if (strlen(t) > kMaxLine) {
      trimmed[kMaxLine - 1] = '.';
      trimmed[kMaxLine - 2] = '.';
      trimmed[kMaxLine - 3] = '.';
      trimmed[kMaxLine]     = '\0';
    } else {
      trimmed[tlen] = '\0';
    }
    off += snprintf(body + off, sizeof(body) - off, "\n> %s", trimmed);
  }
  setBody(g_news_body, body, kText);
}

void setWeatherIconPng(const uint8_t* png, size_t len) {
  (void)png;
  (void)len;
}

}  // namespace

const Layout kTerminalLayout = {
  "terminal",
  buildGridScreen,
  updateWeatherCard,
  updateClaudeCard,
  updateCodexCard,
  updateInboxCard,
  setWeatherIconPng,
  nullptr,
  nullptr,
  { 0, 0 },
};

}  // namespace ui
