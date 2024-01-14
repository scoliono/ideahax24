#include "btserver.hpp"
#include "constants.h"
#include <ArduinoJson.h>

static NimBLEServer* pServer;
static PemdasCharacteristicCallbacks chrCallbacks;

static char json_str[JSON_DOC_LEN];

PemdasBluetoothServer::PemdasBluetoothServer(const Profile* me)
{
    pServer = NimBLEDevice::createServer();

    NimBLEService* pDatingSrv = pServer->createService(DATE_SRV_UUID);
    NimBLECharacteristic* pProfileCharacteristic = pDatingSrv->createCharacteristic(
        PROF_CHAR_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
        /** Require a secure connection for read and write access */
        NIMBLE_PROPERTY::READ_ENC |  // only allow reading if paired / encrypted
        NIMBLE_PROPERTY::WRITE_ENC   // only allow writing if paired / encrypted
    );

    StaticJsonDocument<JSON_DOC_LEN> doc;
    me->toJSON(doc);
    serializeJson(doc, json_str);

    pProfileCharacteristic->setValue(json_str);
    pProfileCharacteristic->setCallbacks(&chrCallbacks);

    /** Start the services when finished creating all Characteristics and Descriptors */
    pDatingSrv->start();

    //Serial.println("server: Started dating service");

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    /** Add the services to the advertisment data **/
    pAdvertising->addServiceUUID(pDatingSrv->getUUID());
    /** If your device is battery powered you may consider setting scan response
     *  to false as it will extend battery life at the expense of less data sent.
     */
    pAdvertising->setScanResponse(true);
    pAdvertising->start();

    //Serial.println("server: Started advertising");
}


PemdasBluetoothServer::~PemdasBluetoothServer()
{

}

void PemdasBluetoothServer::loop()
{
    /** Do your thing here, this just spams notifications to all connected clients */
    if (pServer->getConnectedCount()) {
        NimBLEService* pSvc = pServer->getServiceByUUID(DATE_SRV_UUID);
        if (pSvc) {
            NimBLECharacteristic* pChr = pSvc->getCharacteristic(PROF_CHAR_UUID);
            if (pChr) {
                pChr->notify(true);
            }
        }
    }
}
