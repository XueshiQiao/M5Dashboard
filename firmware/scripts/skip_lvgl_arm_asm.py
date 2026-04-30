"""Pre-build patches for LVGL + M5GFX coexistence on ESP32-P4.

Three issues to neutralise before compilation:

(1) LVGL's ARM-Helium blend assembly (src/draw/sw/blend/helium/*.S) is
    fed through `gcc -x assembler-with-cpp` even on non-ARM targets. The
    preprocessor expands #include <stdint.h>, then the RISC-V assembler
    chokes on the typedefs. Macro flags don't help — the offending
    includes sit above the conditional compile.

(2) M5GFX bundles a stripped copy of LVGL's font helpers in
    src/lgfx/v1/lv_font/font_fmt_txt.c (defines lv_utils_bsearch,
    lv_font_get_glyph_dsc_fmt_txt, etc). LVGL ships the same symbols.
    Result: linker complains about multiple definitions.

(3) M5GFX bundles its own copies of the Montserrat / SimSun / unscii
    LVGL fonts (src/lgfx/Fonts/lvgl/lv_font_*.c). LVGL ships the same
    `lv_font_montserrat_<N>` globals when the corresponding macros are
    enabled in lv_conf.h. Same multiple-definition problem.

Strategy: stub the .c files (and the .S files) with empty bodies. M5GFX's
HEADERS stay intact so its own source compiles; only the duplicate
definitions are removed. Our project doesn't draw text via M5.Display, so
losing M5GFX's font data is no functional loss — LVGL renders all text.
"""

import glob
import os

Import("env")  # type: ignore[name-defined]   # provided by PIO at script load

LIBDEPS = env.subst("$PROJECT_LIBDEPS_DIR/$PIOENV")  # type: ignore[name-defined]


def stub_files(pattern, label, body):
    for path in glob.glob(pattern, recursive=True):
        try:
            with open(path, "r") as fp:
                if fp.read() == body:
                    continue  # already stubbed
        except OSError:
            pass
        with open(path, "w") as fp:
            fp.write(body)
        print(f"[{label}] stubbed {os.path.relpath(path)}")


# (1) LVGL Helium ARM-only assembly
stub_files(
    os.path.join(LIBDEPS, "lvgl/src/draw/sw/blend/helium/*.S"),
    "skip_lvgl_arm_asm",
    "/* stub: lv_blend_helium is ARM-Helium only, not RISC-V */\n",
)

# (2) M5GFX's LVGL helper duplicates  (tolerate either of the two
# possible M5GFX checkout paths PIO may create)
for m5gfx_root_glob in ("M5GFX", "M5GFX@*"):
    stub_files(
        os.path.join(LIBDEPS, m5gfx_root_glob, "src/lgfx/v1/lv_font/font_fmt_txt.c"),
        "skip_m5gfx_lvgl_fmt",
        "/* stub: duplicate of LVGL's lv_font_fmt_txt.c */\n",
    )
    # (3) M5GFX's bundled LVGL font data
    stub_files(
        os.path.join(LIBDEPS, m5gfx_root_glob, "src/lgfx/Fonts/lvgl/lv_font_*.c"),
        "skip_m5gfx_lvgl_font",
        "/* stub: LVGL provides this font; M5GFX copy duplicates it */\n",
    )
