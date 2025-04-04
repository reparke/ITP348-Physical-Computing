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

// #include <MFRC522.h>
// #include <SPI.h>

// constexpr uint8_t RST_PIN = 9;  // Configurable, see typical pin layout above
// constexpr uint8_t SS_PIN = 10;  // Configurable, see typical pin layout above

const int RST_PIN = A5;
const int SS_PIN = A4;

const String card1 = "0E 8B 8E 6A";  // hexadecimal
const String card2 = "4E B3 01 BF";

/*
    Decimal: 0 1 2 3 .. 8 9  10   11   12  13  14  15    16    17
Hexadecimal: 0 1 2 .... 8 9  A    B    C   D   E   F     10    11

binary: 0  1
2^4 = 16


*/

unsigned long prevMillis = 0;
const unsigned long READ_DELAY = 2000;
int lightState = 0;
const int PIN_LED = D7;

// MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
    Serial.begin(9600);  // Initialize serial communications with the PC
    while (!Serial)
        ;  // Do nothing if no serial port is opened (added for Arduinos based
           // on ATMEGA32U4)
    // SPI.begin();                        // Init SPI bus
    // mfrc522.PCD_Init();                 // Init MFRC522
    // mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card
                                        // Reader details
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
    // pinMode(PIN_LED, OUTPUT);
}

void loop()
{
    
}