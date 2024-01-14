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

    out->name = doc["name"];
    out->bdayYear = doc["bdayYear"];
    out->gender = doc["gender"];
    out->desiredGender = doc["desiredGender"];
    out->goal = doc["goal"];
    out->similarity = doc["similarity"];

    sendProfileAck(out);
    return true;
}

void PemdasSerial::sendProfileAck(struct Profile* prof)
{
    StaticJsonDocument<JSON_DOC_LEN> doc;
    doc["name"] = prof->name;
    doc["bdayYear"] = prof->bdayYear;
    doc["gender"] = prof->gender;
    doc["desiredGender"] = prof->desiredGender;
    doc["goal"] = prof->goal;
    doc["similarity"] = prof->similarity;
    serializeJson(doc, Serial);

    // ignore extraneous bytes
    while (Serial.available())
        Serial.read();
}
