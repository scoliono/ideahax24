#ifndef BT_HPP
#define BT_HPP

#include "profile.hpp"

class PemdasBluetooth {
public:
    PemdasBluetooth(const Profile* me);
    ~PemdasBluetooth();
    void loop();
};

#endif
