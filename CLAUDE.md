# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a PlatformIO project for an ESP32S NodeMCU board that reads a PIR motion sensor (KS0052) and transmits motion events over LoRa radio at 433 MHz in JSON format.

## Build & Flash Commands

```bash
# Build the project
pio run

# Upload to device
pio run --target upload

# Open serial monitor (115200 baud)
pio device monitor

# Build and upload in one step
pio run --target upload && pio device monitor
```

## Hardware

**Target board:** `nodemcu-32s` (ESP32S)

**LoRa module:** SX1278 (RA-02) wired to ESP32 via SPI:
- SCK → GPIO18, MOSI → GPIO23, MISO → GPIO19
- NSS (SS) → GPIO5, RST → GPIO2, DIO0 → GPIO4

**PIR sensor:** KS0052 → GPIO35 (input only pin)

## LoRa Configuration

- Frequency: 433 MHz
- Spreading Factor: 7
- Bandwidth: 125 kHz
- Coding Rate: 4/5
- Sync Word: 0x12
- Preamble: 8
- TX Power: 20 dBm
- Max payload: 128 bytes

## Message Format

All LoRa messages are JSON. Two message types:

**Alive (sent on boot):**
```json
{"device":"MCUDEVICE-<efuse_mac>","type":"iamalive","mac":"xx:xx:xx:xx:xx:xx"}
```

**Motion event:**
```json
{"motion":0,"sensor":"ks0052","device":"MCUDEVICE-<efuse_mac>","mac":"xx:xx:xx:xx:xx:xx"}
```
`motion` is `1` (detected) or `0` (stopped). Events are only published on state change.

## Dependencies

Managed by PlatformIO (`lib_deps` in `platformio.ini`):
- `sandeepmistry/LoRa` — LoRa radio driver
- `adafruit/Adafruit SSD1306` — OLED display (declared but not used in current code)
- `adafruit/Adafruit GFX Library` — Graphics (declared but not used in current code)
