// layouts.cpp — three full-screen layouts. Each draws its content into the
// 1280x640 area (rows 0..639); main.cpp paints the bottom 80px tab bar.

#include <M5Unified.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "theme.h"
#include "layouts.h"

namespace ui {
namespace {

// ---- shared drawing helpers ----------------------------------------------

void drawCard(int x, int y, int w, int h, uint16_t accent) {
  auto& d = M5.Display;
  d.fillRect(x, y, w, h, ui::card());
  d.drawRect(x, y, w, h, ui::border());
  d.fillRect(x + 1, y + 1, w - 2, 3, accent);
}

void drawTitle(int x, int y, int w, const char* title, const char* badge,
               uint16_t accent) {
  auto& d = M5.Display;
  d.setFont(&fonts::FreeMonoBold12pt7b);
  d.setTextDatum(top_left);
  d.setTextColor(accent, ui::card());
  d.drawString(title, x + 18, y + 14);
  if (badge) {
    d.setFont(&fonts::FreeMono9pt7b);
    d.setTextDatum(top_right);
    d.setTextColor(ui::text_lo(), ui::card());
    d.drawString(badge, x + w - 18, y + 18);
  }
}

void drawSparkline(int x, int y, int w, int h,
                   const uint8_t* values, int n, uint16_t color) {
  auto& d = M5.Display;
  const int gap   = 1;
  const int bar_w = (w - (n - 1) * gap) / n;
  const int total = bar_w * n + gap * (n - 1);
  const int x0    = x + (w - total) / 2;
  for (int i = 0; i < n; ++i) {
    int v  = values[i];
    int bh = (v * h) / 100;
    if (bh < 2) bh = 2;
    int bx = x0 + i * (bar_w + gap);
    int by = y + h - bh;
    d.fillRect(bx, by, bar_w, bh, color);
  }
}

void drawQuotaBar(int x, int y, int w, int h, int percent, uint16_t color) {
  auto& d = M5.Display;
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  d.fillRect(x, y, w, h, ui::card_alt());
  int filled = (w * percent) / 100;
  if (filled > 0) d.fillRect(x, y, filled, h, color);
  d.drawRect(x, y, w, h, ui::border());
}

// ---- fake data -----------------------------------------------------------

constexpr uint8_t kSparkClaude[16] = {
  12,18,32,40,55,72,88,95,82,68,55,40,28,18,12,8
};
constexpr uint8_t kSparkCodex[16]  = {
  18,12,28,16,42,18,28,32,12,18,12,28,18,12,8,12
};

const char* const kInboxItems[3] = {
  "GPT-4o vision rollout",
  "Anthropic ships Sonnet 4.7",
  "Tab5 review pt 3 published",
};

const char* const kTickerItems[5] = {
  "GPT-4o vision rollout",
  "Anthropic ships Sonnet 4.7",
  "CNX-Software Tab5 review pt 3",
  "Apple WWDC 2026 keynote",
  "HN top: M5Stack ESP32-P4 review",
};

struct DayWX { const char* day; const char* glyph; int hi; int lo; };
constexpr DayWX kForecast[5] = {
  {"MON", "PCLDY",  25, 14},
  {"TUE", "RAIN",   23, 13},
  {"WED", "RAIN",   20, 12},
  {"THU", "SUN",    19, 11},
  {"FRI", "SUN",    22, 14},
};

// FreeMono fonts ship without the ° glyph (0xB0), so we draw the symbol
// manually as a small donut. cx,cy is its center.
void drawDegSymbol(int cx, int cy, int outer_r, uint16_t fg, uint16_t bg) {
  auto& d = M5.Display;
  d.fillCircle(cx, cy, outer_r,     fg);
  d.fillCircle(cx, cy, outer_r - 2, bg);
}

// ---- new helpers used by Plan A's redesigned cards ----

// Generic "person" silhouette inside a colored disc.
void drawAvatar(int cx, int cy, int r, uint16_t bg, uint16_t fg) {
  auto& d = M5.Display;
  d.fillCircle(cx, cy, r, bg);
  // head: small filled circle in the upper portion
  int head_r = r * 7 / 16;
  d.fillCircle(cx, cy - r * 3 / 8, head_r, fg);
  // shoulders: rounded rect, sized so it stays inside the disc
  int sw = r * 5 / 4;
  int sh = r / 2;
  d.fillRoundRect(cx - sw / 2, cy + r / 4, sw, sh, sh / 2, fg);
}

// Pill-shaped badge with centered label.
void drawPill(int x, int y, int w, int h, const char* label,
              uint16_t bg, uint16_t fg, const lgfx::IFont* font) {
  auto& d = M5.Display;
  d.fillRoundRect(x, y, w, h, h / 2, bg);
  d.setFont(font);
  d.setTextDatum(middle_center);
  d.setTextColor(fg, bg);
  d.drawString(label, x + w / 2, y + h / 2 + 1);
}

// Rounded progress bar: dark track + colored fill, both with rounded ends.
void drawProgressBarRounded(int x, int y, int w, int h, int percent,
                            uint16_t track, uint16_t fill) {
  auto& d = M5.Display;
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  d.fillRoundRect(x, y, w, h, h / 2, track);
  int filled = (w * percent) / 100;
  if (filled >= h) {
    d.fillRoundRect(x, y, filled, h, h / 2, fill);
  } else if (filled > 0) {
    d.fillCircle(x + h / 2, y + h / 2, filled / 2, fill);
  }
}

// Programmatic "partly cloudy" icon (sun + foreground cloud).
void drawWeatherIconPartlyCloudy(int cx, int cy, int size) {
  auto& d = M5.Display;
  uint16_t sun_color  = ui::rgb(0xFF, 0xCC, 0x4F);
  uint16_t sun_glow   = ui::rgb(0xFF, 0xE5, 0x8A);
  uint16_t cloud_main = ui::rgb(0xE6, 0xED, 0xF3);
  uint16_t cloud_dim  = ui::rgb(0xC8, 0xD0, 0xDA);

  int sun_r  = size * 22 / 100;
  int sun_cx = cx - size / 7;
  int sun_cy = cy - size / 7;

  // 8 rays radiating outward
  for (int i = 0; i < 8; ++i) {
    float a   = i * (M_PI * 2.0f / 8.0f);
    float ca  = cosf(a), sa = sinf(a);
    int r0    = sun_r + 6;
    int r1    = sun_r + 18;
    int x1    = sun_cx + (int)(r0 * ca);
    int y1    = sun_cy + (int)(r0 * sa);
    int x2    = sun_cx + (int)(r1 * ca);
    int y2    = sun_cy + (int)(r1 * sa);
    // 3-wide thick line via perpendicular offsets
    for (int t = -1; t <= 1; ++t) {
      d.drawLine(x1 + (int)(t * sa), y1 - (int)(t * ca),
                 x2 + (int)(t * sa), y2 - (int)(t * ca), sun_color);
    }
  }
  d.fillCircle(sun_cx, sun_cy, sun_r + 2, sun_glow);
  d.fillCircle(sun_cx, sun_cy, sun_r,     sun_color);

  // Cloud lower-right, partially over the sun
  int cloud_cx = cx + size / 8;
  int cloud_cy = cy + size / 8;
  int cw       = size * 65 / 100;
  d.fillCircle(cloud_cx - cw / 3, cloud_cy + 2,        cw / 4, cloud_main);
  d.fillCircle(cloud_cx,          cloud_cy - cw / 6,   cw / 3, cloud_main);
  d.fillCircle(cloud_cx + cw / 3, cloud_cy + 2,        cw / 4, cloud_main);
  d.fillRoundRect(cloud_cx - cw / 2, cloud_cy, cw, cw * 2 / 5, cw / 8, cloud_main);
  // subtle bottom shadow
  d.drawFastHLine(cloud_cx - cw / 2 + 4, cloud_cy + cw * 2 / 5 - 1,
                  cw - 8, cloud_dim);
}

// Tiny weather icon for the 5-day strip. `kind` ∈ {SUN, PCLDY, RAIN, SNOW}.
void drawMiniWX(int cx, int cy, int size, const char* kind) {
  auto& d = M5.Display;
  uint16_t sun   = ui::rgb(0xFF, 0xCC, 0x4F);
  uint16_t cloud = ui::rgb(0xE6, 0xED, 0xF3);
  uint16_t rain  = ui::rgb(0x6F, 0xCB, 0xFF);

  if (strcmp(kind, "SUN") == 0) {
    int r = size * 35 / 100;
    for (int i = 0; i < 8; ++i) {
      float a = i * (M_PI * 2.0f / 8.0f);
      int x1 = cx + (int)((r + 3) * cosf(a));
      int y1 = cy + (int)((r + 3) * sinf(a));
      int x2 = cx + (int)((r + 8) * cosf(a));
      int y2 = cy + (int)((r + 8) * sinf(a));
      d.drawLine(x1, y1, x2, y2, sun);
    }
    d.fillCircle(cx, cy, r, sun);
  } else if (strcmp(kind, "PCLDY") == 0) {
    int r = size * 25 / 100;
    d.fillCircle(cx - size / 5, cy - size / 5, r, sun);
    int cw = size * 60 / 100;
    int cy2 = cy + size / 8;
    d.fillCircle(cx - cw / 4, cy2, cw / 5, cloud);
    d.fillCircle(cx + 2,      cy2 - cw / 8, cw / 4, cloud);
    d.fillCircle(cx + cw / 4, cy2, cw / 5, cloud);
    d.fillRoundRect(cx - cw / 2, cy2, cw, cw / 3, cw / 8, cloud);
  } else if (strcmp(kind, "RAIN") == 0) {
    int cw  = size * 70 / 100;
    int cy2 = cy - size / 8;
    d.fillCircle(cx - cw / 4, cy2,             cw / 5, cloud);
    d.fillCircle(cx,          cy2 - cw / 8,    cw / 4, cloud);
    d.fillCircle(cx + cw / 4, cy2,             cw / 5, cloud);
    d.fillRoundRect(cx - cw / 2, cy2, cw, cw / 3, cw / 8, cloud);
    // 3 raindrops below
    int by = cy2 + cw / 3 + 4;
    for (int i = 0; i < 3; ++i) {
      int rx = cx - cw / 3 + i * (cw / 3);
      d.fillTriangle(rx, by, rx - 2, by + 6, rx + 2, by + 6, rain);
    }
  } else {  // default: small cloud
    int cw = size * 60 / 100;
    d.fillCircle(cx - cw / 4, cy,           cw / 5, cloud);
    d.fillCircle(cx,          cy - cw / 8,  cw / 4, cloud);
    d.fillCircle(cx + cw / 4, cy,           cw / 5, cloud);
    d.fillRoundRect(cx - cw / 2, cy, cw, cw / 3, cw / 8, cloud);
  }
}

const char* const kDOW[7] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

}  // namespace

// =========================================================================
// Plan A — Glance Grid (2 x 2 of 640 x 320)
// =========================================================================

namespace {

// Weather card with the prominent icon.
void drawWeatherCardA(int x, int y, int w, int h) {
  auto& d = M5.Display;
  drawCard(x, y, w, h, ui::accent_wx());
  drawTitle(x, y, w, "WEATHER", "HALF MOON BAY", ui::accent_wx());

  // Icon 140x140 in left half (centered around x+150, y+150)
  int icon_cx = x + 150;
  int icon_cy = y + 145;
  drawWeatherIconPartlyCloudy(icon_cx, icon_cy, 140);

  // Right side: temp + condition + details
  int col_x = x + 280;

  d.setFont(&fonts::FreeMonoBold24pt7b);
  d.setTextDatum(middle_left);
  d.setTextColor(ui::text_hi(), ui::card());
  d.drawString("22", col_x, y + 110);
  drawDegSymbol(col_x + 82, y + 95, 6, ui::accent_wx(), ui::card());
  d.setFont(&fonts::FreeMonoBold18pt7b);
  d.setTextColor(ui::accent_wx(), ui::card());
  d.drawString("C", col_x + 96, y + 110);

  d.setFont(&fonts::FreeSans18pt7b);
  d.setTextColor(ui::text_md(), ui::card());
  d.drawString("Mostly sunny", col_x, y + 150);

  d.setFont(&fonts::FreeMono12pt7b);
  d.setTextColor(ui::text_lo(), ui::card());
  d.drawString("H 25  L 14",      col_x, y + 180);
  d.drawString("Wind 13 km/h W",  col_x, y + 202);
  d.drawString("Humidity 64%",    col_x, y + 224);

  // 5-day strip across the bottom
  int sx = x + 18, sy = y + 246, sw = w - 36, sh = h - 246 - 16;
  d.fillRect(sx, sy, sw, sh, ui::card_alt());
  d.drawRect(sx, sy, sw, sh, ui::border());
  int col_w = sw / 5;
  for (int i = 0; i < 5; ++i) {
    int cx = sx + i * col_w + col_w / 2;
    d.setFont(&fonts::FreeMonoBold12pt7b);
    d.setTextDatum(top_center);
    d.setTextColor(ui::text_md(), ui::card_alt());
    d.drawString(kForecast[i].day, cx, sy + 6);

    drawMiniWX(cx, sy + 32, 28, kForecast[i].glyph);

    char buf[16];
    snprintf(buf, sizeof(buf), "%d/%d", kForecast[i].hi, kForecast[i].lo);
    d.setFont(&fonts::FreeMonoBold12pt7b);
    d.setTextColor(ui::text_hi(), ui::card_alt());
    d.drawString(buf, cx, sy + sh - 22);
    if (i > 0) d.drawFastVLine(sx + i * col_w, sy + 8, sh - 16, ui::border());
  }
}

// Account-style usage card matching the user's reference: avatar + email +
// status pill + plan pill on top, two metric rows with rounded progress
// bars and reset countdowns, "extra usage" footer.
struct UsageCardData {
  const char*  email;
  const char*  plan_label;
  uint16_t     plan_bg;
  uint16_t     avatar_bg;
  // session quota
  int          session_pct;
  const char*  session_reset;     // e.g. "3h 6m to reset"
  // weekly (5-day) quota
  int          weekly_pct;
  const char*  weekly_reset;      // e.g. "Mon 06:00 reset"
  bool         extra_enabled;
};

void drawUsageCardA(int x, int y, int w, int h, const UsageCardData& u) {
  auto& d = M5.Display;
  drawCard(x, y, w, h, u.plan_bg);

  // ---- header row (avatar + email + Active pill + Plan pill) ----
  int av_r  = 22;
  int av_cx = x + 24 + av_r;
  int av_cy = y + 24 + av_r;
  drawAvatar(av_cx, av_cy, av_r, u.avatar_bg, ui::card());

  // email
  d.setFont(&fonts::FreeSansBold18pt7b);
  d.setTextDatum(middle_left);
  d.setTextColor(ui::text_hi(), ui::card());
  d.drawString(u.email, av_cx + av_r + 14, av_cy + 1);
  int email_w = d.textWidth(u.email);

  // Active pill (green)
  uint16_t active_bg = ui::rgb(0x4C, 0xC8, 0x6A);
  drawPill(av_cx + av_r + 14 + email_w + 14, av_cy - 14, 78, 28,
           "Active", active_bg, ui::rgb(0xFF, 0xFF, 0xFF),
           &fonts::FreeSansBold9pt7b);

  // Plan pill (orange/teal)
  drawPill(x + w - 24 - 78, av_cy - 14, 78, 28, u.plan_label,
           u.plan_bg, ui::rgb(0xFF, 0xFF, 0xFF), &fonts::FreeSansBold12pt7b);

  // ---- Session row ----
  uint16_t track     = ui::rgb(0x3A, 0x42, 0x4E);
  uint16_t fill_good = ui::rgb(0x4C, 0xE0, 0x7C);

  int rsx = x + 28, rex = x + w - 28;
  int row1_y = y + 92;

  d.setFont(&fonts::FreeSans12pt7b);
  d.setTextDatum(top_left);
  d.setTextColor(ui::text_md(), ui::card());
  d.drawString("Session", rsx, row1_y);

  d.setTextDatum(top_right);
  d.setTextColor(ui::text_md(), ui::card());
  d.drawString(u.session_reset, rex, row1_y);

  drawProgressBarRounded(rsx, row1_y + 28, rex - rsx, 14,
                         u.session_pct, track, fill_good);

  d.setTextDatum(top_right);
  d.setTextColor(fill_good, ui::card());
  char pct_buf[8];
  snprintf(pct_buf, sizeof(pct_buf), "%d%%", u.session_pct);
  d.drawString(pct_buf, rex, row1_y + 50);

  // ---- Weekly row ----
  int row2_y = y + 178;
  d.setTextDatum(top_left);
  d.setTextColor(ui::text_md(), ui::card());
  d.drawString("Weekly", rsx, row2_y);

  d.setTextDatum(top_right);
  d.setTextColor(ui::text_md(), ui::card());
  d.drawString(u.weekly_reset, rex, row2_y);

  drawProgressBarRounded(rsx, row2_y + 28, rex - rsx, 14,
                         u.weekly_pct, track, fill_good);

  d.setTextDatum(top_right);
  d.setTextColor(fill_good, ui::card());
  snprintf(pct_buf, sizeof(pct_buf), "%d%%", u.weekly_pct);
  d.drawString(pct_buf, rex, row2_y + 50);

  // ---- Extra usage footer ----
  int row3_y = y + h - 28;
  d.setFont(&fonts::FreeSans12pt7b);
  d.setTextDatum(middle_left);
  d.setTextColor(ui::text_lo(), ui::card());
  d.drawString("\xE2\x9C\xB1 Extra usage", rsx, row3_y);  // ✱ if available; falls back to box

  d.setTextDatum(middle_right);
  d.setTextColor(u.extra_enabled ? fill_good : ui::text_lo(), ui::card());
  d.drawString(u.extra_enabled ? "On" : "Off", rex, row3_y);
}

void drawInboxCardA(int x, int y, int w, int h) {
  auto& d = M5.Display;
  drawCard(x, y, w, h, ui::accent_inbox());
  drawTitle(x, y, w, "INBOX", "3 NEW", ui::accent_inbox());

  int row_y = y + 70;
  for (int i = 0; i < 3; ++i) {
    d.setFont(&fonts::FreeMonoBold12pt7b);
    d.setTextDatum(top_left);
    d.setTextColor(ui::accent_inbox(), ui::card());
    d.drawString(">", x + 28, row_y);
    d.setFont(&fonts::FreeSans12pt7b);
    d.setTextColor(ui::text_hi(), ui::card());
    d.drawString(kInboxItems[i], x + 60, row_y);
    d.drawFastHLine(x + 28, row_y + 50, w - 56, ui::divider());
    row_y += 64;
  }
  d.setFont(&fonts::FreeMono9pt7b);
  d.setTextDatum(bottom_right);
  d.setTextColor(ui::text_lo(), ui::card());
  d.drawString("tap any item to read", x + w - 24, y + h - 14);
}

}  // namespace

void drawLayoutA(const StatusSnapshot& /*s*/) {
  drawWeatherCardA(  0,   0, 640, 320);

  const UsageCardData claude = {
    .email          = "you*@*.com",
    .plan_label     = "Max",
    .plan_bg        = ui::rgb(0xE5, 0x7F, 0x2D),     // orange
    .avatar_bg      = ui::rgb(0xC8, 0x96, 0x66),     // tan
    .session_pct    = 39,
    .session_reset  = "3h 6m to reset",
    .weekly_pct     = 26,
    .weekly_reset   = "Mon 06:00 reset",
    .extra_enabled  = false,
  };
  drawUsageCardA(640,   0, 640, 320, claude);

  const UsageCardData codex = {
    .email          = "j**@gmail.com",
    .plan_label     = "Plus",
    .plan_bg        = ui::rgb(0x10, 0xA3, 0x7B),     // teal
    .avatar_bg      = ui::rgb(0x80, 0xE2, 0x7E),     // mint
    .session_pct    = 18,
    .session_reset  = "1h 42m to reset",
    .weekly_pct     = 15,
    .weekly_reset   = "Mon 06:00 reset",
    .extra_enabled  = true,
  };
  drawUsageCardA(  0, 320, 640, 320, codex);

  drawInboxCardA(640, 320, 640, 320);
}

// =========================================================================
// Plan B — Hero & Stack (left 800 x 640 + right 480 x 640)
// =========================================================================
void drawLayoutB(const StatusSnapshot& /*s*/) {
  auto& d = M5.Display;

  // ---- left hero: weather ----------------------------------------------
  drawCard(0, 0, 800, 640, ui::accent_wx());
  drawTitle(0, 0, 800, "WEATHER", "HALF MOON BAY", ui::accent_wx());

  d.setFont(&fonts::FreeMonoBold24pt7b);
  d.setTextDatum(middle_center);
  d.setTextColor(ui::text_hi(), ui::card());
  d.drawString("22", 380, 200);
  drawDegSymbol(440, 184, 7, ui::accent_wx(), ui::card());
  d.setFont(&fonts::FreeMonoBold18pt7b);
  d.setTextDatum(middle_left);
  d.setTextColor(ui::accent_wx(), ui::card());
  d.drawString("C", 458, 200);

  d.setFont(&fonts::FreeSans18pt7b);
  d.setTextDatum(middle_center);
  d.setTextColor(ui::accent_wx(), ui::card());
  d.drawString("Mostly sunny", 400, 290);

  d.setFont(&fonts::FreeMono12pt7b);
  d.setTextColor(ui::text_md(), ui::card());
  d.drawString("H 25    L 14", 400, 330);
  d.drawString("Wind 13 km/h W   Humidity 64%", 400, 360);

  int sx = 24, sy = 420, sw = 752, sh = 200;
  d.fillRect(sx, sy, sw, sh, ui::card_alt());
  d.drawRect(sx, sy, sw, sh, ui::border());
  int col_w = sw / 5;
  for (int i = 0; i < 5; ++i) {
    int cx = sx + i * col_w;
    d.setFont(&fonts::FreeMonoBold18pt7b);
    d.setTextDatum(top_center);
    d.setTextColor(ui::text_md(), ui::card_alt());
    d.drawString(kForecast[i].day, cx + col_w / 2, sy + 18);
    d.setFont(&fonts::FreeMono12pt7b);
    d.setTextColor(ui::text_lo(), ui::card_alt());
    d.drawString(kForecast[i].glyph, cx + col_w / 2, sy + 70);
    d.setFont(&fonts::FreeMonoBold18pt7b);
    char buf[16];
    snprintf(buf, sizeof(buf), "%d/%d", kForecast[i].hi, kForecast[i].lo);
    d.setTextColor(ui::text_hi(), ui::card_alt());
    d.drawString(buf, cx + col_w / 2, sy + 130);
    if (i > 0) d.drawFastVLine(cx, sy + 14, sh - 28, ui::border());
  }

  // ---- right rail: 3 cards ---------------------------------------------
  struct Card { int y; int h; const char* title;
                const char* big; const char* sub; uint16_t accent; };
  const Card cards[3] = {
    {   0, 213, "CLAUDE CODE", "8h 12m", "$12.40  1.24M tok",  ui::accent_claude() },
    { 213, 213, "CODEX",       "2h 03m", "$3.10   287K tok",   ui::accent_codex()  },
    { 426, 214, "INBOX",       "3 NEW",  "GPT-4o, Sonnet 4.7", ui::accent_inbox()  },
  };
  for (int i = 0; i < 3; ++i) {
    const Card& c = cards[i];
    drawCard(800, c.y, 480, c.h, c.accent);
    drawTitle(800, c.y, 480, c.title, nullptr, c.accent);

    d.setFont(&fonts::FreeMonoBold24pt7b);
    d.setTextDatum(top_left);
    d.setTextColor(ui::text_hi(), ui::card());
    d.drawString(c.big, 824, c.y + 60);

    d.setFont(&fonts::FreeMono12pt7b);
    d.setTextColor(ui::text_md(), ui::card());
    d.drawString(c.sub, 824, c.y + 150);
  }

  // sparklines on top two cards
  drawSparkline(824, 110, 432, 30, kSparkClaude, 16, ui::accent_claude());
  drawSparkline(824, 110 + 213, 432, 30, kSparkCodex, 16, ui::accent_codex());

  // for inbox card list
  d.setFont(&fonts::FreeSans12pt7b);
  d.setTextDatum(top_left);
  int yi = 426 + 100;
  for (int i = 0; i < 3 && yi < 640 - 24; ++i, yi += 32) {
    d.setTextColor(ui::accent_inbox(), ui::card());
    d.drawString(">", 824, yi);
    d.setTextColor(ui::text_hi(), ui::card());
    d.drawString(kInboxItems[i], 850, yi);
  }
}

// =========================================================================
// Plan C — Console (status bar 64 + two tiles 632x440 + ticker 120)
// =========================================================================

void drawStatusBarC(const StatusSnapshot& s) {
  auto& d = M5.Display;

  d.fillRect(0, 0, 1280, 64, ui::card_alt());
  d.drawFastHLine(0, 64, 1280, ui::border());

  d.setFont(&fonts::FreeMonoBold18pt7b);
  d.setTextDatum(middle_left);
  d.setTextColor(ui::text_hi(), ui::card_alt());
  char clock[16];
  snprintf(clock, sizeof(clock), "%02d:%02d", s.clock_hh, s.clock_mm);
  d.drawString(clock, 24, 32);

  d.setFont(&fonts::FreeMono12pt7b);
  d.setTextColor(ui::text_md(), ui::card_alt());
  char date[32];
  snprintf(date, sizeof(date), "%s  %02d/%02d",
           kDOW[s.clock_dow % 7], s.clock_month, s.clock_day);
  d.drawString(date, 140, 32);

  // Temperature + city + condition, manually composed because FreeMono
  // doesn't carry a ° glyph. Lay out left-to-right, centered around x=640.
  d.setFont(&fonts::FreeMonoBold12pt7b);
  const char* num_str = "22";
  const char* tail    = "C  HALF MOON BAY  MOSTLY SUNNY";
  const int num_w  = d.textWidth(num_str);
  const int tail_w = d.textWidth(tail);
  const int deg_w  = 14;                    // diameter + small gap
  const int total  = num_w + deg_w + tail_w;
  int       x      = 640 - total / 2;
  d.setTextDatum(middle_left);
  d.setTextColor(ui::accent_wx(), ui::card_alt());
  d.drawString(num_str, x, 32);
  drawDegSymbol(x + num_w + 7, 24, 4, ui::accent_wx(), ui::card_alt());
  d.drawString(tail, x + num_w + deg_w, 32);

  d.setFont(&fonts::FreeMono12pt7b);
  d.setTextDatum(middle_right);
  uint16_t wifi_color = s.wifi_connected ? ui::status_ok() : ui::status_bad();
  char wifi_buf[32];
  if (s.wifi_connected) snprintf(wifi_buf, sizeof(wifi_buf), "WIFI %d", s.wifi_rssi_dbm);
  else                   snprintf(wifi_buf, sizeof(wifi_buf), "WIFI ----");
  d.setTextColor(wifi_color, ui::card_alt());
  d.drawString(wifi_buf, 1130, 32);

  d.setTextColor(ui::text_hi(), ui::card_alt());
  char bat_buf[16];
  snprintf(bat_buf, sizeof(bat_buf), "BAT %d%%", s.battery_pct);
  d.drawString(bat_buf, 1255, 32);
}

void drawLayoutC(const StatusSnapshot& s) {
  auto& d = M5.Display;

  drawStatusBarC(s);

  // ---- two tiles: 632 x 440, gutter 16 ---------------------------------
  struct Tile {
    int x; const char* title; const char* today; const char* week;
    const uint8_t* spark; int quota; const char* model; uint16_t accent;
  };
  const Tile tiles[2] = {
    {   0, "CLAUDE CODE",
        "TODAY      8h 12m   $12.40",
        "THIS WEEK  28h 14m  $87.20",
        kSparkClaude, 41, "TOP MODEL  Sonnet 4.7  71% calls",
        ui::accent_claude() },
    { 648, "CODEX",
        "TODAY      2h 03m   $3.10",
        "THIS WEEK  4h 02m   $5.80",
        kSparkCodex, 16, "TOP MODEL  gpt-4.1    58% calls",
        ui::accent_codex() },
  };
  for (int i = 0; i < 2; ++i) {
    const Tile& t = tiles[i];
    drawCard(t.x, 72, 632, 440, t.accent);
    drawTitle(t.x, 72, 632, t.title, nullptr, t.accent);

    d.setFont(&fonts::FreeMonoBold18pt7b);
    d.setTextDatum(top_left);
    d.setTextColor(ui::text_hi(), ui::card());
    d.drawString(t.today, t.x + 28, 72 + 70);
    d.drawString(t.week,  t.x + 28, 72 + 110);

    drawSparkline(t.x + 28, 72 + 170, 632 - 56, 110, t.spark, 16, t.accent);

    d.setFont(&fonts::FreeMono9pt7b);
    d.setTextDatum(top_left);
    d.setTextColor(ui::text_lo(), ui::card());
    d.drawString("QUOTA TONIGHT", t.x + 28, 72 + 300);
    char qbuf[16]; snprintf(qbuf, sizeof(qbuf), "%d%%", t.quota);
    d.setTextDatum(top_right);
    d.setTextColor(ui::text_hi(), ui::card());
    d.drawString(qbuf, t.x + 632 - 28, 72 + 300);
    drawQuotaBar(t.x + 28, 72 + 320, 632 - 56, 16, t.quota, t.accent);

    d.setFont(&fonts::FreeMono9pt7b);
    d.setTextDatum(top_left);
    d.setTextColor(ui::text_md(), ui::card());
    d.drawString(t.model, t.x + 28, 72 + 360);
  }

  // ---- bottom inbox ticker 1280 x ~120 ---------------------------------
  const int ty = 520, th = 120;
  d.fillRect(0, ty, 1280, th, ui::card_alt());
  d.drawFastHLine(0, ty, 1280, ui::border());

  d.setFont(&fonts::FreeMonoBold12pt7b);
  d.setTextDatum(top_left);
  d.setTextColor(ui::accent_inbox(), ui::card_alt());
  d.drawString("INBOX", 24, ty + 18);

  d.setFont(&fonts::FreeSans12pt7b);
  d.setTextColor(ui::text_hi(), ui::card_alt());
  d.setTextDatum(top_left);
  int yy = ty + 44;
  for (int i = 0; i < 5 && yy < ty + th - 4; ++i, yy += 16) {
    char line[128];
    snprintf(line, sizeof(line), "%d.  %s", i + 1, kTickerItems[i]);
    d.drawString(line, 24, yy);
  }
}

}  // namespace ui
