#!/usr/bin/env bash
# check-device.sh — detect a connected M5Stack Tab5 (ESP32-P4) and read its chip id.
#
# Usage:
#   ./tools/check-device.sh                # auto-detect /dev/cu.usbmodem*
#   ./tools/check-device.sh /dev/cu.usbmodem111301
#
# Requires esptool. If not present, this script provisions a local venv at .venv/
# and installs esptool there (no system-wide changes).

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
VENV="$ROOT/.venv"
ESPTOOL=""

# 1. Pick a serial port -------------------------------------------------------
PORT="${1:-}"
if [[ -z "$PORT" ]]; then
  PORT="$(ls /dev/cu.usbmodem* 2>/dev/null | head -n1 || true)"
fi
if [[ -z "$PORT" ]]; then
  echo "No /dev/cu.usbmodem* device found."
  echo "  - Is the Tab5 plugged in over USB-C?"
  echo "  - If the green LED is solid, hold Reset ~2s until it blinks rapidly (download mode)."
  exit 1
fi
echo "Using port: $PORT"

# 2. Locate (or install) esptool ---------------------------------------------
if command -v esptool.py >/dev/null 2>&1; then
  ESPTOOL="esptool.py"
elif command -v esptool >/dev/null 2>&1; then
  ESPTOOL="esptool"
elif [[ -x "$VENV/bin/esptool.py" ]]; then
  ESPTOOL="$VENV/bin/esptool.py"
else
  echo "esptool not found — provisioning local venv at $VENV"
  python3 -m venv "$VENV"
  "$VENV/bin/pip" install --quiet --upgrade pip
  "$VENV/bin/pip" install --quiet esptool
  ESPTOOL="$VENV/bin/esptool.py"
fi
echo "Using esptool: $ESPTOOL"
echo

# 3. Probe --------------------------------------------------------------------
echo "=== chip_id ==="
"$ESPTOOL" --chip esp32p4 --port "$PORT" chip_id || {
  echo
  echo "chip_id failed. Common causes:"
  echo "  - Device is not in download mode → hold Reset 2s until green LED rapid-blinks."
  echo "  - Wrong port → list candidates: ls /dev/cu.usbmodem*"
  echo "  - Permissions → no root needed on macOS, but try unplug/replug."
  exit 1
}
echo
echo "=== flash_id ==="
"$ESPTOOL" --chip esp32p4 --port "$PORT" flash_id || true
