#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include "MotorDriver.h"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Define the pins and number of motors
int motorPins[] = {4, 5}; // go to motors 5 and 6 (because of course they do..duh)
const int numMotors = 2;
const int thread_intervals[] = {2000,500};
int last_thread_timestamp[] = {0,0};

// Instantiate MotorDriver with pins and number of motors
MotorDriver motorDriver(motorPins, numMotors);

#define SERVICE_UUID        "1f335a7a-6835-45a0-ab4d-54854ef61706"
#define CHARACTERISTIC_UUID "1f335a7a-6835-45a0-ab4d-54854ef61706"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string rxValue = pCharacteristic->getValue().c_str();
        std::string rxString = rxValue;

        if (rxValue.length() > 0) {
            // Serial.println("*********");
            // Serial.println(rxString.c_str()); // prints out received string 
            motorDriver.setMsg(rxString);
        }
    }
};

void setup() {
    // Initialize pin modes
    for (int p = 0; p < 8; p++) {
        pinMode(p, OUTPUT);
        digitalWrite(p, LOW);
    }
    Serial.begin(115200);

    // Create the BLE Device
    BLEDevice::init("MyESP32");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                      );

    pCharacteristic->setCallbacks(new MyCallbacks());

    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

void loop() {
    motorDriver.executeMsg();
    motorDriver.printMotorOutputs();

    // Notify changed value
    if (deviceConnected && thread_intervals[0] < (millis() - last_thread_timestamp[0])) {
        pCharacteristic->notify();
        last_thread_timestamp[0] = millis();
    }

    // Disconnecting
    if (!deviceConnected && oldDeviceConnected && thread_intervals[1] < (millis() - last_thread_timestamp[1])) {
        pServer->startAdvertising(); // Restart advertising
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
        last_thread_timestamp[1] = millis();
    }

    // Connecting
    if (deviceConnected && !oldDeviceConnected) {
        // Do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
