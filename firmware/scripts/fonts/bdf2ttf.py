import fontforge, sys
src, dst = sys.argv[1], sys.argv[2]
f = fontforge.open(src)
f.bitmapSizes = (16, 20)
f.generate(dst, flags=("opentype","apple"))
