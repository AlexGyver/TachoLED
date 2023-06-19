#include <Arduino.h>

#include "data.h"
#include "hub.h"
#include "led.h"
#include "tacho.h"

void setup() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("TachoLED");

    memory.begin(0, 'b');
    led_init();
    led_change();
    hub_init();
    tacho_init();
}

void loop() {
    hub_tick();
    led_tick();
    tacho_tick();
    memory.tick();
}