#include <Arduino.h>
#include <ArduinoJson.h>
#include <stdint.h>

#include "constants.h"
#include "bt.hpp"
#include "disp.hpp"
#include "serial.hpp"


PemdasSerial* srl;
PemdasBluetooth* bt;
Profile* me;

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    srl = new PemdasSerial;
    me = new Profile;
    /*me = new Profile{
        .name = "James S",
        .bdayYear = 2002,
        .gender = 2,
        .desiredGender = 7,
        .goal = 100,
        .similarity = "0010010000010000001000000010001000001010010100100001100010000101001000010"
    };*/

    // blocks until profile is transferred
    //srl->recvProfile(me);

    // bt needs to know our own profile
    bt = new PemdasBluetooth(me);

    displayInit();
}

void loop() {
    bt->loop();
    delay(10);
}
