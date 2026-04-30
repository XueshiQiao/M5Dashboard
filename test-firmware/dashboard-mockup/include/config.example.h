// config.example.h — template for include/config.h.
//
// Copy this file to include/config.h and fill in real values. The real
// config.h is gitignored so secrets don't leave the device.
//
// In v2 we'll move this to a JSON file in LittleFS edited via an HTTP
// admin page; for now (fake-data mockup) a header is enough.

#pragma once

namespace cfg {

// Wi-Fi
constexpr const char* WIFI_SSID     = "your-ssid";
constexpr const char* WIFI_PASSWORD = "your-password";

// Clock
constexpr long  GMT_OFFSET_SEC    = -8 * 3600;  // PST
constexpr int   DST_OFFSET_SEC    =  3600;      // PDT (1 hour)
constexpr const char* NTP_SERVER  = "pool.ntp.org";

// UI
constexpr int   BRIGHTNESS_ACTIVE = 160;        // 0..255
constexpr int   BRIGHTNESS_DIM    =  30;
constexpr uint32_t IDLE_DIM_MS    = 5 * 60 * 1000;

}  // namespace cfg
