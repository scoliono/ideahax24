#ifndef BTSERVER_HPP
#define BTSERVER_HPP

#include <Arduino.h>
#include <NimBLEDevice.h>
#include "profile.hpp"

/** Handler class for characteristic actions */
class PemdasCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onRead(NimBLECharacteristic* pCharacteristic) {
        //Serial.print(pCharacteristic->getUUID().toString().c_str());
        //Serial.print(": onRead(), value: ");
        //Serial.println(pCharacteristic->getValue().c_str());
    }

    void onWrite(NimBLECharacteristic* pCharacteristic) {
        //Serial.print(pCharacteristic->getUUID().toString().c_str());
        //Serial.print(": onWrite(), value: ");
        //Serial.println(pCharacteristic->getValue().c_str());
    }

    /** Called before notification or indication is sent,
     *  the value can be changed here before sending if desired.
     */
    void onNotify(NimBLECharacteristic* pCharacteristic) {
        //Serial.println("Sending notification to clients");
    }

    /** The status returned in status is defined in NimBLECharacteristic.h.
     *  The value returned in code is the NimBLE host return code.
     */
    void onStatus(NimBLECharacteristic* pCharacteristic, Status status, int code) {
        String str = ("Notification/Indication status code: ");
        str += status;
        str += ", return code: ";
        str += code;
        str += ", ";
        str += NimBLEUtils::returnCodeToString(code);
        //Serial.println(str);
    }

    void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue) {
        String str = "Client ID: ";
        str += desc->conn_handle;
        str += " Address: ";
        str += std::string(NimBLEAddress(desc->peer_ota_addr)).c_str();
        if(subValue == 0) {
            str += " Unsubscribed to ";
        }else if(subValue == 1) {
            str += " Subscribed to notfications for ";
        } else if(subValue == 2) {
            str += " Subscribed to indications for ";
        } else if(subValue == 3) {
            str += " Subscribed to notifications and indications for ";
        }
        str += std::string(pCharacteristic->getUUID()).c_str();

        //Serial.println(str);
    }
};

class PemdasBluetoothServer {
public:
    PemdasBluetoothServer(const Profile* me);
    ~PemdasBluetoothServer();
    void loop();
};

#endif
