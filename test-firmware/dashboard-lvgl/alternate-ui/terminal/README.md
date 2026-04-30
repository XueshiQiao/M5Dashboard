# Terminal UI Alternate

Inactive LVGL implementation for a neofetch/fastfetch-style dashboard.

This folder is intentionally outside `src/` so PlatformIO does not compile it
with the active card-grid UI. To try it, copy these two files over the active
ones in `src/`:

- `screen_grid.cpp`
- `screen_grid.h`

The terminal version uses `lv_font_unscii_16`. Before building it, enable this
in `include/lv_conf.h`:

```c
#define LV_FONT_UNSCII_16 1
```

