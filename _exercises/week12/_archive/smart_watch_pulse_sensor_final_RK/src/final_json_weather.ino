/* NOTE - Rob Parke
4/14/2022
    -Adding Weatherstack API / JSON

9/25/2021
    -Updated version for Qwiic I2C OLED + MAX30101
    -Eliminated software timers

8/29/2022
    -updated to use pulse sensor
    note from Bill Siever: Most of the real work is done by interrupts.  The
process() is needed to do the callbacks to user code (triggers the call to
either PulseSensorAmped_data() or PulseSensorAmped_lost ()).  If you’d want to
try to catch an individual beat, it should be called faster than the expected
heart rate (maybe 2x faster….So maybe 6x per second or ~166ms?) .  If you just
want occasional updates, you can call it less frequently.

*/

// libraries for OLED
#include "JsonParserGeneratorRK.h"
#include "SparkFunMicroOLED.h"  // Include MicroOLED library
#include "bitmaps_watch.h"
#include "bitmaps_weather.h"

JsonParser jsonParser;

// const byte RATE_SIZE = 4;  // Increase this for more averaging. 4 is good.
// byte rates[RATE_SIZE];     // Array of heart rates
// byte rateSpot = 0;
// long lastBeat = 0;  // Time at which the last beat occurred
const int pulseSignalPin = A4;

#include <PulseSensorAmped.h>

int beatAvg;
//////////////////////////
// Heart Screen         //
//////////////////////////
/* refreshing the OLED is very slow and it causes
    the heart rate detection to fail
    This delay was determined experimentally to work well
*/
unsigned long prevScreenUpdateMillis = 0;
unsigned long HEART_SCREEN_UPDATE_MS = 3000;
unsigned long lastRead = 0;
unsigned long samples = 0;

const int LOW_BPM_THRESHOLD = 40;
const int HIGH_BPM_THRESHOLD = 200;

//////////////////////////
// Time  Screen         //
//////////////////////////
/*
 */
unsigned long TIME_SCREEN_UPDATE_MS = 500;

//////////////////////////
// Weather Screen         //
//////////////////////////
/* Weatherstack only has 250 API calls in free plan so use
   very long delay (8 times per day)
*/
unsigned long WEATHER_SCREEN_UPDATE_MS = 10512000;
float tempWeather = 0;
String city = "city";
String weatherDescription = "desc";
int weatherCode = 0;
int humidity = 0;
int uvIndex = 0;

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
// The library assumes a reset pin is necessary. The Qwiic OLED has RST
// hard-wired, so pick an arbitrarty IO pin that is not being used
#define PIN_RESET 9

// The DC_JUMPER is the I2C Address Select jumper. Set to 1 if the jumper is
// open (Default), or set to 0 if it's closed.
#define DC_JUMPER 1
MicroOLED oled(MODE_I2C, PIN_RESET, DC_JUMPER);  // I2C declaration
#include <Wire.h>
//////////////////////////
// Button Variables     //
//////////////////////////
const int PIN_BUTTON = D3;
int prevReading = HIGH;     // the last VERIFIED state
int curButtonState;         // the current VERIFIED state
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 200;   // the debounce time; increase if the output
//////////////////////////
// States               //
//////////////////////////
// need to build this
enum State { HEART, TIME, WEATHER };
State currentState = WEATHER;
void getNextState() {
    switch (currentState) {
        case HEART:
            currentState = TIME;
            break;
        case TIME:
            currentState = WEATHER;
            break;
        case WEATHER:
            currentState = HEART;
            break;
    }
}
void loadNextScreen() {
    switch (currentState) {
        case HEART:
            runHeartScreen();
            break;
        case TIME:
            runTimeScreen();
            break;
        case WEATHER:
            runWeatherScreen();
            break;
    }
}
void runTimeScreen() {
    unsigned long curMillis = millis();
    if (curMillis - prevScreenUpdateMillis > TIME_SCREEN_UPDATE_MS) {
        prevScreenUpdateMillis = curMillis;

        String dateFormat = "%b %d";
        String dayFormat = "%a";
        String timeFormat = "%I:%M";
        String secondsFormat = "%S";
        // String timeFormat = "%I:%M ";
        // for debugging
        Serial.println("Time");

        oled.clear(PAGE);  // Clear the display
        delay(10);
        oled.drawBitmap(clock_16x12);

        oled.setCursor(25, 0);
        oled.setFontType(0);
        oled.print(Time.format(dateFormat));

        oled.setCursor(25, 10);
        oled.setFontType(0);
        oled.print(Time.format(dayFormat));

        oled.setFontType(1);
        oled.setCursor(0, 25);
        oled.print(Time.format(timeFormat));

        oled.setFontType(0);
        oled.setCursor(50, 30);
        oled.print(Time.format(secondsFormat));

        oled.display();
    }
}

