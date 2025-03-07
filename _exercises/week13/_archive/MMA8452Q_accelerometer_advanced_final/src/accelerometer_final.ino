/*	MMA8452Q-Serial_Example.ino
        Jim Lindblom <jim@sparkfun.com>
        August 31, 2015
        https://github.com/sparkfun/SparkFun_MMA8452Q_Particle_Library

        This is a simple example sketch for the SparkFun MMA8452Q
        Particle library. It'll connect to an MMA8452Q and stream the
        values out the serial port as the become available.

        Development environment specifics:
        Particle Build environment (https://www.particle.io/build)
        Particle Photon
        Distributed as-is; no warranty is given.
*/
// Include the library:
#include "SparkFunMMA8452Q.h"

// Create an MMA8452Q object, used throughout the rest of the sketch.
MMA8452Q accel;  // Default constructor, SA0 pin is HIGH

// The above works if the MMA8452Q's address select pin (SA0) is high.
// If SA0 is low (if the jumper on the back of the SparkFun MMA8452Q breakout
// board is closed), initialize it like this:
// MMA8452Q accel(MMA8452Q_ADD_SA0_);

const int PIN_RED = A0;
const int PIN_GREEN = A1;
const int PIN_BLUE = A2;

const int ARR_SIZE = 5;
float xArr[ARR_SIZE];
float yArr[ARR_SIZE];
float zArr[ARR_SIZE];

int curInd = 0;

// The setup function simply starts serial and initializes the
//  accelerometer.
void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Test Code!");
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  // Initialize the accelerometer with begin():
  // begin can take two parameters: full-scale range, and output data rate
  // (ODR). Full-scale range can be: SCALE_2G, SCALE_4G, or SCALE_8G (2, 4, or
  // 8g) ODR can be: ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12, ODR_6
  // or ODR_1
  accel.begin(SCALE_2G,
              ODR_400);  // Set up accel with +/-2g range, and slowest (1Hz) ODR
}

void loop() {
  // accel.available() will return 1 if new data is available, 0 otherwise
  if (accel.available()) {
    // To update acceleration values from the accelerometer, call accel.read();
    accel.read();

    // printAccelGraphXYZ();

    updateAccelArray(accel.cx, accel.cy, accel.cz);

    // changeRgbWithOrientation();
    // changeRgbWithAcceleration();
    if (accel.readTap()) {
      changeRgbLight(255, 255, 255);
    }
  }

  // No need to delay, since our ODR is set to 1Hz, accel.available() will only
  // return 1 about once per second.
}

void updateAccelArray(float x, float y, float z) {
  xArr[curInd] = x;
  yArr[curInd] = y;
  zArr[curInd] = z;
  curInd++;
  if (curInd > ARR_SIZE) {
    curInd = 0;
  }
}

void printAccelGraphXYZ() {
  // After reading, six class variables are updated: x, y, z, cx, cy, and cz.
  // Those are the raw, 12-bit values (x, y, and z) and the calculated
  // acceleration's in units of g (cx, cy, and cz).

  // use the printAccelGraph funciton to print the values along with a bar
  // graph, to see their relation to eachother:
  printAccelGraph(accel.cx, "X", 20, 2.0);
  printAccelGraph(accel.cy, "Y", 20, 2.0);
  printAccelGraph(accel.cz, "Z", 20, 2.0);
  Serial.println();
}

