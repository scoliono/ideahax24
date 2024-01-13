#include <Arduino.h>
#include <stdint.h>

#include "constants.h"
#include "serial.hpp"


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



PemdasSerial* srl;
Profile* me;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  srl = new PemdasSerial;
  me = new Profile;
}

void loop() {
    srl->recvProfile(me);
    // beepError();
}
