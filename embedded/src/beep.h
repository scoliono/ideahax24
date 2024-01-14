#ifndef BEEP_H
#define BEEP_H

#include <Arduino.h>
#include "constants.h"

void beep(int totalwidth, int cycwidth) {
    int cycles = totalwidth/cycwidth;
    for(int i = 0; i < cycles; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(cycwidth/2);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(cycwidth/2);
    }
}

void beepDown() {
    beep(100000, 1000);
    delay(100);
    beep(100000, 2000);
    beep(100000, 2000);
}

void beepUp() {
    beep(100000, 2000);
    beep(100000, 1000);
}

void beepError() {
    beep(100000, 2000);
    delayMicroseconds(100000);
    beep(100000, 2000);
    delayMicroseconds(100000);
    beep(100000, 2000);
}

void playFreq(int freq, int ms) {
    int period = 1000000/freq;
    int iter = ms *1000 / period;
    for (int i = 0; i < iter; i++){
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(period/2);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(period/2);
    }
}

void playTune() {
    playFreq(880, 250);
    playFreq(988, 250);
    playFreq(1109, 250);
    playFreq(988, 250);
    playFreq(880, 250);
    playFreq(831, 250);
    playFreq(740, 250);
    playFreq(988, 250);
    playFreq(831, 250);
    playFreq(659, 250);
    playFreq(880, 1000);
}

#endif
