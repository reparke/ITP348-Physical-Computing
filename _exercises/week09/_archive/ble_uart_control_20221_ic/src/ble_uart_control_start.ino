#include "argon_ble_setup.h"

/*
  Use Adafruit Bluefruit app
  (https://learn.adafruit.com/bluefruit-le-connect/controller)
  Connect to Argon -> controller

  - use color picker to change LED
  - use controller to play a tone and flash on-board LED
  - control fan motor and servo
  - play a tone

| Motor Controller | Argon |
| ---------------- | ----- |
| PWMA             | D5    |
| AIN2             | D4    |
| AIN1             | D3    |
| VCC              | 3v3   |
| GND              | GND   |
| VM               | 3v3   |
| STBY             | 3v3   |

RGB LED (common cathode)
  R - A2
  G - A1
  B - A0

Buzzer - D6
Servo: D2
On-board LED - D7

Specifications
  When "1" is pressed down, on-board LED D7 will blink twice
  When "2" is pressed down, on-board LED D7 will turn on
  When "2" is released, on-board LED D7 will turn off
  When "4" is pressed, play a tone
  When "RIGHT" is pressed down, rotate servo clockwise
  When "LEFT" is pressed down, rotate servo counter clockwise
  When "3" is pressed down, change direction of fan

  When color picker is used, change RGB LED color

Specifications
  When "1" is pressed down, on-board LED D7 will blink twice
  When "2" is pressed down, on-board LED D7 will turn on
  When "2" is released, on-board LED D7 will turn off
  
  when up is pressed, play tone C4
  when down is pressed, play tone E4
  when right is pressed, play tone G4
  when left is pressed, play tone C5


  When color picker is used, change RGB LED color
*/


// RBG LED Pins
const int PIN_RED = A2;
const int PIN_GREEN = A1;
const int PIN_BLUE = A0;

const int PIN_BUZZER = D6;
const int PIN_LED = D7;

const int PIN_SERVO = D2;

// motor pins
const int AIN1 = D3;
const int AIN2 = D4;
const int PWMA = D5;

void setup() {
    /* === START BLE SETUP === */
    argon_ble_setup();
    /* === END BLE SETUP === */

    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    Serial.begin(9600);
}
unsigned long prevMillis = 0;
void loop() {
  unsigned long curMillis = millis();
  if (curMillis - prevMillis > 2000) {
    prevMillis = curMillis;
    Serial.println("Testing");
  }
}

/*  Sample Code to communicate from Argon to Adafruit Bluefruit app
    https://learn.adafruit.com/bluefruit-le-connect/controller
    
    The command scheme is from the Adafruit Bluefruit app
    ex: [‘!’] [‘B’] [‘4’] [‘1’] [CRC]
*/

// onDataReceived() is event handler for incoming data via RX characteristic
// When the RXCharacteristic receives data, the event handler is called
// Note: uint8_t is a byte ("unsigned integer of length 8 bits")
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer,
                    void* context) {
    Serial.println("Rob Byte Received!");
    if (len > 4)  { //we are looking for at least 4 bytes of data from Bluefruit
        // Button 1 pressed:  [‘!’] [‘B’] [‘1’] [‘1’] [CRC]
        //in c++, a single character is called a CHAR and is one byte
        // CHAR uses '' instead "" like a string does
        if (data[0] == '!' && data[1] == 'B' && data[2] == '1' && data[3] == '1') {
          digitalWrite(PIN_LED, HIGH);
          delay(500);
          digitalWrite(PIN_LED, LOW);
        }
        
    }
    // TODO finish event handler
  /* === START DEBUG PRINTING ===
           uncomment following loop to see commands being sent by app ==
   */
/*)
  for (size_t ii = 0; ii < len; ii++) {
    Serial.print(data[ii]);
    Serial.print(" ");
  }
  Serial.println();
  for (size_t ii = 0; ii < len; ii++) {
    Serial.write(data[ii]);
    Serial.print(" ");
  }
  Serial.println();
*/
  /* === END DEBUG PRINTING === */
  
}
