#include "bt.hpp"
#include "btclient.hpp"
#include "btserver.hpp"
#include <random>
#include <time.h>
#include <sstream>

static PemdasBluetoothServer* srv;
static PemdasBluetoothClient* client;

PemdasBluetooth::PemdasBluetooth(const Profile* me)
{
    std::ostringstream deviceNameOss;
    deviceNameOss << "pemdas_" << random(1000);

    NimBLEDevice::init(deviceNameOss.str());
    NimBLEDevice::setPower(ESP_PWR_LVL_P9); /** +9db */

    /** 2 different ways to set security - both calls achieve the same result.
     *  no bonding, no man in the middle protection, secure connections.
     *
     *  These are the default values, only shown here for demonstration.
     */
    //NimBLEDevice::setSecurityAuth(false, false, true);
    NimBLEDevice::setSecurityAuth(/*BLE_SM_PAIR_AUTHREQ_BOND | BLE_SM_PAIR_AUTHREQ_MITM |*/ BLE_SM_PAIR_AUTHREQ_SC);

    srv = new PemdasBluetoothServer(me);
    client = new PemdasBluetoothClient(me);
}

PemdasBluetooth::~PemdasBluetooth()
{
    NimBLEDevice::deinit();
    delete srv;
    delete client;
}

void PemdasBluetooth::loop()
{
    srv->loop();
    client->loop();
}


