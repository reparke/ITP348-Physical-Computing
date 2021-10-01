---
marp: true
theme: itp

category: reference
title: PIR - Passive Infrared Sensor
show_in_list: true
---

<!-- headingDivider: 2 -->

# PIR - Passive Infrared Sensor

<img src="pir_infrared_sensor.assets/13285-01.jpg" alt="https://cdn.sparkfun.com//assets/parts/1/0/5/3/5/13285-01.jpg" style="width:600px" />  



## Wiring

| PIR (Sparkfun)      | Argon                                                    |
| ------------------- | -------------------------------------------------------- |
| + **(red wire)**    | 5v                                                       |
| - **(white wire)**  | GND                                                      |
| AL **(black wire)** | digital input pin <br />(with 10K pullup resistor to 5v) |

*Note: The wire color and pin order is specific to the [Sparkfun PIR sensor](https://www.sparkfun.com/products/13285) and not all PIR sensor*

## Wiring

<img src="pir_infrared_sensor.assets/pir_sparkfun_bb.png" style="width:1000px;" />

## Operation (Sparkfun PIR)

* Requires 1-2 sec delay in `setup()` to get initial reading from room
* Digital output from `AL` pin
* `HIGH` when no movement detected
* `LOW` when movement detected
* Signal remains active `low` for about 3 sec **after** movement (*documentation says 15 sec but that doesn't seem accurate*)

## Code

This is a basic example where any movement triggers alarm. More sophisticated use can include `millis()`

```c++
const int MOTION_PIN = D2;  // Pin connected to motion detector

void setup() {
    Serial.begin(9600);
    pinMode(MOTION_PIN, INPUT);
    delay(10000);  // wait to read the room
}

void loop() {
    int proximity = digitalRead(MOTION_PIN);
    if (proximity == LOW)  // If the sensor's output goes low, motion is detected
    {
        Serial.println("Motion detected!");
    } else {
        Serial.println("Motion stopped!");
    }
}

```



## Credit

* [Sparkfun](https://www.sparkfun.com/products/13285)
* Diagrams created with [Frizting](https://fritzing.org)




























