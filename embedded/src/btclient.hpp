#ifndef BTCLIENT_HPP
#define BTCLIENT_HPP

#include <Arduino.h>
#include "constants.h"
#include "profile.hpp"
#include <NimBLEDevice.h>

static NimBLEAdvertisedDevice* advDevice;
static bool doConnect = false;

/** Define a class to handle the callbacks when advertisments are received */
class AdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {

    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {

        if (advertisedDevice->isAdvertisingService(NimBLEUUID(DATE_SRV_UUID)))
        {
            Serial.println("Found Our Service");
            /** stop scan before connecting */
            NimBLEDevice::getScan()->stop();
            /** Save the device reference in a global for the client to use*/
            advDevice = advertisedDevice;
            /** Ready to connect now */
            doConnect = true;
        }
    };
};

class PemdasBluetoothClient {
public:
    PemdasBluetoothClient(const Profile* profile);
    ~PemdasBluetoothClient();
    void loop();
private:
    bool connectToServer();
    void detectMatch(Profile other);
    const Profile* me;
};

#endif
