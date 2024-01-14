#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <ArduinoJson.h>
#include "profile.hpp"

class PemdasSerial {
public:
    PemdasSerial();
    ~PemdasSerial();
    bool recvProfile(struct Profile* out);
private:
    void sendProfileAck(struct Profile* prof);
};

#endif
