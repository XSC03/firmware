// hid_dump — dumps Rii i8 HID reports to UART on GPIO0/1
// Dongle plugs into RP2040 Zero USB port (native host)
// USB-TTL RX → GPIO0, GND → GND
#ifndef ARDUINO_ARCH_ESP32

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   16
Adafruit_NeoPixel pixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_USBH_Host USBHost;

void setup() {
    pixel.begin();
    pixel.setPixelColor(0, pixel.Color(0, 0, 50)); // blue = waiting for dongle
    pixel.show();

    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);
    delay(500);
    Serial1.println("HID Dump ready — plug in Rii i8 dongle");
}

void loop() {
    delay(1);
}

void setup1() {
    USBHost.begin(0);
}

void loop1() {
    USBHost.task();
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance,
                       uint8_t const* desc_report, uint16_t desc_len) {
    (void)desc_report; (void)desc_len;
    uint8_t proto = tuh_hid_interface_protocol(dev_addr, instance);
    pixel.setPixelColor(0, pixel.Color(0, 50, 0)); // green = dongle connected
    pixel.show();
    Serial1.printf("[MOUNT] dev=%u inst=%u proto=%u", dev_addr, instance, proto);
    switch (proto) {
        case HID_ITF_PROTOCOL_KEYBOARD: Serial1.println(" (KEYBOARD)"); break;
        case HID_ITF_PROTOCOL_MOUSE:    Serial1.println(" (MOUSE)");    break;
        default:                         Serial1.println(" (OTHER)");    break;
    }
    tuh_hid_receive_report(dev_addr, instance);
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    pixel.setPixelColor(0, pixel.Color(0, 0, 50)); // blue = disconnected
    pixel.show();
    Serial1.printf("[UMOUNT] dev=%u inst=%u\n", dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance,
                                  uint8_t const* report, uint16_t len) {
    uint8_t proto = tuh_hid_interface_protocol(dev_addr, instance);

    if (proto == HID_ITF_PROTOCOL_KEYBOARD) {
        if (len < 3) { tuh_hid_receive_report(dev_addr, instance); return; }
        uint8_t mod = report[0];
        bool any_key = false;
        for (int i = 2; i < (int)len; i++) { if (report[i]) { any_key = true; break; } }
        if (!any_key && mod == 0) { tuh_hid_receive_report(dev_addr, instance); return; }

        pixel.setPixelColor(0, pixel.Color(50, 50, 0)); // yellow = keypress
        pixel.show();

        Serial1.printf("KBD mod=0x%02X keys=[", mod);
        for (int i = 2; i < (int)len; i++) {
            if (report[i] == 0) break;
            Serial1.printf("0x%02X ", report[i]);
        }
        Serial1.print("] (");
        if (mod & 0x01) Serial1.print("LCtrl ");
        if (mod & 0x02) Serial1.print("LShift ");
        if (mod & 0x04) Serial1.print("LAlt ");
        if (mod & 0x08) Serial1.print("LWin ");
        if (mod & 0x10) Serial1.print("RCtrl ");
        if (mod & 0x20) Serial1.print("RShift ");
        if (mod & 0x40) Serial1.print("RAlt ");
        if (mod & 0x80) Serial1.print("RWin ");
        Serial1.println(")");

    } else if (proto == HID_ITF_PROTOCOL_MOUSE) {
        // ignore mouse
    } else {
        bool any = false;
        for (int i = 0; i < (int)len; i++) { if (report[i]) { any = true; break; } }
        if (any) {
            Serial1.printf("HID[proto=%u] [", proto);
            for (int i = 0; i < (int)len; i++) Serial1.printf("0x%02X ", report[i]);
            Serial1.println("]");
        }
    }

    tuh_hid_receive_report(dev_addr, instance);
}

#endif