// printAccelGraph prints a simple ASCII bar graph for a single accelerometer
// axis value. Examples:
//	printAccelGraph(-0.1, "X", 20, 2.0) will print:
// 		X:                    =|                     (0.1 g)
//	printAccelGraph(1.0, "Z", 20, 2.0) will print:
//		Z:                     |==========           (1.0 g)
// Input:
//	- [value]: calculated value of an accelerometer axis (e.g accel.cx,
// accel.cy)
//	- [name]: name of the axis (e.g. "X", "Y", "Z")
//	- [numBarsFull]: Maximum number of bars either right or left of 0 point.
//	- [rangeAbs]: Absolute value of the maximum acceleration range
void printAccelGraph(float value, String name, int numBarsFull,
                     float rangeAbs) {
  // Calculate the number of bars to fill, ignoring the sign of numBars for now.
  int numBars = abs(value / (rangeAbs / numBarsFull));

  Serial.print(name + ": ");  // Print the axis name and a colon:

  // Do the negative half of the graph first:
  for (int i = 0; i < numBarsFull; i++) {
    if (value < 0)  // If the value is negative
    {
      // If our position in the graph is in the range we want to graph
      if (i >= (numBarsFull - numBars))
        Serial.print('=');  // Print an '='
      else
        Serial.print(' ');  // print spaces otherwise
    } else                  // If our value is positive, just print spaces
      Serial.print(' ');
  }

  Serial.print('|');  // Print a pipe (|) to represent the 0-point

  // Do the positive half of the graph last:
  for (int i = 0; i < numBarsFull; i++) {
    if (value >
        0) {  // If our position in the graph is in the range we want to graph
      if (i <= numBars)
        Serial.print('=');  // Print an '='
      else
        Serial.print(' ');  // otherwise print spaces
    } else                  // If value is negative, just print spaces
      Serial.print(' ');
  }

  // To end the line, print the actual value:
  Serial.println(" (" + String(value, 2) + " g)");
}
// This function demonstrates how to use the accel.readPL()
// function, which reads the portrait/landscape status of the
// sensor.
void printOrientation() {
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
  byte pl = accel.readPL();
  switch (pl) {
    case PORTRAIT_U:
      Serial.print("Portrait Up");
      break;
    case PORTRAIT_D:
      Serial.print("Portrait Down");
      break;
    case LANDSCAPE_R:
      Serial.print("Landscape Right");
      break;
    case LANDSCAPE_L:
      Serial.print("Landscape Left");
      break;
    case LOCKOUT:
      Serial.print("Flat");

      break;
  }
}

void changeRgbWithOrientation() {
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
  byte pl = accel.readPL();
  switch (pl) {
    case PORTRAIT_U:
      changeRgbLight(0, 0, 255);
      break;
    case PORTRAIT_D:
      changeRgbLight(0, 255, 0);
      break;
    case LANDSCAPE_R:
      changeRgbLight(255, 0, 0);
      break;
    case LANDSCAPE_L:
      changeRgbLight(255, 0, 255);
      break;
    case LOCKOUT:
      changeRgbLight(255, 255, 255);

      break;
  }
}
float avg(float accel[]) {
  float sum = 0;
  for (int i = 0; i < ARR_SIZE; i++) {
    sum += accel[i];
  }
  return sum / ARR_SIZE;
}

void changeRgbWithAcceleration() {
  accel.read();
//   float x = (accel.cx);
//   float y = (accel.cy);
//   float z = (accel.cz);

  float x = avg(xArr);
  float y = avg(yArr);
  float z = avg(zArr);
  int r, g, b;
  r = map(x, -1.0, 1.0, 0.0, 255.0);
  g = map(y, -1.0, 1.0, 0.0, 255.0);
  b = map(z, -1.0, 1.0, 0.0, 255.0);

  //   float x = abs(constrain(accel.cx, -1.0, 1.0));
  //   float y = abs(constrain(accel.cy, -1.0, 1.0));
  //   float z = abs(constrain(accel.cz, -1.0, 1.0));

  //   r = map(x, 0.0, 1.0, 0.0, 255.0);
  //   g = map(y, 0.0, 1.0, 0.0, 255.0);
  //   b = map(z, 0.0, 1.0, 0.0, 255.0);

  changeRgbLight(r, g, b);

  /* === DEBUG PRINTING === */
  Serial.print("(r,g,b): (" + String(r) + ", " + String(g) + "," + String(b) +
               ")");
  Serial.println(" -- (x,y,z): (" + String(x, 2) + ", " + String(y, 2) + ", " +
                 String(z, 2) + ")");
}

void changeRgbLight(int r, int g, int b) {
  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE, b);
}