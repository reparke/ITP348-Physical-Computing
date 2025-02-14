#include "argon_ble_setup.h"

// on board LED
const int PIN_LED = D7;

// Servo
const int PIN_SERVO = D2;

// motor pins
const int AIN1 = D3;
const int AIN2 = D4;
const int PWMA = D5;

void setup() {
    argon_ble_setup();  // BLE setup

    pinMode(PIN_LED, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    Serial.begin(9600);
}
void loop() {}

/* onDataReceived() is event handler for incoming data via RX characteristic
   When the RXCharacteristic receives data, the event handler is called
   Note: uint8_t is a byte ("unsigned integer of length 8 bits")

    The command scheme is from the Adafruit Bluefruit app
    https://learn.adafruit.com/bluefruit-le-connect/controller

    ex: [‘!’] [‘B’] [‘4’] [‘1’] [CRC]
*/
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer,
                    void* context) {
    bluetoothSerialDebug(data, len); /* uncomment for serial monitor debug */
}
/********************************************************************************/

/*
  bluetoothSerialDebug
      used for printing debug info to serial monitor ===
*/
void bluetoothSerialDebug(const uint8_t* data, size_t len) {
    for (size_t index = 0; index < len; index++) {
        Serial.print( (char)data[index]);
        Serial.print(" ");
    }
    Serial.println();
}
