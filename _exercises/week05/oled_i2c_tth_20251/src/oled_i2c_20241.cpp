#include "Particle.h"
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_WARN);

/*
    some sensors are very simple and are easy to write to communicate with
        ex: tmp36 or photoresistor
    for more complicated sensor (ex oled, accelerometer), there is a lot LOW LEVEL code needed to communicate
        ==> instead, we will use LIBRARIES

    libraries is a collection of function and objects, and we use libraries that simplify communicating with complicated sensor

    how to find libraries
    - look at community.particle.io
    - search the particle library
    - internet (eg accelerometer MGPU4050)
    - datasheet

*/

/******************************************************************************
  Micro-OLED-Shield-Example.ino
  SparkFun Micro OLED Library Hello World Example
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: June 22, 2015

  This sketch prints a friendly, recognizable logo on the OLED Shield, then
  goes on to demo the Micro OLED library's functionality drawing pixels,
  lines, shapes, and text.

  Hardware Connections:
        This sketch was written specifically for the Photon Micro OLED Shield,
        which does all the wiring for you. If you have a Micro OLED breakout,
        use the following hardware setup:

    MicroOLED ------------- Photon
      GND ------------------- GND
      VDD ------------------- 3.3V (VCC)
    D1/MOSI ----------------- A5 (don't change)
    D0/SCK ------------------ A3 (don't change)
      D2
      D/C ------------------- D6 (can be any digital pin)
      RST ------------------- D7 (can be any digital pin)
      CS  ------------------- A2 (can be any digital pin)

  Development environment specifics:
        IDE: Particle Build
        Hardware Platform: Particle Photon
                       SparkFun Photon Micro OLED Shield

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!

  Distributed as-is; no warranty is given.
*******************************************************************************/
#include "SparkFunMicroOLED.h"  // Include MicroOLED library
#include "math.h"

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
// Declare a MicroOLED object. If no parameters are supplied, default pins are
// used, which will work for the Photon Micro OLED Shield (RST=D7, DC=D6, CS=A2)
// MicroOLED oled;
MicroOLED oled(MODE_I2C, 9, 1);    // Example I2C declaration RST=D7, DC=LOW
// (0)

// SYSTEM_MODE(MANUAL);

// SYSTEM_MODE(MANUAL);
const int PIN_POT1 = A0;
const int PIN_POT2 = A1;
const int PIN_BUTTON = D2;

int prevButtonVal = HIGH;  //latch

// Center and print a small title
// This function is quick and dirty. Only works for titles one
// line long.
void printTitle(String title, int font) {
    int middleX = oled.getLCDWidth() / 2;
    int middleY = oled.getLCDHeight() / 2;

    oled.clear(PAGE);
    oled.setFontType(font);
    // Try to set the cursor in the middle of the screen
    oled.setCursor(middleX - (oled.getFontWidth() * (title.length() / 2)),
                   middleY - (oled.getFontWidth() / 2));
    // Print the title:
    oled.print(title);
    oled.display();
    delay(1500);
    oled.clear(PAGE);
}



void pixelExample() {
    printTitle("Pixels", 1);

    for (int i = 0; i < 512; i++) {
        oled.pixel(random(oled.getLCDWidth()), random(oled.getLCDHeight()));
        oled.display();
    }
}

void lineExample() {
    int middleX = oled.getLCDWidth() / 2;
    int middleY = oled.getLCDHeight() / 2;
    int xEnd, yEnd;
    int lineWidth = min(middleX, middleY);

    printTitle("Lines!", 1);

    for (int i = 0; i < 3; i++) {
        for (int deg = 0; deg < 360; deg += 15) {
            xEnd = lineWidth * cos(deg * M_PI / 180.0);
            yEnd = lineWidth * sin(deg * M_PI / 180.0);

            oled.line(middleX, middleY, middleX + xEnd, middleY + yEnd);
            oled.display();
            delay(10);
        }
        for (int deg = 0; deg < 360; deg += 15) {
            xEnd = lineWidth * cos(deg * M_PI / 180.0);
            yEnd = lineWidth * sin(deg * M_PI / 180.0);

            oled.line(middleX, middleY, middleX + xEnd, middleY + yEnd, BLACK,
                      NORM);
            oled.display();
            delay(10);
        }
    }
}

