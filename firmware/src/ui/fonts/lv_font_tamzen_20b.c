/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: --bpp 1 --size 20 --font Tamzen10x20b-bnp.ttf --range 0x20-0xFF --format lvgl --no-compress --lv-font-name lv_font_tamzen_20b -o /Users/joey/Code/M5Dashboard/firmware/src/ui/fonts/lv_font_tamzen_20b.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_FONT_TAMZEN_20B
#define LV_FONT_TAMZEN_20B 1
#endif

#if LV_FONT_TAMZEN_20B

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0x6f, 0xff, 0x66, 0x0, 0x6, 0x60,

    /* U+0022 "\"" */
    0xcf, 0x3c, 0xf3,

    /* U+0023 "#" */
    0x19, 0x86, 0x61, 0x99, 0xff, 0x33, 0xc, 0xc3,
    0x30, 0xcc, 0xff, 0x99, 0x86, 0x61, 0x98,

    /* U+0024 "$" */
    0x18, 0x30, 0x63, 0xec, 0x78, 0x30, 0x3e, 0x6,
    0xf, 0x1b, 0xe3, 0x6, 0xc, 0x0,

    /* U+0025 "%" */
    0x70, 0x6c, 0xf6, 0xce, 0xc0, 0xc0, 0xc0, 0xdc,
    0xdb, 0xcd, 0x83, 0x80,

    /* U+0026 "&" */
    0x70, 0x6c, 0x36, 0x1b, 0x7, 0x3, 0x83, 0x6d,
    0x9e, 0xc6, 0x67, 0x9e, 0x60,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x19, 0x98, 0xcc, 0x63, 0x18, 0xc6, 0x18, 0xc3,
    0xc,

    /* U+0029 ")" */
    0xc3, 0xc, 0x61, 0x8c, 0x63, 0x18, 0xcc, 0x66,
    0x60,

    /* U+002A "*" */
    0x18, 0xdb, 0x7e, 0x3c, 0x7e, 0xdb, 0x18,

    /* U+002B "+" */
    0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18,

    /* U+002C "," */
    0x77, 0x33, 0xe0,

    /* U+002D "-" */
    0xff,

    /* U+002E "." */
    0xfc,

    /* U+002F "/" */
    0x3, 0x3, 0x6, 0x6, 0xc, 0xc, 0x18, 0x18,
    0x30, 0x30, 0x60, 0x60, 0xc0, 0xc0,

    /* U+0030 "0" */
    0x7d, 0x8f, 0x1e, 0x7d, 0xfe, 0xf9, 0xe3, 0xc6,
    0xf8,

    /* U+0031 "1" */
    0x18, 0x38, 0x78, 0xd8, 0x18, 0x18, 0x18, 0x18,
    0x18, 0xff,

    /* U+0032 "2" */
    0x7d, 0x8c, 0x18, 0x30, 0xc3, 0xc, 0x30, 0xc1,
    0xfc,

    /* U+0033 "3" */
    0xfe, 0xc, 0x30, 0xc3, 0xc0, 0xc1, 0x83, 0xc6,
    0xf8,

    /* U+0034 "4" */
    0x6, 0xe, 0x1e, 0x36, 0x66, 0xc6, 0xff, 0x6,
    0x6, 0x6,

    /* U+0035 "5" */
    0xff, 0x83, 0x6, 0xf, 0xc0, 0xc1, 0x83, 0xc6,
    0xf8,

    /* U+0036 "6" */
    0x38, 0xc3, 0x6, 0xf, 0xd8, 0xf1, 0xe3, 0xc6,
    0xf8,

    /* U+0037 "7" */
    0xfe, 0xc, 0x30, 0x61, 0x83, 0xc, 0x18, 0x60,
    0xc0,

    /* U+0038 "8" */
    0x7d, 0x8f, 0x1e, 0x37, 0xd8, 0xf1, 0xe3, 0xc6,
    0xf8,

    /* U+0039 "9" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x6f, 0xc1, 0x83, 0xc,
    0x70,

    /* U+003A ":" */
    0xfc, 0x0, 0x3f,

    /* U+003B ";" */
    0x77, 0x0, 0x0, 0x77, 0x33, 0xe0,

    /* U+003C "<" */
    0xc, 0x63, 0x18, 0xc1, 0x83, 0x6, 0xc,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0xc1, 0x83, 0x6, 0xc, 0x63, 0x18, 0xc0,

    /* U+003F "?" */
    0x7e, 0xc3, 0x3, 0x6, 0xc, 0x18, 0x0, 0x0,
    0x0, 0x18, 0x18,

    /* U+0040 "@" */
    0x3e, 0x31, 0xb0, 0x78, 0x3c, 0xfe, 0xcf, 0x67,
    0xb3, 0xd9, 0xe7, 0xf0, 0x18, 0x6, 0x1, 0xf8,

    /* U+0041 "A" */
    0x38, 0xfb, 0x1e, 0x3c, 0x7f, 0xf1, 0xe3, 0xc7,
    0x8c,

    /* U+0042 "B" */
    0xfd, 0x8f, 0x1e, 0x3f, 0xd8, 0xf1, 0xe3, 0xc7,
    0xf8,

    /* U+0043 "C" */
    0x3e, 0xc3, 0x6, 0xc, 0x18, 0x30, 0x60, 0x60,
    0x7c,

    /* U+0044 "D" */
    0xf9, 0x9b, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xcd,
    0xf0,

    /* U+0045 "E" */
    0xff, 0x83, 0x6, 0xf, 0xd8, 0x30, 0x60, 0xc1,
    0xfc,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xf, 0xd8, 0x30, 0x60, 0xc1,
    0x80,

    /* U+0047 "G" */
    0x3e, 0xc3, 0x6, 0xc, 0x19, 0xf1, 0xe3, 0x66,
    0x7c,

    /* U+0048 "H" */
    0xc7, 0x8f, 0x1e, 0x3f, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8c,

    /* U+0049 "I" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0xff,

    /* U+004A "J" */
    0x6, 0xc, 0x18, 0x30, 0x60, 0xf1, 0xe3, 0xc6,
    0xf8,

    /* U+004B "K" */
    0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xf0, 0xd8, 0xcc,
    0xc6, 0xc3,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0xfc,

    /* U+004D "M" */
    0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3,

    /* U+004E "N" */
    0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3,
    0xc3, 0xc3,

    /* U+004F "O" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0x66, 0x3c,

    /* U+0050 "P" */
    0xfd, 0x8f, 0x1e, 0x3c, 0x7f, 0xb0, 0x60, 0xc1,
    0x80,

    /* U+0051 "Q" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0x66, 0x3c, 0xc, 0x7,

    /* U+0052 "R" */
    0xfd, 0x8f, 0x1e, 0x3c, 0x7f, 0xb6, 0x66, 0xc7,
    0x8c,

    /* U+0053 "S" */
    0x7d, 0x8f, 0x6, 0x7, 0x3, 0x81, 0x83, 0xc6,
    0xf8,

    /* U+0054 "T" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18,

    /* U+0055 "U" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc6,
    0xf8,

    /* U+0056 "V" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xdb, 0x36, 0x38,
    0x70,

    /* U+0057 "W" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xff,
    0xe7, 0xc3,

    /* U+0058 "X" */
    0xc7, 0x8f, 0x1b, 0x63, 0x87, 0x1b, 0x63, 0xc7,
    0x8c,

    /* U+0059 "Y" */
    0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18, 0x18, 0x18,
    0x18, 0x18,

    /* U+005A "Z" */
    0xfe, 0xc, 0x18, 0x61, 0x86, 0x18, 0x60, 0xc1,
    0xfc,

    /* U+005B "[" */
    0xff, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc,
    0x30, 0xc3, 0xf0,

    /* U+005C "\\" */
    0xc0, 0xc0, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18,
    0xc, 0xc, 0x6, 0x6, 0x3, 0x3,

    /* U+005D "]" */
    0xfc, 0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30,
    0xc3, 0xf, 0xf0,

    /* U+005E "^" */
    0x10, 0x71, 0xb6, 0x30,

    /* U+005F "_" */
    0xff, 0xc0,

    /* U+0060 "`" */
    0xc3, 0xc, 0x30,

    /* U+0061 "a" */
    0x7c, 0xc, 0x1b, 0xfc, 0x78, 0xf3, 0xbb,

    /* U+0062 "b" */
    0xc0, 0xc0, 0xc0, 0xdc, 0xe6, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc6, 0xfc,

    /* U+0063 "c" */
    0x3c, 0xcf, 0x6, 0xc, 0x18, 0x19, 0x9e,

    /* U+0064 "d" */
    0x3, 0x3, 0x3, 0x3f, 0x63, 0xc3, 0xc3, 0xc3,
    0xc3, 0x67, 0x3b,

    /* U+0065 "e" */
    0x3c, 0xcf, 0x1f, 0xfc, 0x18, 0x18, 0x1f,

    /* U+0066 "f" */
    0x1e, 0x60, 0xc1, 0x8f, 0xe6, 0xc, 0x18, 0x30,
    0x60, 0xc1, 0x80,

    /* U+0067 "g" */
    0x3d, 0xb3, 0xb0, 0xd8, 0x66, 0x61, 0xe1, 0x80,
    0xfc, 0xc3, 0x61, 0x9f, 0x80,

    /* U+0068 "h" */
    0xc1, 0x83, 0x6, 0xee, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0069 "i" */
    0x18, 0x18, 0x0, 0x0, 0xf8, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0xff,

    /* U+006A "j" */
    0xc, 0x30, 0x0, 0x7c, 0x30, 0xc3, 0xc, 0x30,
    0xc3, 0xc, 0x6f, 0x0,

    /* U+006B "k" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc6, 0xcc, 0xd8, 0xf0,
    0xf8, 0xcc, 0xc6, 0xc3,

    /* U+006C "l" */
    0xf8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0xf,

    /* U+006D "m" */
    0xfe, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,

    /* U+006E "n" */
    0xdd, 0xcf, 0x1e, 0x3c, 0x78, 0xf1, 0xe3,

    /* U+006F "o" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c,

    /* U+0070 "p" */
    0xdc, 0xe6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc,
    0xc0, 0xc0, 0xc0,

    /* U+0071 "q" */
    0x3f, 0x63, 0xc3, 0xc3, 0xc3, 0xc3, 0x67, 0x3b,
    0x3, 0x3, 0x3,

    /* U+0072 "r" */
    0xdd, 0xcf, 0x6, 0xc, 0x18, 0x30, 0x60,

    /* U+0073 "s" */
    0x7d, 0x8f, 0x3, 0x81, 0xc0, 0xf1, 0xbe,

    /* U+0074 "t" */
    0x30, 0x63, 0xf9, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x3c,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf3, 0xbb,

    /* U+0076 "v" */
    0xc7, 0x8f, 0x1e, 0x36, 0xcd, 0x8e, 0x1c,

    /* U+0077 "w" */
    0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xdb, 0xdb, 0x7e,

    /* U+0078 "x" */
    0xc7, 0x8d, 0xb1, 0xc3, 0x8d, 0xb1, 0xe3,

    /* U+0079 "y" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x6d, 0xcf, 0x83, 0x6,
    0x19, 0xe0,

    /* U+007A "z" */
    0xfe, 0xc, 0x30, 0xc3, 0xc, 0x30, 0x7f,

    /* U+007B "{" */
    0xf, 0x18, 0x18, 0x18, 0x18, 0x18, 0x30, 0xe0,
    0x30, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xf0, 0x18, 0x18, 0x18, 0x18, 0x18, 0xc, 0x7,
    0xc, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf0,

    /* U+007E "~" */
    0x63, 0xf3, 0xcf, 0xc6,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0x66, 0x0, 0x6, 0x6f, 0xff, 0x60,

    /* U+00A2 "¢" */
    0x18, 0x30, 0xf3, 0x3c, 0x18, 0x30, 0x60, 0x66,
    0x78, 0x60, 0xc0,

    /* U+00A3 "£" */
    0x3c, 0xcd, 0x83, 0xf, 0x8c, 0x18, 0x30, 0x77,
    0xb8,

    /* U+00A4 "¤" */
    0xc1, 0xbf, 0x8d, 0x8c, 0x66, 0x31, 0xb1, 0xfd,
    0x83,

    /* U+00A5 "¥" */
    0xc3, 0x66, 0x3c, 0x18, 0x7e, 0x18, 0x18, 0x7e,
    0x18, 0x18,

    /* U+00A6 "¦" */
    0xff, 0xc0, 0xff, 0xc0,

    /* U+00A8 "¨" */
    0xcf, 0x30,

    /* U+00A9 "©" */
    0x3c, 0x42, 0x81, 0x9d, 0xb1, 0xb1, 0xb1, 0x9d,
    0x81, 0x42, 0x3c,

    /* U+00AB "«" */
    0x19, 0x99, 0x99, 0x99, 0x86, 0x61, 0x98, 0x66,

    /* U+00AD "­" */
    0xff,

    /* U+00B0 "°" */
    0x7b, 0x3c, 0xf3, 0x78,

    /* U+00B4 "´" */
    0x36, 0xc0,

    /* U+00B8 "¸" */
    0x33, 0x3e,

    /* U+00BB "»" */
    0xcc, 0x33, 0xc, 0xc3, 0x33, 0x33, 0x33, 0x30,

    /* U+00BF "¿" */
    0x18, 0x18, 0x0, 0x0, 0x0, 0x18, 0x30, 0x60,
    0xc0, 0xc3, 0x7e,

    /* U+00C0 "À" */
    0xc0, 0xc0, 0xc0, 0x3, 0x8f, 0xb1, 0xe3, 0xc7,
    0xff, 0x1e, 0x3c, 0x78, 0xc0,

    /* U+00C1 "Á" */
    0x6, 0x18, 0x60, 0x3, 0x8f, 0xb1, 0xe3, 0xc7,
    0xff, 0x1e, 0x3c, 0x78, 0xc0,

    /* U+00C2 "Â" */
    0x10, 0x71, 0xb0, 0x3, 0x8f, 0xb1, 0xe3, 0xc7,
    0xff, 0x1e, 0x3c, 0x78, 0xc0,

    /* U+00C3 "Ã" */
    0x67, 0xaf, 0x30, 0x3, 0x8f, 0xb1, 0xe3, 0xc7,
    0xff, 0x1e, 0x3c, 0x78, 0xc0,

    /* U+00C4 "Ä" */
    0xcd, 0x98, 0x1, 0xc7, 0xd8, 0xf1, 0xe3, 0xff,
    0x8f, 0x1e, 0x3c, 0x60,

    /* U+00C5 "Å" */
    0x38, 0xd9, 0xb1, 0xc3, 0x8d, 0xb1, 0xe3, 0xc7,
    0xff, 0x1e, 0x3c, 0x60,

    /* U+00C6 "Æ" */
    0x1f, 0x9e, 0x1b, 0x19, 0x8c, 0xf6, 0x63, 0xf1,
    0x98, 0xcc, 0x67, 0xc0,

    /* U+00C7 "Ç" */
    0x3e, 0xc3, 0x6, 0xc, 0x18, 0x30, 0x60, 0x60,
    0x7c, 0x60, 0xc1, 0x8e, 0x0,

    /* U+00C8 "È" */
    0xc0, 0xc0, 0xc0, 0xf, 0xf8, 0x30, 0x60, 0xfd,
    0x83, 0x6, 0xc, 0x1f, 0xc0,

    /* U+00C9 "É" */
    0x6, 0x18, 0x60, 0xf, 0xf8, 0x30, 0x60, 0xfd,
    0x83, 0x6, 0xc, 0x1f, 0xc0,

    /* U+00CA "Ê" */
    0x10, 0x71, 0xb0, 0xf, 0xf8, 0x30, 0x60, 0xfd,
    0x83, 0x6, 0xc, 0x1f, 0xc0,

    /* U+00CB "Ë" */
    0xcd, 0x98, 0x7, 0xfc, 0x18, 0x30, 0x7e, 0xc1,
    0x83, 0x6, 0xf, 0xe0,

    /* U+00CC "Ì" */
    0x60, 0x30, 0x18, 0x0, 0xff, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00CD "Í" */
    0x6, 0xc, 0x18, 0x0, 0xff, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00CE "Î" */
    0x10, 0x38, 0x6c, 0x0, 0xff, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00CF "Ï" */
    0xcc, 0xcc, 0x0, 0xff, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00D0 "Ð" */
    0x7c, 0x66, 0x63, 0x63, 0xf3, 0x63, 0x63, 0x63,
    0x66, 0x7c,

    /* U+00D1 "Ñ" */
    0x66, 0xd6, 0xcc, 0x0, 0xc3, 0xe3, 0xf3, 0xdb,
    0xcf, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3,

    /* U+00D2 "Ò" */
    0x60, 0x60, 0x60, 0x7, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00D3 "Ó" */
    0x6, 0x18, 0x60, 0x7, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00D4 "Ô" */
    0x10, 0x71, 0xb0, 0x7, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00D5 "Õ" */
    0x67, 0xaf, 0x30, 0x7, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00D6 "Ö" */
    0xcd, 0x98, 0x3, 0xec, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xc0,

    /* U+00D7 "×" */
    0xc6, 0xd8, 0xe1, 0xc6, 0xd8, 0xc0,

    /* U+00D8 "Ø" */
    0x6, 0xfb, 0x1e, 0x3c, 0xfb, 0xfd, 0xf3, 0xc7,
    0x8d, 0xf6, 0x0,

    /* U+00D9 "Ù" */
    0xc0, 0xc0, 0xc0, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00DA "Ú" */
    0x6, 0x18, 0x60, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00DB "Û" */
    0x10, 0x71, 0xb0, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x6f, 0x80,

    /* U+00DC "Ü" */
    0xcd, 0x98, 0x6, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xc0,

    /* U+00DD "Ý" */
    0x6, 0xc, 0x18, 0x0, 0xc3, 0xc3, 0xc3, 0x66,
    0x3c, 0x18, 0x18, 0x18, 0x18, 0x18,

    /* U+00DE "Þ" */
    0xc1, 0x83, 0xf6, 0x3c, 0x78, 0xf1, 0xfe, 0xc1,
    0x80,

    /* U+00DF "ß" */
    0x7d, 0x8f, 0x1e, 0x6d, 0xd8, 0xf1, 0xe3, 0xc7,
    0x98,

    /* U+00E0 "à" */
    0xc0, 0xc0, 0xc0, 0x0, 0xf, 0x81, 0x83, 0x7f,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00E1 "á" */
    0x6, 0x18, 0x60, 0x0, 0xf, 0x81, 0x83, 0x7f,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00E2 "â" */
    0x10, 0x71, 0xb0, 0x0, 0xf, 0x81, 0x83, 0x7f,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00E3 "ã" */
    0x67, 0xaf, 0x30, 0x0, 0xf, 0x81, 0x83, 0x7f,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00E4 "ä" */
    0x66, 0xcc, 0x0, 0x7, 0xc0, 0xc1, 0xbf, 0xc7,
    0x8f, 0x1b, 0xf0,

    /* U+00E5 "å" */
    0x38, 0xd9, 0xb1, 0xc0, 0xf, 0x81, 0x83, 0x7f,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00E6 "æ" */
    0x7b, 0x83, 0x30, 0xcd, 0xff, 0xcc, 0x33, 0xc,
    0xc1, 0xdf,

    /* U+00E7 "ç" */
    0x7d, 0x8f, 0x6, 0xc, 0x18, 0x31, 0xbe, 0x18,
    0x30, 0x63, 0x80,

    /* U+00E8 "è" */
    0xc0, 0xc0, 0xc0, 0x0, 0xf, 0xb1, 0xe3, 0xff,
    0x83, 0x6, 0x7, 0xe0,

    /* U+00E9 "é" */
    0x6, 0x18, 0x60, 0x0, 0xf, 0xb1, 0xe3, 0xff,
    0x83, 0x6, 0x7, 0xe0,

    /* U+00EA "ê" */
    0x10, 0x71, 0xb0, 0x0, 0xf, 0xb1, 0xe3, 0xff,
    0x83, 0x6, 0x7, 0xe0,

    /* U+00EB "ë" */
    0x66, 0xcc, 0x0, 0x7, 0xd8, 0xf1, 0xff, 0xc1,
    0x83, 0x3, 0xf0,

    /* U+00EC "ì" */
    0x60, 0x30, 0x18, 0x0, 0x0, 0xf8, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00ED "í" */
    0x6, 0xc, 0x18, 0x0, 0x0, 0xf8, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00EE "î" */
    0x10, 0x38, 0x6c, 0x0, 0x0, 0xf8, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0xff,

    /* U+00EF "ï" */
    0xcc, 0xcc, 0x0, 0x0, 0xf8, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0xff,

    /* U+00F0 "ð" */
    0x31, 0x1e, 0x7c, 0x6, 0x3f, 0x63, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+00F1 "ñ" */
    0x67, 0xaf, 0x30, 0x0, 0x1f, 0xb1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x60,

    /* U+00F2 "ò" */
    0x60, 0x60, 0x60, 0x0, 0xf, 0xb1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xc0,

    /* U+00F3 "ó" */
    0x6, 0x18, 0x60, 0x0, 0xf, 0xb1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xc0,

    /* U+00F4 "ô" */
    0x10, 0x71, 0xb0, 0x0, 0xf, 0xb1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xc0,

    /* U+00F5 "õ" */
    0x67, 0xaf, 0x30, 0x0, 0xf, 0xb1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xc0,

    /* U+00F6 "ö" */
    0xcd, 0x98, 0x0, 0x7, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1b, 0xe0,

    /* U+00F7 "÷" */
    0x18, 0x18, 0x0, 0x0, 0xff, 0x0, 0x0, 0x18,
    0x18,

    /* U+00F8 "ø" */
    0x6, 0xfb, 0x1e, 0x7d, 0xfe, 0xf9, 0xe3, 0x7d,
    0x80,

    /* U+00F9 "ù" */
    0xc0, 0xc0, 0xc0, 0x0, 0x18, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00FA "ú" */
    0x6, 0x18, 0x60, 0x0, 0x18, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00FB "û" */
    0x10, 0x71, 0xb0, 0x0, 0x18, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xe0,

    /* U+00FC "ü" */
    0xcd, 0x98, 0x0, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1b, 0xf0,

    /* U+00FD "ý" */
    0x6, 0x18, 0x60, 0x0, 0x18, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x37, 0xe0, 0xc1, 0xbe,

    /* U+00FE "þ" */
    0xc0, 0xc0, 0xc0, 0xdc, 0xe6, 0xc3, 0xc3, 0xc3,
    0xc3, 0xe6, 0xdc, 0xc0, 0xc0, 0xc0,

    /* U+00FF "ÿ" */
    0xcd, 0x98, 0x0, 0xc, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1b, 0xf0, 0x60, 0xdf, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 160, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 160, .box_w = 4, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 7, .adv_w = 160, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 10, .adv_w = 160, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 25, .adv_w = 160, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 39, .adv_w = 160, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 51, .adv_w = 160, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 160, .box_w = 2, .box_h = 4, .ofs_x = 3, .ofs_y = 8},
    {.bitmap_index = 65, .adv_w = 160, .box_w = 5, .box_h = 14, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 74, .adv_w = 160, .box_w = 5, .box_h = 14, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 83, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 90, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 97, .adv_w = 160, .box_w = 4, .box_h = 5, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 100, .adv_w = 160, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 101, .adv_w = 160, .box_w = 3, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 102, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 116, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 163, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 208, .adv_w = 160, .box_w = 3, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 160, .box_w = 4, .box_h = 11, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 217, .adv_w = 160, .box_w = 6, .box_h = 9, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 224, .adv_w = 160, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 229, .adv_w = 160, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 236, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 160, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 263, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 272, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 364, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 373, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 412, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 424, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 433, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 442, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 470, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 480, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 489, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 499, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 508, .adv_w = 160, .box_w = 6, .box_h = 14, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 519, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 533, .adv_w = 160, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 544, .adv_w = 160, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 548, .adv_w = 160, .box_w = 10, .box_h = 1, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 550, .adv_w = 160, .box_w = 5, .box_h = 4, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 553, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 571, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 578, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 160, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 620, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 630, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 642, .adv_w = 160, .box_w = 6, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 654, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 666, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 677, .adv_w = 160, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 692, .adv_w = 160, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 700, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 711, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 722, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 745, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 752, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 759, .adv_w = 160, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 767, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 774, .adv_w = 160, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 784, .adv_w = 160, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 791, .adv_w = 160, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 806, .adv_w = 160, .box_w = 2, .box_h = 13, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 810, .adv_w = 160, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 825, .adv_w = 160, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 829, .adv_w = 160, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 830, .adv_w = 160, .box_w = 4, .box_h = 11, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 836, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 847, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 856, .adv_w = 160, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 865, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 875, .adv_w = 160, .box_w = 2, .box_h = 13, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 879, .adv_w = 160, .box_w = 6, .box_h = 2, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 881, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 892, .adv_w = 160, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 900, .adv_w = 160, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 901, .adv_w = 160, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 905, .adv_w = 160, .box_w = 4, .box_h = 3, .ofs_x = 4, .ofs_y = 10},
    {.bitmap_index = 907, .adv_w = 160, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 909, .adv_w = 160, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 917, .adv_w = 160, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 928, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 941, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 954, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 967, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 980, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 992, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1004, .adv_w = 160, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1016, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1029, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1042, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1055, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1068, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1080, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1094, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1108, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1122, .adv_w = 160, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1135, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1145, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1159, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1172, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1185, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1198, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1211, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1223, .adv_w = 160, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1229, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1240, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1253, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1266, .adv_w = 160, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1279, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1291, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1305, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1314, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1323, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1335, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1347, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1359, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1371, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1382, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1394, .adv_w = 160, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1404, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1415, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1427, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1439, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1451, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1462, .adv_w = 160, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1475, .adv_w = 160, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1488, .adv_w = 160, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1501, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1513, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1525, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1537, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1549, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1561, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1573, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1585, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1596, .adv_w = 160, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 1605, .adv_w = 160, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1614, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1626, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1638, .adv_w = 160, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1650, .adv_w = 160, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1661, .adv_w = 160, .box_w = 7, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1675, .adv_w = 160, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1689, .adv_w = 160, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -3}
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
const lv_font_t lv_font_tamzen_20b = {
#else
lv_font_t lv_font_tamzen_20b = {
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



#endif /*#if LV_FONT_TAMZEN_20B*/

