// XIAO-S3-TFT Meshtastic Variant
// Hardware: Seeed XIAO ESP32S3 + Wio-SX1262 baseboard + WeAct 2.8" ILI9341 SPI TFT + XPT2046 touch
// LoRa pinout copied from variants/esp32s3/seeed_xiao_s3 (official Wio-SX1262 support).
// Display/touch/UI logic adapted from variants/esp32s3/diy/handheld-s3 (RFM95W -> SX1262 swap).

#pragma once

// ─── Chip & Flash ────────────────────────────────────────────────────────────
// (Architecture macros defined by PlatformIO)

// ─── USB ─────────────────────────────────────────────────────────────────────
#define USES_CDC_SERIAL

// ─── SPI (Wio-SX1262 hardware SPI bus, shared with ILI9341 + XPT2046) ────────
#define HW_SPI_SCK      7
#define HW_SPI_MISO     8
#define HW_SPI_MOSI     9
#define SPI_SCK         HW_SPI_SCK
#define SPI_MISO        HW_SPI_MISO
#define SPI_MOSI        HW_SPI_MOSI

// ─── LoRa SX1262 (Wio-SX1262 baseboard) ──────────────────────────────────────
#define USE_SX1262

#define LORA_SCK        HW_SPI_SCK
#define LORA_MISO       HW_SPI_MISO
#define LORA_MOSI       HW_SPI_MOSI
#define LORA_CS         41
#define LORA_RESET      42
#define LORA_DIO1       39

#define SX126X_CS               LORA_CS
#define SX126X_DIO1              LORA_DIO1
#define SX126X_BUSY             40
#define SX126X_RESET            LORA_RESET
// DIO2 controls an antenna switch; DIO3 controls TCXO voltage (Wio-SX1262 baseboard)
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_RXEN              38
#define SX126X_TXEN              RADIOLIB_NC
#define SX126X_DIO3_TCXO_VOLTAGE 1.8

// ─── ILI9341 Display (WeAct 2.8", shares SPI bus with LoRa) ──────────────────
#define TFT_SCK         HW_SPI_SCK
#define TFT_MOSI        HW_SPI_MOSI
#define TFT_MISO        HW_SPI_MISO
#define TFT_CS          1    // XIAO D0
#define TFT_DC          2    // XIAO D1
#define TFT_RST         3    // XIAO D2
#define TFT_BL          4    // XIAO D3, backlight PWM

// ─── XPT2046 Touch Controller (shares SPI bus, polling mode — no IRQ pin) ────
#define TOUCH_CS        5    // XIAO D4
#define SCREEN_TOUCH_INT -1  // not wired, polled via SPI

// ─── Button ──────────────────────────────────────────────────────────────────
#define BUTTON_PIN      21   // XIAO onboard BOOT button
#define BUTTON_NEED_PULLUP

// ─── Power ───────────────────────────────────────────────────────────────────
// No PMU — USB powered or raw LiPo with external regulator
#undef HAS_AXP192
#undef HAS_AXP2101

// ─── No GPS / SD / CardKB / sensor / encoder / RTC / buzzer / WS2812 ─────────
// XIAO D5/D6/D7 (GPIO6/43/44) are left free for future expansion (I2C, GPS, etc.)
#undef HAS_RTC
#undef HAS_BUZZER
#undef HAS_WS2812
#undef INPUTBROKER_MATRIX_TYPE

// ─── MUI / LVGL ──────────────────────────────────────────────────────────────
#define USE_EINK_DYNAMICDISPLAY 0
#define HAS_SCREEN      1

// ─── Misc ────────────────────────────────────────────────────────────────────
#define LED_PIN         -1   // no onboard status LED wired
