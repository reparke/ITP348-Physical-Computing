/*
 * MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W
 AND R BY COOQROBOT.
 * The library file MFRC522.h has a wealth of useful info. Please read it.
 * The functions are documented in MFRC522.cpp.
 *
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013
 (Translation to English, refactored, comments, anti collision, cascade levels.)
 * Released into the public domain.
 *
 * Sample program showing how to read data from a PICC using a MFRC522 reader on
 the Arduino SPI interface.
 *-----------------------------------------------------------------------------
 empty_skull
 * Aggiunti pin per arduino Mega
 * add pin configuration for arduino mega
 * http://mac86project.altervista.org/
 -----------------------------------------------------------------------------
 Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin			Pin
 *            Arduino Uno      Arduino Mega      SPARK MFRC522 board
 * ---------------------------------------------------------------------------
 * Reset      9                5                 ANY (D2)		RST
 * SPI SS     10               53                ANY (A2)		SDA
 * SPI MOSI   11               51                A5
 MOSI
 * SPI MISO   12               50                A4
 MISO
 * SPI SCK    13               52                A3 SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522"
 on ebay.com.
 */

//#include <SPI.h>
#include "MFRC522.h"

#define SS_PIN A4
#define RST_PIN A5

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

String card1 = "0E 8B 8E 6A";
String card2 = "4E B3 01 BF";

void setup() {
    Serial.begin(9600);  // Initialize serial communications with the PC
    mfrc522.setSPIConfig();

    mfrc522.PCD_Init();  // Init MFRC522 card
    Serial.println("Scan PICC to see UID and type...");
    pinMode(D7, OUTPUT);
}

void loop() {
    String scannedCard = "";
    if (mfrc522.PICC_IsNewCardPresent() == true) {
        if (mfrc522.PICC_ReadCardSerial() == true) {
            for (byte i = 0; i < mfrc522.uid.size; i++) {
                scannedCard +=
                    String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                scannedCard += String(mfrc522.uid.uidByte[i], HEX);
            }
            scannedCard.toUpperCase(); //convert to upper case
            scannedCard.trim();


            
            Serial.println("Scanned card: |" + scannedCard + "|");
            if (scannedCard == card1) {
              digitalWrite(D7, HIGH);
              Serial.println("Card1 is present!");
            }
            else if (scannedCard == card2) {
              digitalWrite(D7, LOW);
              Serial.println("Card2 is present!");
            }
            delay(2000);
        }
    }
}

void displayCardId() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Dump debug info about the card. PICC_HaltA() is automatically called.
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}