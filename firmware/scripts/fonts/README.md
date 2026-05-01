# fonts/

Source fonts and the build pipeline for the LVGL C font files checked
in under `firmware/src/ui/fonts/`.

## Pipelines

### A. Bitmap source (`.bdf`)

```
*.bdf  ──[bits-n-picas]──>  *-bnp.ttf  ──[lv_font_conv]──>  ../../src/ui/fonts/*.c
```

`bits-n-picas` (Java jar, `bitsnpicas.jar`) embeds the bitmap strikes
into a regular TTF so FreeType can rasterize them. Each pixel becomes
a square in the embedded outline, so **integer scale factors** stay
pixel-perfect (4× a 10×20 strike → exact 40×80; non-integer scales
produce uneven stroke widths).

`lv_font_conv` then emits LVGL 9-compatible C with per-glyph ink
trimming. **Do not feed `lv_font_conv` raw `.bdf` files** — its 1.5.x
content sniffer rejects them with `Unsupported OpenType signature
STAR`. The bnp round-trip is the working path.

### B. Vector source (`.ttf` / `.otf` / `.woff`)

```
*.ttf  ──[lv_font_conv]──>  ../../src/ui/fonts/*.c
```

Skip bnp. `lv_font_conv` rasterizes the outlines via FreeType at any
pixel size. Use `--bpp 4` for anti-aliased text (4× the storage of
bpp=1, but smoother on a high-DPI panel like the Tab5).

## Reusable recipe

For any new font, four steps:

1. **Get the source.** Drop a `.bdf` (bitmap) or `.ttf`/`.otf` (vector)
   into this directory. Bitmap fonts that work well: Tamzen, Terminus,
   Cozette, Spleen. Vector fonts that work well at small sizes: JetBrains
   Mono, IBM Plex Mono, Iosevka.

2. **Convert.** From `firmware/scripts/fonts/`:

   ```bash
   # bitmap input — round-trip through bnp first
   java -jar bitsnpicas.jar convertbitmap -f ttf -o myfont-bnp.ttf myfont.bdf

   npx lv_font_conv@^1.5 \
       --bpp 1 --size 20 \
       --font myfont-bnp.ttf \
       --range 0x20-0xFF \
       --format lvgl --no-compress \
       --lv-font-name lv_font_myfont_20 \
       -o ../../src/ui/fonts/lv_font_myfont_20.c

   # vector input — straight through lv_font_conv
   npx lv_font_conv@^1.5 \
       --bpp 4 --size 28 \
       --font JetBrainsMono-Regular.ttf \
       --range 0x20-0x7F,0xB0,0x2500-0x259F \
       --format lvgl --no-compress \
       --lv-font-name lv_font_jb_mono_28 \
       -o ../../src/ui/fonts/lv_font_jb_mono_28.c
   ```

   Useful flags:
   - `--bpp` 1, 2, or 4 — pixel depth. `1` for bitmap fonts (no AA).
     `4` for vector at small sizes (good AA, 4× the bytes vs bpp=1).
   - `--size` — the rasterized pixel height. For bitmap-source via bnp,
     **stick to integer multiples** of the BDF strike height (e.g.
     `20`, `40`, `60` for a 10×20 source).
   - `--range` — Unicode ranges to include. Common picks:
     - `0x20-0x7E` ASCII printable
     - `0x20-0xFF` ASCII + Latin-1 supplement (covers `°`, `±`, accents)
     - `0x2500-0x257F` box-drawing (`─│┌┐└┘├┤┬┴┼...`)
     - `0x2580-0x259F` block elements (`█▓▒░`)
   - `--no-compress` keeps the bitmap raw; without this, lv_font_conv
     emits RLE-compressed bitmaps (smaller flash, slower decode).
   - `--lv-font-name` — the C symbol name. Match the file name.

3. **Patch the include.** lv_font_conv emits `#include "lvgl/lvgl.h"`
   which doesn't resolve under our PlatformIO library layout. Fix:

   ```bash
   sed -i '' 's|#include "lvgl/lvgl.h"|#include "lvgl.h"|' \
       ../../src/ui/fonts/lv_font_myfont_20.c
   ```

4. **Wire it into the firmware.**

   - Declare in `firmware/src/ui/fonts/fonts.h`:

     ```c
     LV_FONT_DECLARE(lv_font_myfont_20);
     ```

   - Use in a layout (e.g. `terminal_layout.cpp`):

     ```cpp
     g_font_body = lv_font_myfont_20;          // copy into a writable struct
     g_font_body.fallback = &lv_font_unscii_16; // optional fallback chain
     // … then mkLabel(... kFont ...) where kFont = &g_font_body
     ```

   The fallback chain is useful when your primary font lacks specific
   glyphs (e.g. Tamzen has no box-drawing → chain to unscii-16). LVGL
   walks `font->fallback` on missing glyph; `nullptr` ends the chain.

5. **Rebuild and verify on device.** `pio run --target upload` then
   capture serial + a photo (CLAUDE.md describes the DTR/RTS pulse and
   pyserial reader).

## Currently checked in

| Symbol                | Source                    | Size  | Use                  |
| --------------------- | ------------------------- | ----- | -------------------- |
| `lv_font_tamzen_16`   | `Tamzen8x16r.bdf`         | 8×16  | (unused)             |
| `lv_font_tamzen_20`   | `Tamzen10x20r.bdf`        | 10×20 | (unused)             |
| `lv_font_tamzen_20b`  | `Tamzen10x20b.bdf`        | 10×20 | (unused)             |
| `lv_font_tamzen_40b`  | `Tamzen10x20b.bdf` × 2    | 20×40 | terminal layout body |

The unused entries stay so the layout can swap fonts without a
regenerate cycle.

## Sources

- **Tamzen** — <https://github.com/sunaku/tamzen-font> (MIT). BDFs at
  5×9, 6×12, 7×13, 7×14, 8×15, 8×16, 10×20 in regular and bold.
- **bits-n-picas** — <https://github.com/kreativekorp/bitsnpicas>
  (MIT). The `BitsNPicas.jar` checked into this directory is the
  reference build.
- **lv_font_conv** — <https://github.com/lvgl/lv_font_conv> (MIT).
  Run via `npx`; no global install needed.

## Why not `lv_font_conv` directly on BDF?

The 1.5.3 changelog claims BDF support but the parser doesn't actually
match the BDF magic. Feeding a `.bdf` file produces:

```
Cannot load font "X.bdf": Unsupported OpenType signature STAR
```

(it sniffs the first bytes "STARTFONT…" and treats them as an
unknown OpenType signature). The bnp round-trip is the workaround
until upstream fixes it.

## Why not write our own BDF→LVGL converter?

Tried it (`bdf2lvgl.py`, since deleted). Subtle bugs around
`glyph_id_ofs_list` element width, `ofs_y` reference frame, and ink
trimming kept producing rendering that looked plausible until you read
it carefully. Three rounds of "looks better, still wrong" before
switching to canonical tools. **Use the canonical pipeline.**
