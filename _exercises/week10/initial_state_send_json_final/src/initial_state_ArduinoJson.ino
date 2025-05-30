
/*
How to send single values to Initial state in JSON format
  format: {}
  ex:
    {"key":"temperature", "value":79}

How to send multiple values to Initil
  format: [ {}, {} ]
  ex:
    [
      {"key":"temperature":"value":TEMP_VAR},
      {"key":"weather", "value":"WEATHER_VAR"}
    ]
    */

// format for GPS
// lat,long       --> "23.4993,343.3999"

/*
  send data to initial state
  - Create a "bucket" at IS which has our ACCESS KEY and BUNDLE KEY
  - Create a Webhook in Particle console (with ACCESS KEY and BUNDLE KEY) so our data gets relay to IS
  - Create string value that has JSON data we want to send to IS
  - Publish the string to via Particle.cloud
*/
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include <ArduinoJson.h>

String latitudeUSC = "34.02051";
String longitudeUSC = "-118.28563";
String latitudeUCLA = "34.0689";
String longitudeUCLA = "-118.4452";
int battery = 100;

unsigned long batteryDecreaseTime = 3000;
unsigned long prevMillis = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    unsigned long curMillis = millis();
    if (curMillis - prevMillis > batteryDecreaseTime) {  //decrease battery level by 10% ever 3 sec
        battery = battery - 10;
        if (battery == 0) {  //reset battery to 100% when it gets to 0%
            battery = 100;
        }
        latitudeUSC = String(latitudeUSC.toFloat()+0.01);
        longitudeUSC = String(longitudeUSC.toFloat() - 0.01);
        prevMillis = curMillis;
    }

/*
    [
      {"key" : "location" : "value" : "34.33,-193.00"},
     {"key" : "battery", "value" : battery}
     ]
*/

    String coordsUSC = latitudeUSC + "," + longitudeUSC;
    String coordsUCLA = latitudeUCLA + "," + longitudeUCLA;

    StaticJsonDocument<192> doc;

    JsonObject obj1 = doc.createNestedObject();
    obj1["key"] = "locationUSC";
    obj1["value"] = coordsUSC;

    JsonObject obj2 = doc.createNestedObject();
    obj2["key"] = "locationUCLA";
    obj2["value"] = coordsUCLA;

    JsonObject obj3 = doc.createNestedObject();
    obj3["key"] = "battery";
    obj3["value"] = battery--;

    String output;
    serializeJson(doc, output);
    Serial.println(output);
    Particle.publish("initial_state_json", output);

    delay(10000);
}