/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_WARN);
/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples
 * see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from a PICC (that is: a RFID
 * Tag or Card) using a MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * When the Arduino and the MFRC522 module are connected (see the pin layout
 * below), load this sketch into Arduino IDE then verify/compile and upload it.
 * To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M).
 * When you present a PICC (that is: a RFID Tag or Card) at reading distance of
 * the MFRC522 Reader/PCD, the serial output will show the ID/UID, type and any
 * data blocks it can read. Note: you may see "Timeout in communication"
 * messages when removing the PICC from reading distance too early.
 *
 * If your reader supports it, this sketch/program will read all the PICCs
 * presented (that is: multiple tag reading). So if you stack two or more PICCs
 * on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some
 * time as all data blocks are dumped, so keep the PICCs at reading distance
 * until complete.
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro
 * Pro Micro Signal      Pin          Pin           Pin       Pin        Pin Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5 RST
 * SPI SS      SDA(SS)      10            53        D10        10 10 SPI MOSI
 * MOSI         11 / ICSP-4   51        D11        ICSP-4           16 SPI MISO
 * MISO         12 / ICSP-1   50        D12        ICSP-1           14 SPI SCK
 * SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

// #include <SPI.h>
#include "MFRC522.h"

#define SS_PIN A0
#define RST_PIN A1
const String MATCH_ID = "0E 8B 8E 6A";  // target id to match
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void setup() {
    Serial.begin(9600);  // Initialize serial communications with the PC
    mfrc522.setSPIConfig();

    mfrc522.PCD_Init();  // Init MFRC522 card
    Serial.println("Scan PICC to see UID and type...");
}

void loop() {
    // // Look for new cards
    // if (!mfrc522.PICC_IsNewCardPresent()) {
    //     return;
    // }

    // // Select one of the cards
    // if (!mfrc522.PICC_ReadCardSerial()) {
    //     return;
    // }

    // // Dump debug info about the card. PICC_HaltA() is automatically called.
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

    String scanId = "";
    if (mfrc522.PICC_IsNewCardPresent()) {    // check sensor
        if (mfrc522.PICC_ReadCardSerial()) {  // check valid read
            for (byte i = 0; i < mfrc522.uid.size; i++) {
                scanId += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                scanId += String(mfrc522.uid.uidByte[i], HEX);
            }
            scanId.toUpperCase();  // scanId will be lowercase
            scanId.trim();         // scanId has an intial leading " "
                            // now we can check for a match!
            if (scanId == MATCH_ID) {
                // we found a match!
                Serial.println("found match");
            }
        }
    }
}
