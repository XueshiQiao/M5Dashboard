# VIBEHUB alternate UI

Landscape adaptation of the colorful terminal mockup.

The active firmware UI is untouched. To test this version, copy these two
files over the active screen files:

```bash
cd firmware
cp alternate-ui/vibehub/screen_grid.cpp src/screen_grid.cpp
cp alternate-ui/vibehub/screen_grid.h src/screen_grid.h
pio run
```

The layout keeps the same firmware data contract:

- Weather uses `/api/weather` plus the PNG weather icon hook.
- Claude and Codex use the existing usage envelope and brand icon hooks.
- News uses `/api/news`.
- The todo block is static for now; there is no todo endpoint in the server.

Compared with the generated portrait concept, this version compresses the
large ASCII logo into a top prompt/status bar and uses a 1280x720 cockpit
layout so text remains readable on the Tab5.
