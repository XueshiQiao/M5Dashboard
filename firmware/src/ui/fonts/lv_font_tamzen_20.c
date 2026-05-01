/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: --bpp 1 --size 20 --font Tamzen10x20r-bnp.ttf --range 0x20-0xFF --format lvgl --no-compress --lv-font-name lv_font_tamzen_20 -o /Users/joey/Code/M5Dashboard/firmware/src/ui/fonts/lv_font_tamzen_20.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_FONT_TAMZEN_20
#define LV_FONT_TAMZEN_20 1
#endif

#if LV_FONT_TAMZEN_20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc, 0x60,

    /* U+0022 "\"" */
    0x8c, 0x63, 0x10,

    /* U+0023 "#" */
    0x11, 0x8, 0x84, 0x4f, 0xf2, 0x21, 0x10, 0x88,
    0x44, 0xff, 0x22, 0x11, 0x8, 0x80,

    /* U+0024 "$" */
    0x10, 0x20, 0x43, 0xe8, 0x30, 0x20, 0x3e, 0x2,
    0x6, 0xb, 0xe1, 0x2, 0x4, 0x0,

    /* U+0025 "%" */
    0x60, 0x91, 0x92, 0x64, 0x8, 0x10, 0x26, 0x49,
    0x89, 0x6,

    /* U+0026 "&" */
    0x60, 0x90, 0x90, 0x90, 0x60, 0x60, 0x92, 0x8a,
    0x84, 0x8a, 0x71,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x12, 0x44, 0x88, 0x88, 0x88, 0x44, 0x21,

    /* U+0029 ")" */
    0x84, 0x22, 0x11, 0x11, 0x11, 0x22, 0x48,

    /* U+002A "*" */
    0x11, 0x25, 0x51, 0xc5, 0x52, 0x44, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0x6c, 0x9c,

    /* U+002D "-" */
    0xfe,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x2, 0x4, 0x10, 0x20, 0x81, 0x4, 0x8, 0x20,
    0x41, 0x2, 0x8, 0x10, 0x0,

    /* U+0030 "0" */
    0x7d, 0x6, 0x1c, 0x59, 0x34, 0x70, 0xc1, 0x82,
    0xf8,

    /* U+0031 "1" */
    0x10, 0x61, 0x44, 0x81, 0x2, 0x4, 0x8, 0x11,
    0xfc,

    /* U+0032 "2" */
    0x7d, 0x4, 0x8, 0x10, 0x41, 0x4, 0x10, 0x41,
    0xfc,

    /* U+0033 "3" */
    0xfe, 0x8, 0x20, 0x83, 0xc0, 0x40, 0x81, 0x82,
    0xf8,

    /* U+0034 "4" */
    0x4, 0x18, 0x51, 0x24, 0x50, 0xbf, 0x82, 0x4,
    0x8,

    /* U+0035 "5" */
    0xff, 0x2, 0x4, 0xf, 0xc0, 0x40, 0x81, 0x82,
    0xf8,

    /* U+0036 "6" */
    0x38, 0x82, 0x4, 0xf, 0xd0, 0x60, 0xc1, 0x82,
    0xf8,

    /* U+0037 "7" */
    0xfe, 0x4, 0x10, 0x20, 0x81, 0x4, 0x8, 0x20,
    0x40,

    /* U+0038 "8" */
    0x7d, 0x6, 0xc, 0x17, 0xd0, 0x60, 0xc1, 0x82,
    0xf8,

    /* U+0039 "9" */
    0x7d, 0x6, 0xc, 0x18, 0x2f, 0xc0, 0x81, 0x4,
    0x70,

    /* U+003A ":" */
    0xf0, 0xf,

    /* U+003B ";" */
    0x6c, 0x0, 0x1b, 0x27, 0x0,

    /* U+003C "<" */
    0x8, 0x88, 0x88, 0x20, 0x82, 0x8,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0x82, 0x8, 0x20, 0x88, 0x88, 0x80,

    /* U+003F "?" */
    0x7d, 0x4, 0x8, 0x20, 0x82, 0x0, 0x0, 0x0,
    0x20, 0x40,

    /* U+0040 "@" */
    0x1e, 0x10, 0x90, 0x30, 0x18, 0xec, 0x96, 0x4b,
    0x25, 0x92, 0xc6, 0xa0, 0x8, 0x2, 0x0, 0xf8,

    /* U+0041 "A" */
    0x38, 0x8a, 0xc, 0x18, 0x3f, 0xe0, 0xc1, 0x83,
    0x4,

    /* U+0042 "B" */
    0xfd, 0x6, 0xc, 0x1f, 0xd0, 0x60, 0xc1, 0x83,
    0xf8,

    /* U+0043 "C" */
    0x3e, 0x82, 0x4, 0x8, 0x10, 0x20, 0x40, 0x40,
    0x7c,

    /* U+0044 "D" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x85,
    0xf0,

    /* U+0045 "E" */
    0xff, 0x2, 0x4, 0xf, 0xd0, 0x20, 0x40, 0x81,
    0xfc,

    /* U+0046 "F" */
    0xff, 0x2, 0x4, 0xf, 0xd0, 0x20, 0x40, 0x81,
    0x0,

    /* U+0047 "G" */
    0x3e, 0x82, 0x4, 0x8, 0x11, 0xe0, 0xc1, 0x42,
    0x7c,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1, 0x83,
    0x4,

    /* U+0049 "I" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x11,
    0xfc,

    /* U+004A "J" */
    0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0xc1, 0x82,
    0xf8,

    /* U+004B "K" */
    0x83, 0xa, 0x24, 0x8e, 0x1c, 0x24, 0x44, 0x85,
    0x4,

    /* U+004C "L" */
    0x81, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81,
    0xfc,

    /* U+004D "M" */
    0x83, 0x8e, 0xac, 0x99, 0x30, 0x60, 0xc1, 0x83,
    0x4,

    /* U+004E "N" */
    0x83, 0x86, 0x8c, 0x98, 0xb0, 0xe0, 0xc1, 0x83,
    0x4,

    /* U+004F "O" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x42, 0x3c,

    /* U+0050 "P" */
    0xfd, 0x6, 0xc, 0x18, 0x3f, 0xa0, 0x40, 0x81,
    0x0,

    /* U+0051 "Q" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x42, 0x3c, 0x8, 0x7,

    /* U+0052 "R" */
    0xfd, 0x6, 0xc, 0x18, 0x3f, 0xa4, 0x44, 0x85,
    0x4,

    /* U+0053 "S" */
    0x7d, 0x6, 0x4, 0x7, 0x1, 0x80, 0x81, 0x82,
    0xf8,

    /* U+0054 "T" */
    0xff, 0x84, 0x2, 0x1, 0x0, 0x80, 0x40, 0x20,
    0x10, 0x8, 0x4, 0x0,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x82,
    0xf8,

    /* U+0056 "V" */
    0x83, 0x6, 0xc, 0x14, 0x48, 0x8a, 0x14, 0x10,
    0x20,

    /* U+0057 "W" */
    0x83, 0x6, 0xc, 0x18, 0x32, 0x64, 0xc9, 0xab,
    0x8c,

    /* U+0058 "X" */
    0x83, 0x5, 0x11, 0x41, 0x2, 0xa, 0x22, 0x83,
    0x4,

    /* U+0059 "Y" */
    0x82, 0x89, 0x11, 0x42, 0x82, 0x4, 0x8, 0x10,
    0x20,

    /* U+005A "Z" */
    0xfe, 0x8, 0x20, 0x41, 0x2, 0x8, 0x10, 0x41,
    0xfc,

    /* U+005B "[" */
    0xfc, 0x21, 0x8, 0x42, 0x10, 0x84, 0x21, 0x8,
    0x7c,

    /* U+005C "\\" */
    0x81, 0x1, 0x2, 0x2, 0x4, 0x4, 0x8, 0x8,
    0x10, 0x10, 0x20, 0x20, 0x40,

    /* U+005D "]" */
    0xf8, 0x42, 0x10, 0x84, 0x21, 0x8, 0x42, 0x10,
    0xfc,

    /* U+005E "^" */
    0x10, 0x51, 0x14, 0x10,

    /* U+005F "_" */
    0xff, 0xc0,

    /* U+0060 "`" */
    0x84, 0x21,

    /* U+0061 "a" */
    0x7c, 0x4, 0xb, 0xf8, 0x30, 0x61, 0xbd,

    /* U+0062 "b" */
    0x81, 0x2, 0x5, 0xcc, 0x50, 0x60, 0xc1, 0x83,
    0xb, 0xe0,

    /* U+0063 "c" */
    0x3c, 0x86, 0x4, 0x8, 0x10, 0x10, 0x9e,

    /* U+0064 "d" */
    0x2, 0x4, 0x9, 0xf4, 0x30, 0x60, 0xc1, 0x82,
    0x8c, 0xe8,

    /* U+0065 "e" */
    0x3c, 0x86, 0xf, 0xf8, 0x10, 0x10, 0x1f,

    /* U+0066 "f" */
    0x1e, 0x40, 0x81, 0xf, 0xe4, 0x8, 0x10, 0x20,
    0x40, 0x81, 0x0,

    /* U+0067 "g" */
    0x39, 0x46, 0x82, 0x82, 0x44, 0x38, 0x40, 0x7c,
    0x82, 0x82, 0x7c,

    /* U+0068 "h" */
    0x81, 0x2, 0x4, 0xea, 0x38, 0x60, 0xc1, 0x83,
    0x6, 0x8,

    /* U+0069 "i" */
    0x10, 0x20, 0x0, 0xf, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x47, 0xf0,

    /* U+006A "j" */
    0x8, 0x40, 0x7, 0x84, 0x21, 0x8, 0x42, 0x10,
    0x8b, 0x80,

    /* U+006B "k" */
    0x81, 0x2, 0x4, 0x8, 0x51, 0x24, 0x50, 0xd1,
    0x12, 0x14, 0x10,

    /* U+006C "l" */
    0xf0, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x38,

    /* U+006D "m" */
    0xb3, 0x66, 0x62, 0x31, 0x18, 0x8c, 0x46, 0x23,
    0x11,

    /* U+006E "n" */
    0x9d, 0x47, 0xc, 0x18, 0x30, 0x60, 0xc1,

    /* U+006F "o" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x51, 0x1c,

    /* U+0070 "p" */
    0xb9, 0x8a, 0xc, 0x18, 0x30, 0x61, 0x7c, 0x81,
    0x2, 0x0,

    /* U+0071 "q" */
    0x3e, 0x86, 0xc, 0x18, 0x30, 0x51, 0x9d, 0x2,
    0x4, 0x8,

    /* U+0072 "r" */
    0x9d, 0x47, 0x4, 0x8, 0x10, 0x20, 0x40,

    /* U+0073 "s" */
    0x7d, 0x6, 0x3, 0x80, 0xc0, 0x60, 0xbe,

    /* U+0074 "t" */
    0x20, 0x43, 0xf9, 0x2, 0x4, 0x8, 0x10, 0x20,
    0x3c,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0xe2, 0xb9,

    /* U+0076 "v" */
    0x83, 0x5, 0x12, 0x22, 0x85, 0x4, 0x8,

    /* U+0077 "w" */
    0x80, 0xc0, 0x62, 0x31, 0x18, 0x8c, 0x46, 0x22,
    0xee,

    /* U+0078 "x" */
    0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81,

    /* U+0079 "y" */
    0x83, 0x6, 0xc, 0x18, 0x28, 0xce, 0x81, 0x2,
    0x9, 0xe0,

    /* U+007A "z" */
    0xfe, 0x8, 0x20, 0x82, 0x8, 0x20, 0x7f,

    /* U+007B "{" */
    0x7, 0x8, 0x8, 0x8, 0x8, 0x8, 0x10, 0xe0,
    0x10, 0x8, 0x8, 0x8, 0x8, 0x8, 0x7,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0xe0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x8, 0x7,
    0x8, 0x10, 0x10, 0x10, 0x10, 0x10, 0xe0,

    /* U+007E "~" */
    0x61, 0x91, 0x89, 0x86,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xc7, 0xe0,

    /* U+00A2 "¢" */
    0x10, 0x20, 0xf2, 0x99, 0x12, 0x24, 0x48, 0x52,
    0x78, 0x40, 0x80,

    /* U+00A3 "£" */
    0x1c, 0x44, 0x81, 0xf, 0xc4, 0x8, 0x10, 0x73,
    0x18,

    /* U+00A4 "¤" */
    0x80, 0xae, 0x88, 0x88, 0x24, 0x12, 0x8, 0x88,
    0xba, 0x80, 0x80,

    /* U+00A5 "¥" */
    0x82, 0x88, 0xa0, 0x87, 0xc2, 0x4, 0x3e, 0x10,
    0x20,

    /* U+00A6 "¦" */
    0xf8, 0xf8,

    /* U+00A8 "¨" */
    0x8c, 0x40,

    /* U+00A9 "©" */
    0x3e, 0x20, 0xa3, 0x32, 0x19, 0xc, 0x86, 0x32,
    0x82, 0x3e, 0x0,

    /* U+00AB "«" */
    0x11, 0x22, 0x44, 0x88, 0x44, 0x22, 0x11,

    /* U+00AD "­" */
    0xfe,

    /* U+00B0 "°" */
    0x74, 0x63, 0x17, 0x0,

    /* U+00B4 "´" */
    0x12, 0x48,

    /* U+00B8 "¸" */
    0x24, 0xe0,

    /* U+00BB "»" */
    0x88, 0x44, 0x22, 0x11, 0x22, 0x44, 0x88,

    /* U+00BF "¿" */
    0x10, 0x20, 0x0, 0x0, 0x2, 0x8, 0x20, 0x81,
    0x5, 0xf0,

    /* U+00C0 "À" */
    0x40, 0x40, 0x40, 0x1, 0x2, 0xa, 0x14, 0x44,
    0x89, 0xf4, 0x18, 0x30, 0x40,

    /* U+00C1 "Á" */
    0x4, 0x10, 0x40, 0x1, 0x2, 0xa, 0x14, 0x44,
    0x89, 0xf4, 0x18, 0x30, 0x40,

    /* U+00C2 "Â" */
    0x10, 0x51, 0x10, 0x1, 0x2, 0xa, 0x14, 0x44,
    0x89, 0xf4, 0x18, 0x30, 0x40,

    /* U+00C3 "Ã" */
    0x63, 0x26, 0x30, 0x1, 0x2, 0xa, 0x14, 0x44,
    0x89, 0xf4, 0x18, 0x30, 0x40,

    /* U+00C4 "Ä" */
    0x44, 0x88, 0x0, 0x81, 0x5, 0xa, 0x22, 0x44,
    0xfa, 0xc, 0x18, 0x20,

    /* U+00C5 "Å" */
    0x10, 0x50, 0xa0, 0x81, 0x5, 0xa, 0x22, 0x44,
    0xfa, 0xc, 0x18, 0x20,

    /* U+00C6 "Æ" */
    0x1f, 0x18, 0x28, 0x28, 0x4f, 0x48, 0x78, 0x88,
    0x88, 0x8f,

    /* U+00C7 "Ç" */
    0x3e, 0x82, 0x4, 0x8, 0x10, 0x20, 0x40, 0x40,
    0x7c, 0x20, 0x40, 0x86, 0x0,

    /* U+00C8 "È" */
    0x40, 0x40, 0x40, 0xf, 0xf0, 0x20, 0x40, 0xfd,
    0x2, 0x4, 0x8, 0x1f, 0xc0,

    /* U+00C9 "É" */
    0x4, 0x10, 0x40, 0xf, 0xf0, 0x20, 0x40, 0xfd,
    0x2, 0x4, 0x8, 0x1f, 0xc0,

    /* U+00CA "Ê" */
    0x10, 0x51, 0x10, 0xf, 0xf0, 0x20, 0x40, 0xfd,
    0x2, 0x4, 0x8, 0x1f, 0xc0,

    /* U+00CB "Ë" */
    0x44, 0x88, 0x7, 0xf8, 0x10, 0x20, 0x7e, 0x81,
    0x2, 0x4, 0xf, 0xe0,

    /* U+00CC "Ì" */
    0x40, 0x40, 0x40, 0xf, 0xe2, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x81, 0x1f, 0xc0,

    /* U+00CD "Í" */
    0x4, 0x10, 0x40, 0xf, 0xe2, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x81, 0x1f, 0xc0,

    /* U+00CE "Î" */
    0x10, 0x51, 0x10, 0xf, 0xe2, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x81, 0x1f, 0xc0,

    /* U+00CF "Ï" */
    0x44, 0x88, 0x7, 0xf1, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x8f, 0xe0,

    /* U+00D0 "Ð" */
    0x7c, 0x42, 0x41, 0x41, 0xf1, 0x41, 0x41, 0x41,
    0x42, 0x7c,

    /* U+00D1 "Ñ" */
    0x63, 0x26, 0x30, 0x8, 0x38, 0x68, 0xc9, 0x8b,
    0xe, 0xc, 0x18, 0x30, 0x40,

    /* U+00D2 "Ò" */
    0x40, 0x40, 0x40, 0x3, 0x88, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00D3 "Ó" */
    0x4, 0x10, 0x40, 0x3, 0x88, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00D4 "Ô" */
    0x10, 0x51, 0x10, 0x3, 0x88, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00D5 "Õ" */
    0x63, 0x26, 0x30, 0x3, 0x88, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00D6 "Ö" */
    0x44, 0x88, 0x0, 0x3, 0x88, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00D7 "×" */
    0x85, 0x23, 0xc, 0x4a, 0x10,

    /* U+00D8 "Ø" */
    0x4, 0x79, 0x14, 0x58, 0xb2, 0x64, 0xd1, 0xa2,
    0x89, 0xe2, 0x0,

    /* U+00D9 "Ù" */
    0x40, 0x40, 0x40, 0x8, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00DA "Ú" */
    0x4, 0x10, 0x40, 0x8, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00DB "Û" */
    0x10, 0x51, 0x10, 0x8, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x14, 0x47, 0x0,

    /* U+00DC "Ü" */
    0x44, 0x88, 0x4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xa, 0x23, 0x80,

    /* U+00DD "Ý" */
    0x4, 0x10, 0x40, 0x8, 0x28, 0x91, 0x14, 0x28,
    0x20, 0x40, 0x81, 0x2, 0x0,

    /* U+00DE "Þ" */
    0x81, 0x3, 0xf4, 0x18, 0x30, 0x60, 0xfe, 0x81,
    0x0,

    /* U+00DF "ß" */
    0x3c, 0x86, 0xc, 0x28, 0xd0, 0x60, 0xc1, 0x85,
    0x30,

    /* U+00E0 "à" */
    0x40, 0x40, 0x40, 0x0, 0xf, 0x80, 0x81, 0x7f,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00E1 "á" */
    0x4, 0x10, 0x40, 0x0, 0xf, 0x80, 0x81, 0x7f,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00E2 "â" */
    0x10, 0x51, 0x10, 0x0, 0xf, 0x80, 0x81, 0x7f,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00E3 "ã" */
    0x63, 0x26, 0x30, 0x0, 0xf, 0x80, 0x81, 0x7f,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00E4 "ä" */
    0x44, 0x88, 0x0, 0x7, 0xc0, 0x40, 0xbf, 0x83,
    0x6, 0xb, 0xf0,

    /* U+00E5 "å" */
    0x18, 0x48, 0x90, 0xc0, 0xf, 0x80, 0x81, 0x7f,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00E6 "æ" */
    0x77, 0x4, 0x42, 0x2f, 0xf8, 0x84, 0x42, 0x20,
    0xef,

    /* U+00E7 "ç" */
    0x7d, 0x6, 0x4, 0x8, 0x10, 0x20, 0xbe, 0x8,
    0x10, 0x21, 0x80,

    /* U+00E8 "è" */
    0x40, 0x40, 0x40, 0x0, 0xf, 0xa0, 0xc1, 0xff,
    0x2, 0x4, 0x7, 0xe0,

    /* U+00E9 "é" */
    0x2, 0x8, 0x20, 0x0, 0xf, 0xa0, 0xc1, 0xff,
    0x2, 0x4, 0x7, 0xe0,

    /* U+00EA "ê" */
    0x10, 0x51, 0x10, 0x0, 0xf, 0xa0, 0xc1, 0xff,
    0x2, 0x4, 0x7, 0xe0,

    /* U+00EB "ë" */
    0x44, 0x88, 0x0, 0x7, 0xd0, 0x60, 0xff, 0x81,
    0x2, 0x3, 0xf0,

    /* U+00EC "ì" */
    0x40, 0x40, 0x40, 0x0, 0x1e, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x8f, 0xe0,

    /* U+00ED "í" */
    0x4, 0x10, 0x40, 0x0, 0x1e, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x8f, 0xe0,

    /* U+00EE "î" */
    0x10, 0x51, 0x10, 0x0, 0x1e, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x8f, 0xe0,

    /* U+00EF "ï" */
    0x44, 0x88, 0x0, 0xf, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x47, 0xf0,

    /* U+00F0 "ð" */
    0x12, 0x18, 0xd0, 0x13, 0xa8, 0xe0, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+00F1 "ñ" */
    0x63, 0x26, 0x30, 0x0, 0x1f, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x18, 0x20,

    /* U+00F2 "ò" */
    0x40, 0x40, 0x40, 0x0, 0xf, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+00F3 "ó" */
    0x4, 0x10, 0x40, 0x0, 0xf, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+00F4 "ô" */
    0x10, 0x51, 0x10, 0x0, 0xf, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+00F5 "õ" */
    0x63, 0x26, 0x30, 0x0, 0xf, 0xa0, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xc0,

    /* U+00F6 "ö" */
    0x44, 0x88, 0x0, 0x7, 0xd0, 0x60, 0xc1, 0x83,
    0x6, 0xb, 0xe0,

    /* U+00F7 "÷" */
    0x10, 0x20, 0x0, 0xf, 0xe0, 0x0, 0x8, 0x10,

    /* U+00F8 "ø" */
    0x4, 0xfa, 0x2c, 0x59, 0x32, 0x68, 0xd1, 0x7c,
    0x80,

    /* U+00F9 "ù" */
    0x40, 0x40, 0x40, 0x0, 0x10, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00FA "ú" */
    0x4, 0x10, 0x40, 0x0, 0x10, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00FB "û" */
    0x10, 0x51, 0x10, 0x0, 0x10, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xe0,

    /* U+00FC "ü" */
    0x44, 0x88, 0x0, 0x8, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xb, 0xf0,

    /* U+00FD "ý" */
    0x4, 0x10, 0x40, 0x0, 0x10, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x17, 0xe0, 0x40, 0x81, 0x7c,

    /* U+00FE "þ" */
    0x81, 0x2, 0x5, 0xcc, 0x50, 0x60, 0xc1, 0x83,
    0x8a, 0xe4, 0x8, 0x10, 0x0,

    /* U+00FF "ÿ" */
    0x44, 0x88, 0x0, 0x8, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xb, 0xf0, 0x20, 0x40, 0xbe
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 160, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 160, .box_w = 1, .box_h = 11, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 160, .box_w = 5, .box_h = 4, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 6, .adv_w = 160, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 160, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 34, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 160, .box_w = 1, .box_h = 4, .ofs_x = 4, .ofs_y = 8},
    {.bitmap_index = 56, .adv_w = 160, .box_w = 4, .box_h = 14, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 63, .adv_w = 160, .box_w = 4, .box_h = 14, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 70, .adv_w = 160, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 77, .adv_w = 160, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 84, .adv_w = 160, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 86, .adv_w = 160, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 87, .adv_w = 160, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 101, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 160, .box_w = 2, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 160, .box_w = 3, .box_h = 11, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 198, .adv_w = 160, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 204, .adv_w = 160, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 209, .adv_w = 160, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 215, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 160, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 241, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 304, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 313, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 349, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 386, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 398, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 416, .adv_w = 160, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 437, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 455, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 464, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 482, .adv_w = 160, .box_w = 5, .box_h = 14, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 491, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 504, .adv_w = 160, .box_w = 5, .box_h = 14, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 513, .adv_w = 160, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 517, .adv_w = 160, .box_w = 10, .box_h = 1, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 519, .adv_w = 160, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 521, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 528, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 538, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 545, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 555, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 562, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 584, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 594, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 605, .adv_w = 160, .box_w = 5, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 615, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 626, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 160, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 645, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 652, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 659, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 669, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 679, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 693, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 709, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 716, .adv_w = 160, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 725, .adv_w = 160, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 733, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 743, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 750, .adv_w = 160, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 765, .adv_w = 160, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 767, .adv_w = 160, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 782, .adv_w = 160, .box_w = 8, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 786, .adv_w = 160, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 787, .adv_w = 160, .box_w = 1, .box_h = 11, .ofs_x = 4, .ofs_y = -3},
    {.bitmap_index = 789, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 800, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 809, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 820, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 829, .adv_w = 160, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 831, .adv_w = 160, .box_w = 5, .box_h = 2, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 833, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 851, .adv_w = 160, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 852, .adv_w = 160, .box_w = 5, .box_h = 5, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 856, .adv_w = 160, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 858, .adv_w = 160, .box_w = 3, .box_h = 4, .ofs_x = 3, .ofs_y = -4},
    {.bitmap_index = 860, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 867, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 877, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 890, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 903, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 916, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 929, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 941, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 953, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 963, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 976, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 989, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1002, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1015, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1027, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1040, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1053, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1066, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1078, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1088, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1101, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1114, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1140, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1153, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1166, .adv_w = 160, .box_w = 6, .box_h = 6, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 1171, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1182, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1195, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1208, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1221, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1233, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1246, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1255, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1264, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1276, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1288, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1300, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1312, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1323, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1335, .adv_w = 160, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1344, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1355, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1367, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1379, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1391, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1402, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1414, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1426, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1438, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1449, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1460, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1472, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1484, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1496, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1508, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1520, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1531, .adv_w = 160, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1539, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1548, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1560, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1572, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1584, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1595, .adv_w = 160, .box_w = 7, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1610, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1623, .adv_w = 160, .box_w = 7, .box_h = 16, .ofs_x = 1, .ofs_y = -4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_1[] = {
    0, 1, 2, 3, 4, 5, 6, 0,
    7, 8, 0, 9, 0, 10, 0, 0,
    11, 0, 0, 0, 12, 0, 0, 0,
    13, 0, 0, 14
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 160, .range_length = 28, .glyph_id_start = 96,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_1, .list_length = 28, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 191, .range_length = 65, .glyph_id_start = 111,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_tamzen_20 = {
#else
lv_font_t lv_font_tamzen_20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 18,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_TAMZEN_20*/

