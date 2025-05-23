#define BLYNK_TEMPLATE_ID "TMPLiSjJodGO"
#define BLYNK_DEVICE_NAME "W6 20221"
#define BLYNK_AUTH_TOKEN "XP3DNyJZiH5IjIVR1EHTvF63WJ1EHkTU"

#define BLYNK_PRINT Serial
#include <blynk.h>
#define BLYNK_IP \
    IPAddress(64, 225, 16, 22)  // https://ny3.blynk.cloud/ – New York

const int PIN_LED = D7;
const int PIN_RED = D3;
const int PIN_GREEN = D4;
const int PIN_BLUE = D5;
const int PIN_SWITCH = D2;

int prevDoorState = HIGH;
int currDoorState = HIGH;

int virtualButtonVal = HIGH;

unsigned long intervalBlynk = 1000;
unsigned long prevMillisBlynk = 0;

// from APP to ARGON
// we have to create function--when data gets sent from APP to ARGON,
// THIS function is called
// V5 is the virtual "button" which will control the onboard LED
BLYNK_WRITE(V5) {
    // take virtual button val and write directly to REAL onboard LED
    //  int buttonVal = param.asInt();  //we stored the APP value into an int
    //  digitalWrite(PIN_LED, buttonVal);
    virtualButtonVal = param.asInt();
}

void setup() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_SWITCH, INPUT);
    Serial.begin(9600);

    delay(5000);
    Blynk.begin(BLYNK_AUTH_TOKEN, BLYNK_IP);
}

void updateArgon() { digitalWrite(PIN_LED, virtualButtonVal); }

/*
    How to send data from ARGON to APP
    Blynk.virtualWrite(VIRTUAL_PIN, DATA)
        DATA can be int, string, float
    if we are virtualWrites in loop(), we must use millis()
*/

void loop() {
    // we can't use delay in loop()
    Blynk.run();  // goes OUTSIDE any millis timer
    unsigned long currMillis = millis();
    if (currMillis - prevMillisBlynk > intervalBlynk) {
        prevMillisBlynk = currMillis;
        //generate a random number and send to App on V6
        int randNum = random(0,256);
        Blynk.virtualWrite(V6, randNum);
        updateArgon();
    }
}
    // void loop() {

    //     currDoorState = digitalRead(PIN_SWITCH);
    //     // Serial.println(currDoorState);
    //     if (currDoorState == LOW && prevDoorState == HIGH) {
    //         Serial.println("Door was closed");
    //         Particle.publish("Door state", "Door was closed");
    //         analogWrite(PIN_RED, 255);
    //         analogWrite(PIN_GREEN, 0);
    //         analogWrite(PIN_BLUE, 0);
    //     } else if (currDoorState == HIGH && prevDoorState == LOW) {
    //         Serial.println("Door was opened");
    //         Particle.publish("Door State", "Door was opened");
    //         analogWrite(PIN_RED, 255);
    //         analogWrite(PIN_GREEN, 255);
    //         analogWrite(PIN_BLUE, 255);
    //     }
    //     prevDoorState = currDoorState;
    // }
