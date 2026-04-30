// hello-tab5 — minimal end-to-end test for the M5Stack Tab5.
//
// What it verifies:
//   1. Toolchain can build for ESP32-P4 + Arduino + M5Unified
//   2. Firmware flashes over native USB-JTAG to /dev/cu.usbmodem*
//   3. Display (ST7123) renders text — confirms graphics pipeline
//   4. Touch (ST7123) reports coordinates — confirms input pipeline
//   5. Power monitor (INA226) reports battery state — confirms I2C bus
//   6. Serial monitor receives uptime ticks — confirms USB CDC
//
// If you see "Hello, M5Dashboard!" on the screen, hear the speaker, can tap
// to draw cyan dots, and the serial monitor scrolls battery readings, the
// toolchain + hardware are good. We can move on to building the real app.

#include <M5Unified.h>

static uint32_t last_status_ms = 0;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  // Give USB-CDC a moment to enumerate after boot before first print.
  delay(300);
  Serial.println();
  Serial.println("=== M5Dashboard hello-tab5 ===");
  Serial.printf("Display: %dx%d\n", M5.Display.width(), M5.Display.height());
  Serial.printf("Has touch: %s\n", M5.Touch.isEnabled() ? "yes" : "no");
  Serial.printf("Battery: %d%%  voltage: %.2fV\n",
                M5.Power.getBatteryLevel(),
                M5.Power.getBatteryVoltage() / 1000.0f);

  M5.Display.setBrightness(160);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setTextDatum(middle_center);

  M5.Display.setFont(&fonts::FreeSansBold24pt7b);
  M5.Display.drawString("Hello, M5Dashboard!",
                        M5.Display.width() / 2,
                        M5.Display.height() / 2 - 60);

  M5.Display.setFont(&fonts::FreeSans18pt7b);
  M5.Display.setTextColor(TFT_DARKGRAY, TFT_BLACK);
  M5.Display.drawString("Tap anywhere to draw",
                        M5.Display.width() / 2,
                        M5.Display.height() / 2 + 20);

  // A quick chirp to confirm the audio path is alive too.
  M5.Speaker.tone(1000, 80);
}

void loop() {
  M5.update();

  // Touch → draw a cyan dot wherever the user pressed.
  auto count = M5.Touch.getCount();
  for (size_t i = 0; i < count; ++i) {
    auto detail = M5.Touch.getDetail(i);
    if (detail.wasPressed()) {
      Serial.printf("Touch[%u] pressed at (%d, %d)\n",
                    (unsigned)i, detail.x, detail.y);
      M5.Display.fillCircle(detail.x, detail.y, 18, TFT_CYAN);
    }
  }

  // Once a second: heartbeat + power readout.
  uint32_t now = millis();
  if (now - last_status_ms >= 1000) {
    last_status_ms = now;
    Serial.printf("uptime=%lus  batt=%d%%  V=%.2f  I=%.0fmA\n",
                  (unsigned long)(now / 1000),
                  M5.Power.getBatteryLevel(),
                  M5.Power.getBatteryVoltage() / 1000.0f,
                  M5.Power.getBatteryCurrent());
  }
}
