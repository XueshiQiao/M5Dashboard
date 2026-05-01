#!/usr/bin/env python3
# bdf2lvgl.py — convert a 1-bpp BDF bitmap font into an LVGL 9 C font file.
#
# Usage:
#   python3 bdf2lvgl.py Tamzen8x16r.bdf lv_font_tamzen_16 \
#       --out lv_font_tamzen_16.c
#
# Emits a C file with:
#   const lv_font_t <name> = { ... };
# The font uses bpp=1 and a single FORMAT0_TINY cmap range, which fits
# Tamzen's contiguous Latin-1 (0x20-0xFF) range. Glyphs outside that
# range fall back via lv_font_t.fallback at runtime.

import argparse
import re
import sys
from dataclasses import dataclass


@dataclass
class Glyph:
    code: int
    dwidth: int
    bbw: int
    bbh: int
    bbx: int
    bby: int
    rows: list  # list[int] left-aligned in the BBX width


def parse_bdf(path):
    glyphs = []
    bbx = (0, 0, 0, 0)
    ascent = None
    descent = None

    with open(path, "r", encoding="utf-8", errors="replace") as f:
        line_iter = iter(f)
        for line in line_iter:
            line = line.rstrip("\n")
            if line.startswith("FONTBOUNDINGBOX"):
                parts = line.split()
                bbx = tuple(int(x) for x in parts[1:5])
            elif line.startswith("FONT_ASCENT"):
                ascent = int(line.split()[1])
            elif line.startswith("FONT_DESCENT"):
                descent = int(line.split()[1])
            elif line.startswith("STARTCHAR"):
                code, dwidth = -1, bbx[0]
                gbbw, gbbh, gbbx, gbby = bbx
                rows = []
                for sub in line_iter:
                    sub = sub.rstrip("\n")
                    if sub.startswith("ENCODING"):
                        code = int(sub.split()[1])
                    elif sub.startswith("DWIDTH"):
                        dwidth = int(sub.split()[1])
                    elif sub.startswith("BBX"):
                        parts = sub.split()
                        gbbw, gbbh, gbbx, gbby = (int(p) for p in parts[1:5])
                    elif sub.startswith("BITMAP"):
                        rows = []
                        for hex_row in line_iter:
                            hex_row = hex_row.strip()
                            if hex_row == "ENDCHAR":
                                break
                            # Each hex row encodes one row of the BBX, MSB-aligned
                            # to the byte width. BBX width may not align to byte.
                            byte_count = (gbbw + 7) // 8
                            value = int(hex_row, 16) if hex_row else 0
                            # Left-justify the bits within (byte_count * 8) cells.
                            value <<= (byte_count * 8 - gbbw)
                            rows.append((value, byte_count))
                        break
                if code >= 0:
                    glyphs.append(Glyph(code, dwidth, gbbw, gbbh, gbbx, gbby, rows))

    return glyphs, ascent, descent, bbx


def pack_bitmap_bpp1(g, line_height, ascent):
    """Return a bytes() left-justified row-major bitmap for one glyph,
    padded to the byte boundary at the right edge. LVGL bpp=1 expects
    one bit per pixel, MSB first, rows packed end-to-end."""
    if g.bbw == 0 or g.bbh == 0:
        return b""
    bits_per_row = g.bbw
    out = bytearray()
    for value, byte_count in g.rows:
        # value's MSB is the leftmost pixel of the row, but it spans
        # byte_count*8 bits. Re-pack to bits_per_row bits, MSB-first.
        row_bytes = bytearray()
        bit_acc = 0
        bit_count = 0
        # Walk the value's high bits leftmost to bits_per_row.
        for i in range(bits_per_row):
            bit = (value >> (byte_count * 8 - 1 - i)) & 1
            bit_acc = (bit_acc << 1) | bit
            bit_count += 1
            if bit_count == 8:
                row_bytes.append(bit_acc & 0xFF)
                bit_acc = 0
                bit_count = 0
        if bit_count:
            row_bytes.append((bit_acc << (8 - bit_count)) & 0xFF)
        out.extend(row_bytes)
    return bytes(out)