void runWeatherScreen() {  // doesn't use JSON or webhook

    unsigned long curMillis = millis();
    if (curMillis - prevScreenUpdateMillis > WEATHER_SCREEN_UPDATE_MS) {
        prevScreenUpdateMillis = curMillis;
        String data = "90089";
        Particle.publish("WeatherStackJSON", data, PRIVATE);
    }

    oled.clear(PAGE);
    switch (weatherCode) {
        case 296:
        case 302:
        case 308:
            oled.drawBitmap(weather_rainy_up_left);
            break;
        case 116:
        case 119:
        case 122:
            oled.drawBitmap(weather_cloudy_up_left);
            break;
        case 227:
            oled.drawBitmap(weather_snowing_up_left);
            break;

        default:
            oled.drawBitmap(weather_sunny_up_left);
            break;
    }

    oled.setCursor(38, 5);
    oled.setFontType(1);
    oled.print(tempWeather, 0);
    oled.setFontType(0);
    oled.print("o");

    oled.setFontType(0);
    oled.setCursor(0, 28);
    oled.print(weatherDescription.substring(0, 9));

    oled.setFontType(0);
    oled.setCursor(0, 40);
    oled.print("Hum ");
    oled.print(humidity);
    oled.print("%");

    oled.setCursor(40, 40);
    oled.print("UV ");
    oled.print(uvIndex);

    oled.display();
}

void runHeartScreen() {
    unsigned long curMillis = millis();
    if (curMillis - prevScreenUpdateMillis > HEART_SCREEN_UPDATE_MS) {
        prevScreenUpdateMillis = curMillis;

        if (beatAvg > LOW_BPM_THRESHOLD &&
            beatAvg < HIGH_BPM_THRESHOLD) {  // VALID!
            oled.clear(PAGE);
            oled.drawBitmap(heart16x12);
            oled.setFontType(1);
            oled.setCursor(20, 0);
            oled.print(String(beatAvg));
        } else {
            // INVALID!
            oled.clear(PAGE);
            oled.drawBitmap(heart16x12);
            oled.setFontType(1);
            oled.setCursor(20, 0);
            oled.print("---");
        }

        float voltage = analogRead(BATT) * 0.0011224;
        oled.setCursor(0, 40);
        oled.setFontType(0);
        oled.print("Batt ");
        oled.print(String(voltage, 2));
        oled.display();
        Serial.println("BPM: " + String(beatAvg));
    }
    // consider adding battery status bands
    // https://community.particle.io/t/can-argon-or-xenon-read-the-battery-state/45554/35?u=rob7
}
void setup() {
    analogRead(pulseSignalPin);

    Serial.begin(115200);
    Serial.println("Initializing...");

    PulseSensorAmped.attach(pulseSignalPin);
    PulseSensorAmped.start();

    // initialize OLED
    oled.begin();      // Initialize the OLED
    oled.clear(PAGE);  // Clear the display
    oled.drawBitmap(trojan_bmp);
    oled.display();
    delay(1000);  // Delay 1000 ms
    oled.drawBitmap(usc_bmp);
    oled.display();
    delay(1000);  // Delay 1000 ms

    pinMode(PIN_BUTTON, INPUT);
    Particle.subscribe("hook-response/WeatherStackJSON",
                       jsonSubscriptionHandler, MY_DEVICES);

    String data = "90089";
    Particle.publish("WeatherStackJSON", data, PRIVATE);
}

void loop() {
    int curReading = digitalRead(PIN_BUTTON);  // check button read

    if (curReading == HIGH && prevReading == LOW) {
        getNextState();  // button was pressed down, we should change
        prevScreenUpdateMillis = 0;  // make sure next screen loads
                                     // immediately instead of waiting
    }
    PulseSensorAmped.process();

    // FINALLY! here is your button code
    // if want to execute when button pressed down ONLY

    loadNextScreen();
    prevReading = curReading;  // update for next loop
}

void jsonSubscriptionHandler(const char *event, const char *data) {
    // Part 1 allows for webhook responses to be delivered in multple "chunks";
    // you don't need to change this
    int responseIndex = 0;
    const char *slashOffset = strrchr(event, '/');
    if (slashOffset) responseIndex = atoi(slashOffset + 1);
    if (responseIndex == 0) jsonParser.clear();
    jsonParser.addString(data);

    // Part 2 is where you can parse the actual data; you code goes in the IF
    if (jsonParser.parse()) {
        city =
            jsonParser.getReference().key("location").key("name").valueString();
        tempWeather = jsonParser.getReference()
                          .key("current")
                          .key("temperature")
                          .valueDouble();

        weatherCode = jsonParser.getReference()
                          .key("current")
                          .key("weather_code")
                          .valueInt();
        uvIndex =
            jsonParser.getReference().key("current").key("uv_index").valueInt();

        weatherDescription = jsonParser.getReference()
                                 .key("current")
                                 .key("weather_descriptions")
                                 .index(0)
                                 .valueString();

        Serial.println(city + "\n  " + weatherDescription +
                       "\n  temperature: " + String(tempWeather, 1) +
                       " F\n  humidith: " + String(humidity) +
                       "\n  uv index: " + String(uvIndex) +
                       "\n  weather code: " + String(weatherCode));
    }
}

// An optional function to recieve the Beats Per Minute (BPM) and Interbeat
// Interval (IBI) in mS
void PulseSensorAmped_data(int BPM, int IBI) {
    // Serial.print("BPM:");
    // Serial.print(BPM);
    // Serial.print(" IBI: ");
    // Serial.println(IBI);
    beatAvg = BPM;
}

// An optional function to indicate that the pulse signal has been lost
// (Due to the nature of pulse detection via reflected light, spurios signals
// are possible.
//  i.e. received data may not be valid)
void PulseSensorAmped_lost(void) {
    // Serial.println("Pulse Lost");
    // beatAvg = 0;
}
