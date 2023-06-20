#pragma once
#include <Arduino.h>

#define GH_NO_MQTT
#include <GyverHub.h>

#include "data.h"
#include "palettes.h"
#include "tacho.h"

GyverHub hub("MyDevices", "TachoLED", "");

static int _analog_f = VBAT_MAX;

static int getBat() {
    int vbat = map(_analog_f, VBAT_MIN, VBAT_MAX, 0, 100);
    return constrain(vbat, 0, 100);
}

void build() {
    bool flag = 0;
    hub.Title(F("Status"));
    hub.BeginWidgets();
    hub.WidgetSize(50);
    hub.Gauge_(F("spd"), tacho_kmh(), F("kmh"), F("Speed"), 0, data.max_spd);
    hub.Gauge(getBat(), F("%"), F("Battery"));

    hub.Title(F("Mode"));
    hub.WidgetSize(100);
    flag |= hub.Tabs(&data.mode, F("Pallette,Color"), F("Mode"));
    
    if (data.mode == 0) {
        hub.WidgetSize(50);
        flag |= hub.Input(&data.pal_len, GH_UINT16, F("Length [leds]"));
        flag |= hub.Select(&data.pal, FPSTR(paletteNames));
    }

    hub.Title(F("Settings"));
    hub.WidgetSize(50);
    flag |= hub.Input(&data.led_amount, GH_UINT16, F("LED amount"));
    flag |= hub.Input(&data.diam, GH_UINT16, F("Diameter [mm]"));
    flag |= hub.Input(&data.ledm, GH_UINT8, F("leds/m"));
    flag |= hub.Input(&data.max_spd, GH_UINT8, F("Max speed [kmh]"));
    hub.WidgetSize(100);
    flag |= hub.Slider(&data.led_bri, GH_UINT8, F("Brightness"), 0, 255);

    if (flag) memory.update();
}

void hub_init() {
    hub.onBuild(build);
    hub.begin();
}

void hub_tick() {
    hub.tick();

    // filter analog read
    static GHtimer vbat(500);
    if (vbat) _analog_f += (analogRead(0) - _analog_f) / 4;

    static GHtimer upd(1000);
    if (upd) {
        hub.sendUpdate(F("spd"), String(tacho_kmh()));
    }
}
