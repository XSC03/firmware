// LovyanGFX driver config for ILI9341 2.8" 320x240 + XPT2046 touch on XIAO ESP32S3 + Wio-SX1262
// Variant-local file — do not modify core src/
// Copied from variants/esp32s3/diy/handheld-s3/lgfx_user_config.h with pins remapped.

#pragma once
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ILI9341  _panel_instance;
    lgfx::Bus_SPI        _bus_instance;
    lgfx::Light_PWM      _light_instance;
#if defined(USE_XPT2046)
    lgfx::Touch_XPT2046  _touch_instance;
#endif

public:
    LGFX(void) {
        // ── SPI bus config (shared with LoRa SX1262, different CS) ────────────
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host   = SPI2_HOST;
            cfg.spi_mode   = 0;
            cfg.freq_write = 20000000;
            cfg.freq_read  = 16000000;
            cfg.spi_3wire  = false;
            cfg.use_lock   = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk   = HW_SPI_SCK;
            cfg.pin_mosi   = HW_SPI_MOSI;
            cfg.pin_miso   = HW_SPI_MISO;
            cfg.pin_dc     = TFT_DC;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        // ── Panel config ──────────────────────────────────────────────────────
        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs        = TFT_CS;
            cfg.pin_rst       = TFT_RST;
            cfg.pin_busy      = -1;
            cfg.panel_width   = 240;
            cfg.panel_height  = 320;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;
            cfg.offset_rotation = 1;  // landscape
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits  = 1;
            cfg.readable      = true;
            cfg.invert        = false;
            cfg.rgb_order     = false;
            cfg.dlen_16bit    = false;
            cfg.bus_shared    = true;  // shares SPI with SX1262 LoRa radio
            _panel_instance.config(cfg);
        }

        // ── Backlight config ──────────────────────────────────────────────────
        {
            auto cfg = _light_instance.config();
            cfg.pin_bl        = TFT_BL;
            cfg.invert        = false;
            cfg.freq          = 44100;
            cfg.pwm_channel   = 7;
            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

#if defined(USE_XPT2046)
        // ── Touch config (XPT2046, shares SPI bus with TFT + LoRa) ───────────
        // No IRQ pin wired — driver polls via SPI only (pin_int = -1).
        {
            auto cfg = _touch_instance.config();
            cfg.x_min        = 300;
            cfg.x_max        = 3900;
            cfg.y_min        = 300;
            cfg.y_max        = 3900;
            cfg.pin_int      = -1;
            cfg.pin_cs       = TOUCH_CS;          // defined in variant.h
            cfg.bus_shared   = true;              // shares SPI with TFT + LoRa
            cfg.offset_rotation = 1;              // match display landscape
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
#endif

        setPanel(&_panel_instance);
    }
};
