// ESP32 I2C reader — polls RP2040 CardKB emulator at 0x5F
// Wiring:
//   ESP32 GPIO21 (SDA) → RP2040 GPIO4
//   ESP32 GPIO22 (SCL) → RP2040 GPIO5
//   ESP32 GND          → RP2040 GND

#include <Wire.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define CARDKB_ADDR 0x5F

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector
    Serial.begin(115200);
    Wire.begin(21, 22);
    Serial.println("CardKB I2C reader ready — press keys on Rii i8");
}

void loop() {
    Wire.requestFrom(CARDKB_ADDR, 1);
    if (Wire.available()) {
        uint8_t key = Wire.read();
        if (key != 0x00) {
            Serial.printf("KEY: 0x%02X (%d) '%c'\n", key, key, (key >= 0x20 && key < 0x7F) ? key : '?');
        }
    }
    delay(50);
}
