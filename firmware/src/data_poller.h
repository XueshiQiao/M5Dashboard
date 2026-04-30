// data_poller — pulls /api/weather + /api/claude on a background task,
// stages results behind a mutex. The main loop calls drain() (single-
// threaded with LVGL) to apply staged snapshots to the screen.

#pragma once

void poller_start();   // call once after lvgl_bridge_init + buildGridScreen
void poller_drain();   // call every loop tick; cheap when nothing's new
