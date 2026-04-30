#pragma once

#include <stdint.h>

// Single-file Tab5 power-state helper.
//
// Usage in one .cpp file:
//   #define TAB5_POWER_STATE_IMPLEMENTATION
//   #include "tab5_power_state.h"
//
// Other files can include this header normally for the enum/types.

namespace tab5_power {

enum class State : uint8_t {
  NoBatteryNoCable,
  NoBatteryCable,
  BatteryOnly,
  BatteryCable,
};

struct Snapshot {
  State state;
  bool battery_attached;
  bool cable_connected;
  bool charging;
  int battery_pct;
  int battery_mv;
  int battery_current_ma;
};

constexpr int kBatteryPresentMv = 5800;

inline bool batteryAttachedFromMillivolts(int battery_mv) {
  // Tab5 uses a 2S NP-F550 pack. Real pack voltage is roughly 6.0-8.4 V.
  // With no pack installed, the INA226 path may still see USB/external rails,
  // so anything below this threshold is not treated as an attached battery.
  return battery_mv >= kBatteryPresentMv;
}

inline int clampPercent(int pct) {
  if (pct < 0) return 0;
  if (pct > 100) return 100;
  return pct;
}

inline State classify(bool battery_attached, bool cable_connected) {
  if (battery_attached && cable_connected) return State::BatteryCable;
  if (battery_attached) return State::BatteryOnly;
  if (cable_connected) return State::NoBatteryCable;
  return State::NoBatteryNoCable;
}

inline const char* stateName(State state) {
  switch (state) {
    case State::BatteryCable: return "battery+cable";
    case State::BatteryOnly: return "battery";
    case State::NoBatteryCable: return "cable";
    case State::NoBatteryNoCable: return "none";
  }
  return "unknown";
}

Snapshot read();
State readState();

}  // namespace tab5_power

#ifdef TAB5_POWER_STATE_IMPLEMENTATION

#include <Arduino.h>
#include <M5Unified.h>

namespace tab5_power {
namespace {

bool readCableConnected(bool battery_attached) {
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE && \
    defined(ARDUINO_USB_CDC_ON_BOOT) && ARDUINO_USB_CDC_ON_BOOT
  if (Serial.isPlugged()) return true;
#endif

  // If the pack rail is absent but firmware is still running, the Tab5 is
  // externally powered. This covers the visible "no battery + cable" state.
  return !battery_attached;
}

}  // namespace

Snapshot read() {
  const int battery_mv = M5.Power.getBatteryVoltage();
  const bool battery_attached = batteryAttachedFromMillivolts(battery_mv);
  const bool cable_connected = readCableConnected(battery_attached);
  const State state = classify(battery_attached, cable_connected);

  Snapshot snap = {};
  snap.state = state;
  snap.battery_attached = battery_attached;
  snap.cable_connected = cable_connected;
  snap.charging = state == State::BatteryCable;
  snap.battery_pct = clampPercent(M5.Power.getBatteryLevel());
  snap.battery_mv = battery_mv;
  snap.battery_current_ma = M5.Power.getBatteryCurrent();
  return snap;
}

State readState() {
  return read().state;
}

}  // namespace tab5_power

#endif  // TAB5_POWER_STATE_IMPLEMENTATION
