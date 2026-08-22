# XIAO-S3-TFT — Meshtastic Node with Touchscreen UI

A compact Meshtastic node built on the Seeed **XIAO ESP32S3 + Wio-SX1262** kit, with a
**WeAct 2.8" ILI9341 SPI TFT (320×240) + XPT2046 resistive touch** bolted on for the full
MUI/LVGL touchscreen interface.

Combines two existing references:
- LoRa + board pinout: [`variants/esp32s3/seeed_xiao_s3`](../../seeed_xiao_s3) (official Wio-SX1262 support)
- Display/touch/MUI logic: [`variants/esp32s3/diy/handheld-s3`](../handheld-s3) (RFM95W swapped for SX1262)

---

## Hardware BOM

| Component | Part | Notes |
|---|---|---|
| MCU | Seeed XIAO ESP32S3 | 8 MB flash, 8 MB PSRAM |
| Radio | Wio-SX1262 baseboard | SPI, onboard SX1262, 915/868/433 MHz per region |
| Display | WeAct 2.8" TFT, ILI9341 | SPI, 240×320, shared bus |
| Touch | XPT2046 | SPI, shared bus, polling mode (no IRQ pin used) |

---

## Pin Assignments

### SPI bus (hardware SPI — shared by LoRa, TFT, and touch)

| Signal | GPIO | XIAO header |
|---|---|---|
| SCK | 7 | D8 |
| MOSI | 9 | D10 |
| MISO | 8 | D9 |

### LoRa SX1262 (Wio-SX1262 baseboard — internal pads, not on header)

| Signal | GPIO |
|---|---|
| CS | 41 |
| RESET | 42 |
| DIO1 | 39 |
| BUSY | 40 |
| RXEN / DIO2 (RF switch) | 38 |

### ILI9341 (Display)

| Signal | GPIO | XIAO header |
|---|---|---|
| CS | 1 | D0 |
| DC | 2 | D1 |
| RST | 3 | D2 |
| Backlight | 4 | D3 |

### XPT2046 (Touch)

| Signal | GPIO | XIAO header |
|---|---|---|
| CS | 5 | D4 |
| INT | not wired | polling mode via SPI |

### Free for expansion

XIAO header pins D5 (GPIO6), D6 (GPIO43), D7 (GPIO44) are unused — available for I2C sensors,
GPS, a button, etc.

---

## Wiring the WeAct 2.8" TFT to XIAO

| TFT pin | XIAO pin |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SCK | D8 |
| MOSI (SDI) | D10 |
| MISO (SDO) | D9 |
| CS | D0 |
| DC | D1 |
| RST | D2 |
| LED (backlight) | D3 |
| T_CS | D4 |
| T_CLK | D8 (shared with SCK) |
| T_DIN | D10 (shared with MOSI) |
| T_DO | D9 (shared with MISO) |
| T_IRQ | not connected |

> All SPI CS lines (LoRa, TFT, touch) are pulled HIGH in `initVariant()` before the bus
> starts, matching the T-Deck/handheld-s3 boot-safety pattern — otherwise a floating CS
> can corrupt the first SPI transaction.

---

## Build & Flash

### Prerequisites

- [PlatformIO](https://platformio.org/) (CLI or VSCode extension)
- This repo cloned: `git clone --recurse-submodules https://github.com/meshtastic/firmware`

### Add the environment

Merge `[env:xiao-s3-tft]` from `platformio.ini` into the root `platformio.ini` of the
Meshtastic firmware repo.

### Build & upload

```bash
pio run -e xiao-s3-tft -t upload
```

XIAO ESP32S3 needs to be put in bootloader mode for the first flash (double-tap reset,
or hold BOOT while tapping RESET) if `esptool` can't auto-reset it.

---

## Known Limitations

- Touch is polling-mode only (no IRQ line wired) — slightly higher latency than
  interrupt-driven touch, same tradeoff as handheld-s3.
- No GPS, SD card, or environmental sensor wired in this base config — D5/D6/D7 are
  free if you want to add any of those later (see handheld-s3 for a working example
  of GPS + BME280 + SD wiring, though pin numbers will differ).
- Untested on real hardware — this variant was scaffolded from two existing, working
  variants (`seeed_xiao_s3` for LoRa/board, `handheld-s3` for display/MUI) but the
  combination has not yet been build-tested or bench-tested.

---

## Dependencies

- [Meshtastic firmware](https://github.com/meshtastic/firmware) — develop branch
- [LovyanGFX](https://github.com/lovyan03/LovyanGFX) 1.2.21
