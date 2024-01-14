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

void playTune() {
    tone(15, 880, 250);
    tone(15, 988, 250);
    tone(15, 1109, 250);
    tone(15, 988, 250);
    tone(15, 880, 250);
    tone(15, 831, 250);
    tone(15, 740, 250);
    tone(15, 988, 250);
    tone(15, 831, 250);
    tone(15, 659, 250);
    tone(15, 880, 1000);
}

#endif
