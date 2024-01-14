#include "serial.hpp"
#include "constants.h"
#include <Arduino.h>

PemdasSerial::PemdasSerial()
{
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Initialized Serial");
}

PemdasSerial::~PemdasSerial()
{
    Serial.end();
}

bool PemdasSerial::recvProfile(struct Profile *out)
{
    while (!Serial.available())
        ;

    DynamicJsonDocument doc(JSON_DOC_LEN);
    DeserializationError error = deserializeJson(doc, Serial);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    Profile prof = Profile::fromJSON(doc);
    *out = prof;

    sendProfileAck(out);
    return true;
}

void PemdasSerial::sendProfileAck(struct Profile* prof)
{
    StaticJsonDocument<JSON_DOC_LEN> doc;
    prof->toJSON(doc);
    serializeJson(doc, Serial);

    // ignore extraneous bytes
    while (Serial.available())
        Serial.read();
}
