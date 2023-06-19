#pragma once
#include <Arduino.h>
#include <EEManager.h>

#define TACHO_PIN D1
#define LED_PIN D2
#define LED_CHIP WS2812
#define LED_ORDER GRB
#define VBAT_MAX 630
#define VBAT_MIN 500
#define LED_PRD 30

struct Data {
    uint16_t led_amount = 26;
    uint16_t diam = 68;
    uint16_t pal_len = 60;
    uint8_t led_bri = 255;
    uint8_t ledm = 60;
    uint8_t mode = 0;
    uint8_t max_spd = 25;
    uint8_t pal = 0;
};

Data data;
EEManager memory(data);