void shapeExample() {
    printTitle("Shapes!", 0);

    // Silly pong demo. It takes a lot of work to fake pong...
    int paddleW = 3;   // Paddle width
    int paddleH = 15;  // Paddle height
    // Paddle 0 (left) position coordinates
    int paddle0_Y = (oled.getLCDHeight() / 2) - (paddleH / 2);
    int paddle0_X = 2;
    // Paddle 1 (right) position coordinates
    int paddle1_Y = (oled.getLCDHeight() / 2) - (paddleH / 2);
    int paddle1_X = oled.getLCDWidth() - 3 - paddleW;
    int ball_rad = 2;  // Ball radius
    // Ball position coordinates
    int ball_X = paddle0_X + paddleW + ball_rad;
    int ball_Y = random(
        1 + ball_rad, oled.getLCDHeight() - ball_rad);  // paddle0_Y + ball_rad;
    int ballVelocityX = 1;     // Ball left/right velocity
    int ballVelocityY = 1;     // Ball up/down velocity
    int paddle0Velocity = -1;  // Paddle 0 velocity
    int paddle1Velocity = 1;   // Paddle 1 velocity

    // while(ball_X >= paddle0_X + paddleW - 1)
    while ((ball_X - ball_rad > 1) &&
           (ball_X + ball_rad < oled.getLCDWidth() - 2)) {
        // Increment ball's position
        ball_X += ballVelocityX;
        ball_Y += ballVelocityY;
        // Check if the ball is colliding with the left paddle
        if (ball_X - ball_rad < paddle0_X + paddleW) {
            // Check if ball is within paddle's height
            if ((ball_Y > paddle0_Y) && (ball_Y < paddle0_Y + paddleH)) {
                ball_X++;  // Move ball over one to the right
                ballVelocityX = -ballVelocityX;  // Change velocity
            }
        }
        // Check if the ball hit the right paddle
        if (ball_X + ball_rad > paddle1_X) {
            // Check if ball is within paddle's height
            if ((ball_Y > paddle1_Y) && (ball_Y < paddle1_Y + paddleH)) {
                ball_X--;  // Move ball over one to the left
                ballVelocityX = -ballVelocityX;  // change velocity
            }
        }
        // Check if the ball hit the top or bottom
        if ((ball_Y <= ball_rad) ||
            (ball_Y >= (oled.getLCDHeight() - ball_rad - 1))) {
            // Change up/down velocity direction
            ballVelocityY = -ballVelocityY;
        }
        // Move the paddles up and down
        paddle0_Y += paddle0Velocity;
        paddle1_Y += paddle1Velocity;
        // Change paddle 0's direction if it hit top/bottom
        if ((paddle0_Y <= 1) ||
            (paddle0_Y > oled.getLCDHeight() - 2 - paddleH)) {
            paddle0Velocity = -paddle0Velocity;
        }
        // Change paddle 1's direction if it hit top/bottom
        if ((paddle1_Y <= 1) ||
            (paddle1_Y > oled.getLCDHeight() - 2 - paddleH)) {
            paddle1Velocity = -paddle1Velocity;
        }

        // Draw the Pong Field
        oled.clear(PAGE);  // Clear the page
        // Draw an outline of the screen:
        oled.rect(0, 0, oled.getLCDWidth() - 1, oled.getLCDHeight());
        // Draw the center line
        oled.rectFill(oled.getLCDWidth() / 2 - 1, 0, 2, oled.getLCDHeight());
        // Draw the Paddles:
        oled.rectFill(paddle0_X, paddle0_Y, paddleW, paddleH);
        oled.rectFill(paddle1_X, paddle1_Y, paddleW, paddleH);
        // Draw the ball:
        oled.circle(ball_X, ball_Y, ball_rad);
        // Actually draw everything on the screen:
        oled.display();
        delay(25);  // Delay for visibility
    }
    delay(1000);
}