def emit(c_path, font_name, glyphs, ascent, descent, bbx_h):
    """Write the LVGL C font file."""
    line_height = ascent + descent if ascent and descent else bbx_h

    # Sort by codepoint, find contiguous range.
    glyphs = sorted(glyphs, key=lambda g: g.code)
    if not glyphs:
        sys.exit("no glyphs found")
    cmin = glyphs[0].code
    cmax = glyphs[-1].code

    # Build index by code, fill gaps with empty glyph (id 0 = space-ish missing).
    by_code = {g.code: g for g in glyphs}

    # LVGL convention: glyph_dsc[0] is the "missing-glyph" sentinel.
    # We emit it as a zero-width zero-height bitmap.
    descs = [{"bitmap_index": 0, "adv_w": 0, "box_w": 0,
              "box_h": 0, "ofs_x": 0, "ofs_y": 0}]
    bitmap = bytearray()
    glyph_id_map = {}  # code -> glyph_id

    for code in range(cmin, cmax + 1):
        if code not in by_code:
            glyph_id_map[code] = 0  # missing glyph
            continue
        g = by_code[code]
        bm = pack_bitmap_bpp1(g, line_height, ascent or bbx_h)
        descs.append({
            "bitmap_index": len(bitmap),
            "adv_w": g.dwidth * 16,  # 4-bit fractional × 16 (we use whole px)
            "box_w": g.bbw,
            "box_h": g.bbh,
            "ofs_x": g.bbx,
            "ofs_y": g.bby,
        })
        bitmap.extend(bm)
        glyph_id_map[code] = len(descs) - 1

    # Emit C.
    out = []
    out.append("// Generated by bdf2lvgl.py — DO NOT EDIT BY HAND.")
    out.append("// Converted from a BDF bitmap font for LVGL 9 (bpp=1).")
    out.append("")
    out.append("#include \"lvgl.h\"")
    out.append("")

    # bitmap
    out.append("static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {")
    for i in range(0, len(bitmap), 16):
        chunk = bitmap[i:i + 16]
        out.append("    " + ", ".join(f"0x{b:02X}" for b in chunk) + ",")
    out.append("};")
    out.append("")

    # glyph descriptors
    out.append("static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {")
    for d in descs:
        out.append(
            "    {{.bitmap_index = {bitmap_index}, .adv_w = {adv_w}, "
            ".box_w = {box_w}, .box_h = {box_h}, .ofs_x = {ofs_x}, .ofs_y = {ofs_y}}},"
            .format(**d)
        )
    out.append("};")
    out.append("")

    # cmap: FORMAT0_FULL — covers the contiguous range_start..range_end
    # with a uint8_t[] of (glyph_id - glyph_id_start). LVGL adds glyph_id_start
    # back at lookup time. Gaps in the range emit 0; LVGL treats 0-on-non-first
    # as "missing glyph" per the special case in lv_font_fmt_txt.c.
    glyph_id_start = 1
    out.append("static const uint8_t glyph_id_ofs_list[] = {")
    for code in range(cmin, cmax + 1):
        gid = glyph_id_map[code]
        ofs = (gid - glyph_id_start) if gid != 0 else 0
        if ofs < 0 or ofs > 255:
            sys.exit(f"glyph_id offset {ofs} out of range for uint8_t at U+{code:04X}")
        out.append(f"    {ofs},")
    out.append("};")
    out.append("")

    out.append("static const lv_font_fmt_txt_cmap_t cmaps[] = {")
    out.append("    {")
    out.append(f"        .range_start = 0x{cmin:04X},")
    out.append(f"        .range_length = {cmax - cmin + 1},")
    out.append(f"        .glyph_id_start = {glyph_id_start},")
    out.append("        .unicode_list = NULL,")
    out.append("        .glyph_id_ofs_list = glyph_id_ofs_list,")
    out.append(f"        .list_length = {cmax - cmin + 1},")
    out.append("        .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL,")
    out.append("    }")
    out.append("};")
    out.append("")

    # Font dsc.
    out.append("static const lv_font_fmt_txt_dsc_t font_dsc = {")
    out.append("    .glyph_bitmap = glyph_bitmap,")
    out.append("    .glyph_dsc = glyph_dsc,")
    out.append("    .cmaps = cmaps,")
    out.append("    .kern_dsc = NULL,")
    out.append("    .kern_scale = 0,")
    out.append("    .cmap_num = 1,")
    out.append("    .bpp = 1,")
    out.append("    .kern_classes = 0,")
    out.append("    .bitmap_format = 0,")
    out.append("};")
    out.append("")

    # Public lv_font_t.
    base_line = descent if descent is not None else 0
    out.append(f"const lv_font_t {font_name} = {{")
    out.append("    .get_glyph_dsc    = lv_font_get_glyph_dsc_fmt_txt,")
    out.append("    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,")
    out.append(f"    .line_height = {line_height},")
    out.append(f"    .base_line   = {base_line},")
    out.append("    .subpx = LV_FONT_SUBPX_NONE,")
    out.append("    .underline_position  = -2,")
    out.append("    .underline_thickness = 1,")
    out.append("    .dsc = &font_dsc,")
    out.append("    .fallback = NULL,")
    out.append("    .user_data = NULL,")
    out.append("};")
    out.append("")

    with open(c_path, "w", encoding="utf-8") as f:
        f.write("\n".join(out))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("bdf")
    ap.add_argument("name", help="C symbol name, e.g. lv_font_tamzen_16")
    ap.add_argument("--out", required=True)
    args = ap.parse_args()

    glyphs, ascent, descent, bbx = parse_bdf(args.bdf)
    print(f"parsed {len(glyphs)} glyphs from {args.bdf}; "
          f"ascent={ascent} descent={descent} bbx={bbx}")
    emit(args.out, args.name, glyphs, ascent, descent, bbx[1])
    print(f"wrote {args.out}")


if __name__ == "__main__":
    main()
