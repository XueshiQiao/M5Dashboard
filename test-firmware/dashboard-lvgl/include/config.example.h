// Template — copy to include/config.h and fill in real values.

#pragma once

namespace cfg {
constexpr const char* WIFI_SSID     = "your-ssid";
constexpr const char* WIFI_PASSWORD = "your-password";
constexpr long  GMT_OFFSET_SEC      = -8 * 3600;
constexpr int   DST_OFFSET_SEC      =  3600;
constexpr const char* NTP_SERVER    = "pool.ntp.org";
constexpr int   BRIGHTNESS_ACTIVE   = 160;
constexpr int   BRIGHTNESS_DIM      =  30;
constexpr uint32_t IDLE_DIM_MS      = 5 * 60 * 1000;

// Dashboard server. SERVER_URL is your Mac's LAN IP + port (default 8787).
// AUTH_TOKEN must match the server's .env (`openssl rand -hex 24`).
constexpr const char* SERVER_URL = "http://192.168.x.x:8787";
constexpr const char* AUTH_TOKEN = "paste-token-from-server-dot-env";
}
