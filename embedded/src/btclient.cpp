#include "btclient.hpp"
#include "profile.hpp"
#include "disp.hpp"

static int lastButton = 0;
static Profile match;
static bool foundMatch = false;

PemdasBluetoothClient::PemdasBluetoothClient(const Profile* profile)
{
    this->me = profile;

    /** create new scan */
    NimBLEScan* pScan = NimBLEDevice::getScan();

    /** create a callback that gets called when advertisers are found */
    pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

    /** Set scan interval (how often) and window (how long) in milliseconds */
    pScan->setInterval(45);
    pScan->setWindow(15);

    /** Active scan will gather scan response data from advertisers
     *  but will use more energy from both devices
     */
    pScan->setActiveScan(true);

    pScan->start(0);

    Serial.println("client: scanning for dating services...");
}

PemdasBluetoothClient::~PemdasBluetoothClient()
{

}

/** Handles the provisioning of clients and connects / interfaces with the server */
bool PemdasBluetoothClient::connectToServer() {
    NimBLEClient* pClient = nullptr;

    /** Check if we have a client we should reuse first **/
    if (NimBLEDevice::getClientListSize()) {
        /** Special case when we already know this device, we send false as the
         *  second argument in connect() to prevent refreshing the service database.
         *  This saves considerable time and power.
         */
        pClient = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());
        if (pClient) {
            if (!pClient->connect(advDevice, false)) {
                Serial.println("Reconnect failed");
                return false;
            }
            Serial.println("Reconnected client");
        }
        /** We don't already have a client that knows this device,
         *  we will check for a client that is disconnected that we can use.
         */
        else {
            pClient = NimBLEDevice::getDisconnectedClient();
        }
    }

    /** No client to reuse? Create a new one. */
    if (!pClient) {
        if(NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS) {
            Serial.println("Max clients reached - no more connections available");
            return false;
        }

        pClient = NimBLEDevice::createClient();

        Serial.println("New client created");

        /** Set initial connection parameters: These settings are 15ms interval, 0 latency, 120ms timout.
         *  These settings are safe for 3 clients to connect reliably, can go faster if you have less
         *  connections. Timeout should be a multiple of the interval, minimum is 100ms.
         *  Min interval: 12 * 1.25ms = 15, Max interval: 12 * 1.25ms = 15, 0 latency, 51 * 10ms = 510ms timeout
         */
        pClient->setConnectionParams(12,12,0,51);
        /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
        pClient->setConnectTimeout(5);

        if (!pClient->connect(advDevice)) {
            /** Created a client but failed to connect, don't need to keep it as it has no data */
            NimBLEDevice::deleteClient(pClient);
            Serial.println("Failed to connect, deleted client");
            return false;
        }
    }

    if (!pClient->isConnected()) {
        if (!pClient->connect(advDevice)) {
            Serial.println("Failed to connect");
            return false;
        }
    }

    Serial.print("Connected to: ");
    Serial.println(pClient->getPeerAddress().toString().c_str());
    Serial.print("RSSI: ");
    Serial.println(pClient->getRssi());

    /** Now we can read/write/subscribe the charateristics of the services we are interested in */
    NimBLERemoteService* pSvc = nullptr;
    NimBLERemoteCharacteristic* pChr = nullptr;
    NimBLERemoteDescriptor* pDsc = nullptr;

    pSvc = pClient->getService(DATE_SRV_UUID);
    if (pSvc) {     /** make sure it's not null */
        pChr = pSvc->getCharacteristic(PROF_CHAR_UUID);

        if (pChr) {     /** make sure it's not null */
            if (pChr->canRead()) {
                Serial.print(pChr->getUUID().toString().c_str());
                Serial.print(" Value: ");
                Serial.println(pChr->readValue().c_str());

                // deserialize the json
                DynamicJsonDocument doc(JSON_DOC_LEN);
                DeserializationError error = deserializeJson(doc, pChr->readValue().c_str());
                if (error) {
                    Serial.println("Error deserializing json!");
                } else {
                    Profile other = Profile::fromJSON(doc);
                    detectMatch(other);
                }

            }

            pClient->disconnect();
        }
    } else {
        Serial.println("Dating service not found.");
    }

    Serial.println("Done with this device!");
    return true;
}

void PemdasBluetoothClient::detectMatch(Profile other)
{
    if (me->isMatch(&other)) {
        Serial.println("Found match!!!");
        match = other;
        foundMatch = true;
        displayMatchFound(&match);
    } else {
        Serial.println("We are not a match with this user");
    }
}

void PemdasBluetoothClient::loop()
{
    // check for button press to cycle screens
    if (digitalRead(DISP_BUTTON) == LOW && millis() - lastButton > 300) {
        int buttonStart = millis();
        while (digitalRead(DISP_BUTTON) == LOW);
        int buttonDir = millis() - buttonStart;
        lastButton = millis();
        if (buttonDir > 5000) {
            // long press: clear this match
            displayInit();
        } else if (foundMatch) {
            displayMatchDesc(&match);
        }
    }

    /** Ignore until we find a device we want to connect to */
    if (!doConnect) {
        return;
    }

    doConnect = false;

    /** Found a device we want to connect to, do it now */
    if (connectToServer()) {
        Serial.println("Success! we should now be getting notifications, scanning for more!");
    } else {
        Serial.println("Failed to connect, starting scan");
        NimBLEDevice::getScan()->start(0);
    }
}
