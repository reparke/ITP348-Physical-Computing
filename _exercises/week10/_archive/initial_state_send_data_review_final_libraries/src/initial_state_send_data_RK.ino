
#include "JsonParserGeneratorRK.h"
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
  - Create a Webhook in Particle console (with ACCESS KEY and BUNDLE KEY) so our
  data gets relay to IS
  - Create string value that has JSON data we want to send to IS
  - Publish the string to via Particle.cloud
*/

String latitudeUSC = "34.02051";
String longitudeUSC = "-118.28563";
String latitudeUCLA = "34.0689";
String longitudeUCLA = "-118.4452";
int battery = 100;

unsigned long batteryDecreaseTime = 3000;
unsigned long prevMillis = 0;

void setup() { Serial.begin(9600); }

void loop() {
    unsigned long curMillis = millis();
    if (curMillis - prevMillis >
        batteryDecreaseTime) {  // decrease battery level by 10% ever 3 sec
        battery = battery - 10;
        if (battery == 0) {  // reset battery to 100% when it gets to 0%
            battery = 100;
        }
        latitudeUSC = String(latitudeUSC.toFloat() + 0.01);
        longitudeUSC = String(longitudeUSC.toFloat() - 0.01);
        prevMillis = curMillis;
    }

    /*
        [
          {"key" : "location" : "value" : "34.33,-193.00"},
         {"key" : "battery", "value" : battery}
         ]
    */

    publishJSONRK();
    //  publishJSONManually();

    delay(10000);
}

void publishJSONManually() {
    String coordsUSC = latitudeUSC + "," + longitudeUSC;
    String coordsUCLA = latitudeUCLA + "," + longitudeUCLA;

    String data = "[{\"key\":\"locationUSC\",\"value\":\"" + coordsUSC +
                  "\"},{\"key\":\"locationUCLA\",\"value\":\"" + coordsUCLA +
                  "\"},{\"key\":\"battery\",\"value\":" + String(battery) +
                  "}]";
    Particle.publish("week10_20203", data);
}
void publishJSONRK() {
    JsonWriterStatic<622> jw;
    jw.init();  // empty buffer for reuse (since JsonWriterStatic is static)

    // Creating a scope like this in {} with a JsonWriterAutoObject in it
    // creates an object, and automatically closes the object when leaving the
    // scope. This is necessary because all JSON values must be in either an
    // object or an array to be valid, and JsonWriter requires all startObject
    // to be balanced with a finishObjectOrArray and JsonWriterAutoObject takes
    // care of doing that automatically.

    String coordsUSC = latitudeUSC + "," + longitudeUSC;
    String coordsUCLA = latitudeUCLA + "," + longitudeUCLA;

    // //simple JSON object (doesn't work for initial state which needs array)
    //     {
    //         JsonWriterAutoObject obj(&jw);

    //         // Add various types of data
    //         jw.insertKeyValue("locationUSC", coordsUSC);
    //         jw.insertKeyValue("locationUCLA", coordsUCLA);
    //         jw.insertKeyValue("battery", battery);
    //     }
    {
        JsonWriterAutoArray obj(&jw);

        jw.startObject();
        jw.insertKeyValue("key", "locationUSC");
        jw.insertKeyValue("value", coordsUSC);
        jw.finishObjectOrArray();

        jw.startObject();
        jw.insertKeyValue("key", "locationUCLA");
        jw.insertKeyValue("value", coordsUCLA);
        jw.finishObjectOrArray();

        jw.startObject();
        jw.insertKeyValue("key", "battery");
        jw.insertKeyValue("value", battery);
        jw.finishObjectOrArray();

        jw.finishObjectOrArray();
    }

    Particle.publish("week10_20203", jw.getBuffer());
    // Serial.println(jw.getBuffer());
    // Wait 60 seconds
}