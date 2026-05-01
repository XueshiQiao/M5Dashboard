# fonts/

Source bitmap fonts and the build pipeline for the LVGL C font files
checked in under `firmware/src/ui/fonts/`.

## Pipeline

```
*.bdf  ──[bits-n-picas]──>  *-bnp.ttf  ──[lv_font_conv]──>  ../../src/ui/fonts/*.c
```

`bits-n-picas` (Java jar, `bitsnpicas.jar`) embeds the bitmap strikes
into a regular TTF so FreeType can rasterize them. `lv_font_conv` then
emits LVGL 9-compatible C font files at the exact pixel size we asked
for, tightly packing each glyph's ink (no zero-padded rows).

## Regenerating

```bash
java -jar bitsnpicas.jar convertbitmap -f ttf -o Tamzen8x16r-bnp.ttf  Tamzen8x16r.bdf
java -jar bitsnpicas.jar convertbitmap -f ttf -o Tamzen10x20r-bnp.ttf Tamzen10x20r.bdf
java -jar bitsnpicas.jar convertbitmap -f ttf -o Tamzen10x20b-bnp.ttf Tamzen10x20b.bdf

npx lv_font_conv@^1.5 --bpp 1 --size 16 --font Tamzen8x16r-bnp.ttf  --range 0x20-0xFF \
    --format lvgl --no-compress --lv-font-name lv_font_tamzen_16  -o ../../src/ui/fonts/lv_font_tamzen_16.c
npx lv_font_conv@^1.5 --bpp 1 --size 20 --font Tamzen10x20r-bnp.ttf --range 0x20-0xFF \
    --format lvgl --no-compress --lv-font-name lv_font_tamzen_20  -o ../../src/ui/fonts/lv_font_tamzen_20.c
npx lv_font_conv@^1.5 --bpp 1 --size 20 --font Tamzen10x20b-bnp.ttf --range 0x20-0xFF \
    --format lvgl --no-compress --lv-font-name lv_font_tamzen_20b -o ../../src/ui/fonts/lv_font_tamzen_20b.c
```

`lv_font_conv` does not accept BDF directly despite some changelog
claims; the bnp TTF round-trip is the working path.
