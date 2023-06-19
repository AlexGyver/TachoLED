#pragma once
#include <Arduino.h>

#include "data.h"
#include "hub.h"

static volatile uint32_t _prev;
static volatile uint32_t _pulse;
static volatile uint32_t _tprd;
static uint16_t _speed, _speed_f;

IRAM_ATTR void tachoISR() {
    uint32_t us = micros();
    if (!_tprd) _tprd = us - _prev;
    _prev = us;
    _pulse++;
}

void tacho_init() {
    attachInterrupt(TACHO_PIN, tachoISR, FALLING);
    pinMode(TACHO_PIN, INPUT_PULLUP);
}

uint16_t tacho_speed() {
    return _speed_f;
}
uint16_t tacho_kmh() {
    return _speed_f / 278;
}
uint32_t tacho_pulse() {
    return _pulse;
}

void tacho_tick() {
    if (_tprd) {
        _speed = (data.diam * 3141592ul) / _tprd;   // (2*Pi*D / (prd / 1000000))
        _tprd = 0;
    }
    
    static GHtimer tmr(50);
    if (tmr) {
        _speed_f += (_speed - _speed_f) >> 2;
        if (_speed && micros() - _prev > 1000000) _speed = _speed_f = 0;
    }
}