void textExamples() {
    printTitle("Text!", 1);

    // Demonstrate font 0. 5x8 font
    oled.clear(PAGE);      // Clear the screen
    oled.setFontType(0);   // Set font to type 0
    oled.setCursor(0, 0);  // Set cursor to top-left
    // There are 255 possible characters in the font 0 type.
    // Lets run through all of them and print them out!
    for (int i = 0; i <= 255; i++) {
        // You can write byte values and they'll be mapped to
        // their ASCII equivalent character.
        oled.write(i);   // Write a byte out as a character
        oled.display();  // Draw on the screen
        delay(10);       // Wait 10ms
        // We can only display 60 font 0 characters at a time.
        // Every 60 characters, pause for a moment. Then clear
        // the page and start over.
        if ((i % 60 == 0) && (i != 0)) {
            delay(500);            // Delay 500 ms
            oled.clear(PAGE);      // Clear the page
            oled.setCursor(0, 0);  // Set cursor to top-left
        }
    }
    delay(500);  // Wait 500ms before next example

    // Demonstrate font 1. 8x16. Let's use the print function
    // to display every character defined in this font.
    oled.setFontType(1);   // Set font to type 1
    oled.clear(PAGE);      // Clear the page
    oled.setCursor(0, 0);  // Set cursor to top-left
    // Print can be used to print a string to the screen:
    oled.print(" !\"#$%&'()*+,-./01234");
    oled.display();  // Refresh the display
    delay(1000);     // Delay a second and repeat
    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.print("56789:;<=>?@ABCDEFGHI");
    oled.display();
    delay(1000);
    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.print("JKLMNOPQRSTUVWXYZ[\\]^");
    oled.display();
    delay(1000);
    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.print("_`abcdefghijklmnopqrs");
    oled.display();
    delay(1000);
    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.print("tuvwxyz{|}~");
    oled.display();
    delay(1000);

    // Demonstrate font 2. 10x16. Only numbers and '.' are defined.
    // This font looks like 7-segment displays.
    // Lets use this big-ish font to display readings from the
    // analog pins.
    for (int i = 0; i < 25; i++) {
        oled.clear(PAGE);            // Clear the display
        oled.setCursor(0, 0);        // Set cursor to top-left
        oled.setFontType(0);         // Smallest font
        oled.print("A0:");           // Print "A0"
        oled.setFontType(2);         // 7-segment font
        oled.print(analogRead(A0));  // Print a0 reading
        oled.setCursor(0, 16);       // Set cursor to top-middle-left
        oled.setFontType(0);         // Repeat
        oled.print("A1:");
        oled.setFontType(2);
        oled.print(analogRead(A1));
        oled.setCursor(0, 32);
        oled.setFontType(0);
        oled.print("A7:");
        oled.setFontType(2);
        // oled.print(analogRead(A7)); //what is wrong here? - >Photon2 has A0-A5
        oled.display();
        delay(100);
    }

    // Demonstrate font 3. 12x48. Stopwatch demo.
    oled.setFontType(3);  // Use the biggest font
    int ms = 0;
    int s = 0;
    while (s <= 50) {
        oled.clear(PAGE);      // Clear the display
        oled.setCursor(0, 0);  // Set cursor to top-left
        if (s < 10)
            oled.print("00");  // Print "00" if s is 1 digit
        else if (s < 100)
            oled.print("0");  // Print "0" if s is 2 digits
        oled.print(s);        // Print s's value
        oled.print(":");      // Print ":"
        oled.print(ms);       // Print ms value
        oled.display();       // Draw on the screen
        ms++;                 // Increment ms
        if (ms >= 10)         // If ms is >= 10
        {
            ms = 0;  // Set ms back to 0
            s++;     // and increment s
        }
        delay(1);
    }
}

void setup() {
    Serial.begin(9600);
    oled.begin();     // Initialize the OLED
    oled.clear(ALL);  // Clear the display's internal memory
    oled.display();   // Display what's in the buffer (splashscreen)
    delay(1000);      // Delay 1000 ms
    randomSeed(analogRead(A0) + analogRead(A1));
    pinMode(PIN_BUTTON, INPUT);
    pinMode(PIN_POT1, INPUT);
    pinMode(PIN_POT2, INPUT);

    oled.clear(PAGE);
}

void loop() {
    int xPot = analogRead(PIN_POT1);
    int yPot = analogRead(PIN_POT2); //range 0-4095

    int xMax = oled.getLCDWidth(); // max x
    int yMax = oled.getLCDHeight(); // max y

    int xPixel = map(xPot, 0, 4095, 0, xMax);
    int yPixel = map(yPot, 0, 4095, 0, yMax);

    oled.pixel(xPixel, yPixel);

    oled.display();

    //clear screen with latch
    int curButtonVal = digitalRead(PIN_BUTTON);
    if (curButtonVal == LOW && prevButtonVal == HIGH) {
        oled.clear(PAGE);
    }
    prevButtonVal = curButtonVal;




    // pixelExample();  // Run the pixel example function
    // lineExample();   // Then the line example function
    // shapeExample();  // Then the shape example
    // textExamples();  // Finally the text example

    // print out "Hello!" on one line
    //print out "ITP 348" on next line
    // DONT use printTitle()

    // oled.clear(PAGE);   // clears anything from the screen
    // oled.setFontType(0);
    // oled.setCursor(0,0);
    // // oled.println("Hello!");
    // // oled.println("ITP 348");
    // oled.print("Hello!");

    // oled.setCursor(0,10);
    // oled.print("ITP348");
    // //after we add any text or graphics to the screen, call display

    // oled.display();

    /*
        one possibility: use line or shape function
        print out dots to represent pixel
        or create array of x and y to represents dots

        use pots as knobs
        use analogread with pots
        button to clear screen
        
    */



}