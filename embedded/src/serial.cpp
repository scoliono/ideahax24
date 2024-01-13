#include "serial.hpp"
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
    Serial.read(this->json, MAX_PAYLOAD_LEN);
    DynamicJsonDocument doc(MAX_PAYLOAD_LEN);
    DeserializationError error = deserializeJson(doc, this->json);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    out->name = doc["name"];
    out->bdayYear = doc["bdayYear"];
    out->gender = doc["gender"];
    out->desiredGender = doc["desiredGender"];
    out->goal = doc["goal"];
    out->similarity = doc["similarity"];

    sendProfileAck();
    return true;
}

void PemdasSerial::sendProfileAck()
{
    Serial.write(this->json);
}
