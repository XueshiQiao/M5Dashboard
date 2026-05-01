/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font Tamzen8x16r-bnp.ttf --range 0x20-0xFF --format lvgl --no-compress --lv-font-name lv_font_tamzen_16 -o /Users/joey/Code/M5Dashboard/firmware/src/ui/fonts/lv_font_tamzen_16.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_FONT_TAMZEN_16
#define LV_FONT_TAMZEN_16 1
#endif

#if LV_FONT_TAMZEN_16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc, 0xc0,

    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x49, 0x2f, 0xd2, 0x49, 0x2f, 0xd2, 0x48,

    /* U+0024 "$" */
    0x21, 0x1f, 0x8, 0x38, 0x21, 0xf1, 0x8,

    /* U+0025 "%" */
    0x61, 0x26, 0x53, 0x41, 0x5, 0x94, 0xc9, 0xc,

    /* U+0026 "&" */
    0x61, 0x22, 0x44, 0x86, 0xc, 0x64, 0xc6, 0x8c,
    0xe4,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x29, 0x49, 0x24, 0x89, 0x10,

    /* U+0029 ")" */
    0x89, 0x12, 0x49, 0x29, 0x40,

    /* U+002A "*" */
    0x48, 0xcf, 0xcc, 0x48,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0xf5, 0x80,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x4, 0x10, 0x82, 0x10, 0x42, 0x8, 0x41, 0x8,
    0x20,

    /* U+0030 "0" */
    0x7a, 0x18, 0xe5, 0xa7, 0x18, 0x61, 0x78,

    /* U+0031 "1" */
    0x23, 0x28, 0x42, 0x10, 0x84, 0xf8,

    /* U+0032 "2" */
    0x7a, 0x10, 0x41, 0x8, 0x42, 0x10, 0xfc,

    /* U+0033 "3" */
    0xfc, 0x21, 0xe, 0x4, 0x10, 0x61, 0x78,

    /* U+0034 "4" */
    0x8, 0x62, 0x92, 0x8b, 0xf0, 0x82, 0x8,

    /* U+0035 "5" */
    0xfe, 0x8, 0x3e, 0x4, 0x10, 0x61, 0x78,

    /* U+0036 "6" */
    0x39, 0x8, 0x20, 0xfa, 0x18, 0x61, 0x78,

    /* U+0037 "7" */
    0xfc, 0x10, 0x82, 0x10, 0x42, 0x8, 0x20,

    /* U+0038 "8" */
    0x7a, 0x18, 0x61, 0x7a, 0x18, 0x61, 0x78,

    /* U+0039 "9" */
    0x7a, 0x18, 0x61, 0x7c, 0x10, 0x42, 0x70,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xf0, 0x3d, 0x60,

    /* U+003C "<" */
    0x8, 0x88, 0x88, 0x20, 0x82, 0x8,

    /* U+003D "=" */
    0xfc, 0x0, 0x3f,

    /* U+003E ">" */
    0x82, 0x8, 0x20, 0x88, 0x88, 0x80,

    /* U+003F "?" */
    0x7a, 0x10, 0x42, 0x10, 0x80, 0x0, 0x20, 0x80,

    /* U+0040 "@" */
    0x38, 0x8a, 0xc, 0xfa, 0x34, 0x68, 0xd3, 0x9b,
    0x1, 0x1, 0xf0,

    /* U+0041 "A" */
    0x31, 0x28, 0x61, 0x87, 0xf8, 0x61, 0x84,

    /* U+0042 "B" */
    0xfa, 0x18, 0x61, 0xfa, 0x18, 0x61, 0xf8,

    /* U+0043 "C" */
    0x3d, 0x8, 0x20, 0x82, 0x8, 0x10, 0x3c,

    /* U+0044 "D" */
    0xf2, 0x28, 0x61, 0x86, 0x18, 0x62, 0xf0,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0xfc,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0x80,

    /* U+0047 "G" */
    0x3d, 0x8, 0x20, 0x8e, 0x18, 0x51, 0x3c,

    /* U+0048 "H" */
    0x86, 0x18, 0x61, 0xfe, 0x18, 0x61, 0x84,

    /* U+0049 "I" */
    0xf9, 0x8, 0x42, 0x10, 0x84, 0xf8,

    /* U+004A "J" */
    0x4, 0x10, 0x41, 0x4, 0x18, 0x61, 0x78,

    /* U+004B "K" */
    0x86, 0x29, 0x28, 0xc2, 0x89, 0x22, 0x84,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0xfc,

    /* U+004D "M" */
    0x83, 0x8e, 0xac, 0x99, 0x30, 0x60, 0xc1, 0x82,

    /* U+004E "N" */
    0x87, 0x1a, 0x65, 0x8e, 0x18, 0x61, 0x84,

    /* U+004F "O" */
    0x7a, 0x18, 0x61, 0x86, 0x18, 0x61, 0x78,

    /* U+0050 "P" */
    0xfa, 0x18, 0x61, 0xfa, 0x8, 0x20, 0x80,

    /* U+0051 "Q" */
    0x79, 0xa, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x78,
    0x1c,

    /* U+0052 "R" */
    0xfa, 0x18, 0x61, 0xfa, 0x48, 0xa1, 0x84,

    /* U+0053 "S" */
    0x7e, 0x8, 0x10, 0x30, 0x20, 0x41, 0xf8,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x61, 0x78,

    /* U+0056 "V" */
    0x86, 0x18, 0x61, 0x85, 0x24, 0x8c, 0x30,

    /* U+0057 "W" */
    0x83, 0x6, 0xc, 0x19, 0x32, 0x64, 0xd5, 0xc6,

    /* U+0058 "X" */
    0x83, 0x5, 0x11, 0x41, 0x5, 0x11, 0x41, 0x82,

    /* U+0059 "Y" */
    0x83, 0x5, 0x11, 0x41, 0x2, 0x4, 0x8, 0x10,

    /* U+005A "Z" */
    0xfc, 0x21, 0x4, 0x20, 0x84, 0x10, 0xfc,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x8f,

    /* U+005C "\\" */
    0x82, 0x4, 0x10, 0x20, 0x81, 0x4, 0x8, 0x20,
    0x41,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x1f,

    /* U+005E "^" */
    0x22, 0xa2,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0x84, 0x21,

    /* U+0061 "a" */
    0x78, 0x10, 0x5f, 0x86, 0x37, 0x40,

    /* U+0062 "b" */
    0x82, 0x8, 0x2e, 0xc6, 0x18, 0x61, 0xc6, 0xe0,

    /* U+0063 "c" */
    0x7a, 0x18, 0x20, 0x82, 0x17, 0x80,

    /* U+0064 "d" */
    0x4, 0x10, 0x5d, 0x8e, 0x18, 0x61, 0x8d, 0xd0,

    /* U+0065 "e" */
    0x7a, 0x18, 0x7f, 0x82, 0x7, 0xc0,

    /* U+0066 "f" */
    0x1c, 0x82, 0x3f, 0x20, 0x82, 0x8, 0x20, 0x80,

    /* U+0067 "g" */
    0x7e, 0x28, 0xa2, 0x72, 0x7, 0xa1, 0x85, 0xe0,

    /* U+0068 "h" */
    0x82, 0x8, 0x2e, 0xc6, 0x18, 0x61, 0x86, 0x10,

    /* U+0069 "i" */
    0x21, 0x1, 0xc2, 0x10, 0x84, 0x27, 0xc0,

    /* U+006A "j" */
    0x11, 0x7, 0x11, 0x11, 0x11, 0x11, 0xe0,

    /* U+006B "k" */
    0x82, 0x8, 0x22, 0x92, 0x8e, 0x24, 0x8a, 0x10,

    /* U+006C "l" */
    0xe0, 0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x70,

    /* U+006D "m" */
    0xed, 0x26, 0x4c, 0x99, 0x32, 0x64, 0x80,

    /* U+006E "n" */
    0xbb, 0x18, 0x61, 0x86, 0x18, 0x40,

    /* U+006F "o" */
    0x7a, 0x18, 0x61, 0x86, 0x17, 0x80,

    /* U+0070 "p" */
    0xbb, 0x18, 0x61, 0x87, 0x1b, 0xa0, 0x82, 0x0,

    /* U+0071 "q" */
    0x76, 0x38, 0x61, 0x86, 0x37, 0x41, 0x4, 0x10,

    /* U+0072 "r" */
    0xbb, 0x18, 0x20, 0x82, 0x8, 0x0,

    /* U+0073 "s" */
    0x7e, 0x4, 0xc, 0x8, 0x1f, 0x80,

    /* U+0074 "t" */
    0x20, 0x8f, 0xc8, 0x20, 0x82, 0x8, 0x1c,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x86, 0x37, 0x40,

    /* U+0076 "v" */
    0x86, 0x18, 0x52, 0x48, 0xc3, 0x0,

    /* U+0077 "w" */
    0x83, 0x26, 0x4c, 0x99, 0x32, 0x5b, 0x0,

    /* U+0078 "x" */
    0x82, 0x88, 0xa0, 0x82, 0x88, 0xa0, 0x80,

    /* U+0079 "y" */
    0x86, 0x18, 0x61, 0x86, 0x37, 0x41, 0x5, 0xe0,

    /* U+007A "z" */
    0xfc, 0x21, 0x8, 0x42, 0xf, 0xc0,

    /* U+007B "{" */
    0xe, 0x20, 0x40, 0x81, 0x2, 0x38, 0x8, 0x10,
    0x20, 0x40, 0x80, 0xe0,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0xe0, 0x20, 0x40, 0x81, 0x2, 0x3, 0x88, 0x10,
    0x20, 0x40, 0x8e, 0x0,

    /* U+007E "~" */
    0x63, 0x26, 0x30,

    /* U+00A0 " " */
    0x0,

    /* U+00A1 "¡" */
    0xcf, 0xc0,

    /* U+00A2 "¢" */
    0x10, 0x43, 0x91, 0x82, 0x8, 0x11, 0x38, 0x41,
    0x0,

    /* U+00A3 "£" */
    0x1c, 0x44, 0x81, 0xf, 0x84, 0x8, 0x39, 0x8c,

    /* U+00A4 "¤" */
    0x85, 0xe4, 0x92, 0x49, 0xe8, 0x40,

    /* U+00A5 "¥" */
    0x82, 0x88, 0xa0, 0x87, 0xc2, 0x1f, 0x8, 0x10,

    /* U+00A6 "¦" */
    0xf1, 0xe0,

    /* U+00A8 "¨" */
    0x8c, 0x40,

    /* U+00A9 "©" */
    0x38, 0x8a, 0xc, 0xda, 0x34, 0x66, 0xc1, 0x44,
    0x70,

    /* U+00AB "«" */
    0x12, 0x49, 0x24, 0x84, 0x84, 0x84, 0x80,

    /* U+00AD "­" */
    0xfc,

    /* U+00B0 "°" */
    0x74, 0x63, 0x17, 0x0,

    /* U+00B8 "¸" */
    0x27, 0x0,

    /* U+00BB "»" */
    0x90, 0x90, 0x90, 0x92, 0x49, 0x24, 0x0,

    /* U+00BF "¿" */
    0x10, 0x40, 0x0, 0x10, 0x84, 0x20, 0x85, 0xe0,

    /* U+00C0 "À" */
    0x40, 0x80, 0xc, 0x31, 0x24, 0x92, 0xfe, 0x18,
    0x61,

    /* U+00C1 "Á" */
    0x8, 0x40, 0xc, 0x31, 0x24, 0x92, 0xfe, 0x18,
    0x61,

    /* U+00C2 "Â" */
    0x31, 0x20, 0xc, 0x31, 0x24, 0x92, 0xfe, 0x18,
    0x61,

    /* U+00C3 "Ã" */
    0x66, 0x60, 0xc, 0x31, 0x24, 0x92, 0xfe, 0x18,
    0x61,

    /* U+00C4 "Ä" */
    0x49, 0x20, 0xc, 0x31, 0x24, 0x92, 0xfe, 0x18,
    0x61,

    /* U+00C5 "Å" */
    0x31, 0x24, 0x8c, 0x31, 0x24, 0x92, 0xfe, 0x18,
    0x61,

    /* U+00C6 "Æ" */
    0x1e, 0x50, 0xa2, 0x44, 0xef, 0x22, 0x44, 0x8e,

    /* U+00C7 "Ç" */
    0x3d, 0x8, 0x20, 0x82, 0x8, 0x10, 0x3c, 0x41,
    0x18,

    /* U+00C8 "È" */
    0x40, 0x80, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x3f,

    /* U+00C9 "É" */
    0x8, 0x40, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x3f,

    /* U+00CA "Ê" */
    0x31, 0x20, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x3f,

    /* U+00CB "Ë" */
    0x49, 0x20, 0x3f, 0x82, 0x8, 0x3e, 0x82, 0x8,
    0x3f,

    /* U+00CC "Ì" */
    0x41, 0x1, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00CD "Í" */
    0x11, 0x1, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00CE "Î" */
    0x64, 0x81, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00CF "Ï" */
    0x8c, 0x41, 0xf2, 0x10, 0x84, 0x21, 0x9, 0xf0,

    /* U+00D0 "Ð" */
    0x78, 0x89, 0xa, 0x1f, 0x28, 0x50, 0xa2, 0x78,

    /* U+00D1 "Ñ" */
    0x66, 0x60, 0x21, 0xc6, 0x99, 0x63, 0x86, 0x18,
    0x61,

    /* U+00D2 "Ò" */
    0x20, 0x20, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+00D3 "Ó" */
    0x8, 0x20, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+00D4 "Ô" */
    0x30, 0x90, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+00D5 "Õ" */
    0x65, 0x30, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+00D6 "Ö" */
    0x48, 0x90, 0x1, 0xc4, 0x50, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+00D7 "×" */
    0x8a, 0x88, 0xa8, 0x80,

    /* U+00D8 "Ø" */
    0x5, 0xe8, 0x63, 0x96, 0x9c, 0x61, 0x85, 0xe8,
    0x0,

    /* U+00D9 "Ù" */
    0x40, 0x80, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x5e,

    /* U+00DA "Ú" */
    0x8, 0x40, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x5e,

    /* U+00DB "Û" */
    0x31, 0x20, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x5e,

    /* U+00DC "Ü" */
    0x49, 0x20, 0x21, 0x86, 0x18, 0x61, 0x86, 0x18,
    0x5e,

    /* U+00DD "Ý" */
    0x8, 0x20, 0x4, 0x18, 0x28, 0x8a, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+00DE "Þ" */
    0x82, 0xf, 0xa1, 0x86, 0x1f, 0xa0, 0x80,

    /* U+00DF "ß" */
    0x7a, 0x18, 0xa6, 0x86, 0x18, 0x62, 0xb0,

    /* U+00E0 "à" */
    0x40, 0x80, 0x1e, 0x4, 0x17, 0xe1, 0x85, 0xf0,

    /* U+00E1 "á" */
    0x8, 0x40, 0x1e, 0x4, 0x17, 0xe1, 0x85, 0xf0,

    /* U+00E2 "â" */
    0x31, 0x20, 0x0, 0x78, 0x10, 0x5f, 0x86, 0x17,
    0xc0,

    /* U+00E3 "ã" */
    0x66, 0x60, 0x0, 0x78, 0x10, 0x5f, 0x86, 0x17,
    0xc0,

    /* U+00E4 "ä" */
    0x49, 0x20, 0x1e, 0x4, 0x17, 0xe1, 0x85, 0xf0,

    /* U+00E5 "å" */
    0x31, 0x24, 0x8c, 0x1, 0xe0, 0x41, 0x7e, 0x18,
    0x5f,

    /* U+00E6 "æ" */
    0x6c, 0x25, 0xcc, 0xf9, 0x12, 0x1b, 0x80,

    /* U+00E7 "ç" */
    0x7a, 0x18, 0x20, 0x82, 0x17, 0x84, 0x11, 0x80,

    /* U+00E8 "è" */
    0x40, 0x80, 0x1e, 0x86, 0x1f, 0xe0, 0x81, 0xf0,

    /* U+00E9 "é" */
    0x8, 0x40, 0x1e, 0x86, 0x1f, 0xe0, 0x81, 0xf0,

    /* U+00EA "ê" */
    0x31, 0x20, 0x0, 0x7a, 0x18, 0x7f, 0x82, 0x7,
    0xc0,

    /* U+00EB "ë" */
    0x49, 0x20, 0x1e, 0x86, 0x1f, 0xe0, 0x81, 0xf0,

    /* U+00EC "ì" */
    0x41, 0x1, 0xc2, 0x10, 0x84, 0x27, 0xc0,

    /* U+00ED "í" */
    0x11, 0x1, 0xc2, 0x10, 0x84, 0x27, 0xc0,

    /* U+00EE "î" */
    0x64, 0x80, 0xe, 0x10, 0x84, 0x21, 0x3e,

    /* U+00EF "ï" */
    0x94, 0x81, 0xc2, 0x10, 0x84, 0x27, 0xc0,

    /* U+00F0 "ð" */
    0x12, 0x18, 0xd0, 0x13, 0xa8, 0xe0, 0xc1, 0x82,
    0x88, 0xe0,

    /* U+00F1 "ñ" */
    0x66, 0x60, 0x0, 0xfa, 0x18, 0x61, 0x86, 0x18,
    0x40,

    /* U+00F2 "ò" */
    0x20, 0x40, 0x1e, 0x86, 0x18, 0x61, 0x85, 0xe0,

    /* U+00F3 "ó" */
    0x8, 0x40, 0x1e, 0x86, 0x18, 0x61, 0x85, 0xe0,

    /* U+00F4 "ô" */
    0x31, 0x20, 0x0, 0x7a, 0x18, 0x61, 0x86, 0x17,
    0x80,

    /* U+00F5 "õ" */
    0x66, 0x60, 0x0, 0x7a, 0x18, 0x61, 0x86, 0x17,
    0x80,

    /* U+00F6 "ö" */
    0x49, 0x20, 0x1e, 0x86, 0x18, 0x61, 0x85, 0xe0,

    /* U+00F7 "÷" */
    0x30, 0xc0, 0x0, 0xfc, 0x0, 0xc, 0x30,

    /* U+00F8 "ø" */
    0x5, 0xe8, 0xe5, 0xa7, 0x18, 0x5e, 0x80,

    /* U+00F9 "ù" */
    0x40, 0x80, 0x21, 0x86, 0x18, 0x61, 0x85, 0xf0,

    /* U+00FA "ú" */
    0x8, 0x40, 0x21, 0x86, 0x18, 0x61, 0x85, 0xf0,

    /* U+00FB "û" */
    0x31, 0x20, 0x0, 0x86, 0x18, 0x61, 0x86, 0x17,
    0xc0,

    /* U+00FC "ü" */
    0x49, 0x20, 0x21, 0x86, 0x18, 0x61, 0x85, 0xf0,

    /* U+00FD "ý" */
    0x8, 0x40, 0x21, 0x86, 0x18, 0x61, 0x85, 0xf0,
    0x41, 0x78,

    /* U+00FE "þ" */
    0x81, 0x2, 0x5, 0xcc, 0x50, 0x60, 0xc1, 0xc5,
    0x72, 0x4, 0x8, 0x0,

    /* U+00FF "ÿ" */
    0x49, 0x20, 0x21, 0x86, 0x18, 0x61, 0x85, 0xf0,
    0x41, 0x78
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 128, .box_w = 1, .box_h = 10, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 128, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 5, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 128, .box_w = 5, .box_h = 11, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 19, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 128, .box_w = 1, .box_h = 4, .ofs_x = 3, .ofs_y = 7},
    {.bitmap_index = 37, .adv_w = 128, .box_w = 3, .box_h = 12, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 42, .adv_w = 128, .box_w = 3, .box_h = 12, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 47, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 51, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 58, .adv_w = 128, .box_w = 2, .box_h = 5, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 60, .adv_w = 128, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 61, .adv_w = 128, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 71, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 78, .adv_w = 128, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 128, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 128, .box_w = 2, .box_h = 10, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 145, .adv_w = 128, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 128, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 154, .adv_w = 128, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 179, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 221, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 228, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 235, .adv_w = 128, .box_w = 5, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 284, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 300, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 128, .box_w = 4, .box_h = 12, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 373, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 382, .adv_w = 128, .box_w = 4, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 388, .adv_w = 128, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 390, .adv_w = 128, .box_w = 8, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 391, .adv_w = 128, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 393, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 427, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 443, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 451, .adv_w = 128, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 458, .adv_w = 128, .box_w = 4, .box_h = 13, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 465, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 488, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 508, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 516, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 528, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 535, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 547, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 554, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 569, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 587, .adv_w = 128, .box_w = 1, .box_h = 13, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 589, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 601, .adv_w = 128, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 604, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 605, .adv_w = 128, .box_w = 1, .box_h = 10, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 607, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 616, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 624, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 630, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 638, .adv_w = 128, .box_w = 1, .box_h = 11, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 640, .adv_w = 128, .box_w = 5, .box_h = 2, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 642, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 651, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 658, .adv_w = 128, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 659, .adv_w = 128, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 663, .adv_w = 128, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 665, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 672, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 680, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 689, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 698, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 707, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 716, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 725, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 734, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 751, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 760, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 769, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 778, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 787, .adv_w = 128, .box_w = 5, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 795, .adv_w = 128, .box_w = 5, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 803, .adv_w = 128, .box_w = 5, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 811, .adv_w = 128, .box_w = 5, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 819, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 827, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 836, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 847, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 858, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 869, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 891, .adv_w = 128, .box_w = 5, .box_h = 5, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 895, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 904, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 913, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 922, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 931, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 940, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 951, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 958, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 965, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 973, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 981, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 990, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 999, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1007, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1016, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1023, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1031, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1039, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1047, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1056, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1064, .adv_w = 128, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1071, .adv_w = 128, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1078, .adv_w = 128, .box_w = 5, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1085, .adv_w = 128, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1092, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1102, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1111, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1119, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1136, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1145, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1153, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1160, .adv_w = 128, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1167, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1175, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1183, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1192, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1200, .adv_w = 128, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1210, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1222, .adv_w = 128, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_1[] = {
    0, 1, 2, 3, 4, 5, 6, 0,
    7, 8, 0, 9, 0, 10, 0, 0,
    11, 0, 0, 0, 0, 0, 0, 0,
    12, 0, 0, 13
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
        .range_start = 191, .range_length = 65, .glyph_id_start = 110,
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
const lv_font_t lv_font_tamzen_16 = {
#else
lv_font_t lv_font_tamzen_16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
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



#endif /*#if LV_FONT_TAMZEN_16*/

