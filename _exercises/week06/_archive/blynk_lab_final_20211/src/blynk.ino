/*Note: This corresponds to locally installed project "Week 6", tab "lab"
  Features
    - ZeRGBa to control LED (V0 V1 V2 app --> argon)
    - Display to show if switch is open (V3 argon --> app)
    - Gauge to display random number (V6 argon --> app)
    - Text box to send command (V4 app --> argon)

*/

/*
install Blynk app on your phone

connect RGB led to argon (D2, D3, D4)

connect switch (Gnd - A0 - 3.3v)
*/

#include <blynk.h>                                 //library
char auth[] = "_Ce745Lvb6anFEDvGggMSYvZCwIF8b6i";  // this is a array of
                                                   // characters with our token

// if you're sending values to from ARGON to APP (push), we MUST use millis()
// limit is 10 values per sec
// in loop(), we MUST NOT use delay
unsigned long blynkDelay = 1000;
unsigned long prevMillis = 0;

const int PIN_LED = D7;
const int PIN_RED = D3;
const int PIN_GREEN = D4;
const int PIN_BLUE = D5;
const int PIN_SWITCH = D2;

// virtual pin headers

// retrieve from app
// app ==> argon
//  create a special function called BLYNK_WRITE(<<PIN>>)

enum Color { WHITE = 1, GREEN, MAGENTA, BLUE, YELLOW, RED, ORANGE };

void displayColor(Color c) {
    switch (c) {
        case WHITE:
            analogWrite(PIN_RED, 255);
            analogWrite(PIN_GREEN, 255);
            analogWrite(PIN_BLUE, 255);
            break;
        case BLUE:
            analogWrite(PIN_RED, 0);
            analogWrite(PIN_GREEN, 0);
            analogWrite(PIN_BLUE, 255);
            break;
        case ORANGE:
            analogWrite(PIN_RED, 255);
            analogWrite(PIN_GREEN, 165);
            analogWrite(PIN_BLUE, 0);
            break;
        case GREEN:
            analogWrite(PIN_RED, 0);
            analogWrite(PIN_GREEN, 255);
            analogWrite(PIN_BLUE, 0);
            break;
        case MAGENTA:
            analogWrite(PIN_RED, 255);
            analogWrite(PIN_GREEN, 0);
            analogWrite(PIN_BLUE, 255);
            break;
        case YELLOW:
            analogWrite(PIN_RED, 255);
            analogWrite(PIN_GREEN, 255);
            analogWrite(PIN_BLUE, 0);
            break;
        case RED:
            analogWrite(PIN_RED, 255);
            analogWrite(PIN_GREEN, 0);
            analogWrite(PIN_BLUE, 0);
            break;
    }
}

//from app to argon
BLYNK_WRITE(V0) {  // RED
    // event handler for when the user moves the sliders on the app
    // we have a slider that send an integer value from 0-255
    // lets read the value on the slider
    int sliderValue = param.asInt();  // represents slider value
    analogWrite(PIN_RED, sliderValue);
    Serial.println("Red: " + String(sliderValue));
}
BLYNK_WRITE(V1) {                     // GREEN
    int sliderValue = param.asInt();  // represents slider value
    // if ()
    analogWrite(PIN_GREEN, sliderValue);
    Serial.println("Green: " + String(sliderValue));
}
BLYNK_WRITE(V2) {                     // BLUE
    int sliderValue = param.asInt();  // represents slider value
    analogWrite(PIN_BLUE, sliderValue);
    Serial.println("Blue: " + String(sliderValue));
}

BLYNK_WRITE(V4) {  // random color button
    if (param.asInt() == 1) {           //since button widget is "push", we need this to avoid triggering on push and release
        int rand = random(1, 8);
        Color c = (Color)rand;
        displayColor(c);

        Serial.println("random color");
    }
}
BLYNK_WRITE(V5) {                 // menu
    int menuVal = param.asInt();  // represents slider value - starts at index 1 (not 0)
    Color c = (Color)menuVal;       //cast / converted our int to an enum
    displayColor(c);
    Serial.println("menu val named color: " + String(menuVal));
}

void setup() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_SWITCH, INPUT);
    Serial.begin(9600);

    // 1. have delay
    delay(5000);  // needs to be delay, not millis

    // 2. connect to blynk
    // Blynk.begin(auth);
    Blynk.begin(auth, IPAddress(192, 168, 0, 131), 8080);
}

void loop() {
    Blynk.run();  // do NOT put in millis timer

    unsigned long currMillis = millis();
    if (currMillis - prevMillis > blynkDelay) {
        prevMillis = currMillis;

        int switchVal = digitalRead(PIN_SWITCH);

        // NOW we send value
        // Blynk.virtualWrite(<<PIN>>, <<VALUE>>)
        if (switchVal == HIGH) {
            Blynk.virtualWrite(
                V3,
                "open");  // send data from argon ===> app
                          // switchRead = digitalRead(PIN_SWITCH);
        } else {
            Blynk.virtualWrite(
                V3,
                "closed");  // send data from argon ===> app
                            // switchRead = digitalRead(PIN_SWITCH);
        }
        int randNum = random(0, 255);
        Serial.println("sending random number: " + String(randNum));
        Blynk.virtualWrite(V6,
                           randNum);  // send data from argon ===> app
    }
}