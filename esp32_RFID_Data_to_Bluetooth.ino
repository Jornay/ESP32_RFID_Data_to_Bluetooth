#include <Wire.h>
#include <Adafruit_PN532.h>
#include "BluetoothSerial.h"

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);
const char *pin = "1234";  // Change this to a more secure PIN.

//------------------BLUETOOTH-------------------------//
String device_name = "ESP32-BT-Slave";
BluetoothSerial SerialBT;
//----------------------------------------------------------//

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello! NFC reader with ESP32");

//------------------BLUETOOTH-------------------------//
  SerialBT.begin(device_name);  // Inicializa o Bluetooth com o nome do dispositivo
  
  #ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
  pinMode(19, OUTPUT);
  #endif
//---------------------------------------------------//
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  nfc.SAMConfig();  // Configure board to read NFC tags
  Serial.println("Waiting for an NFC card ...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Armazena os códigos lidos em uma matriz de caracteres
  char uidCodes[64] = ""; // Tamanho máximo da string

  // Wait for an NFC card to be detected
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an NFC card!");

    // Display some basic information about the card
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);

      // Adiciona o código lido à variável uidCodes
      char hexCode[3];
      sprintf(hexCode, "%02X", uid[i]);
      strcat(uidCodes, hexCode);
    }
    Serial.println("");

    // Envia a variável uidCodes pelo Bluetooth
    SerialBT.println("RFID Codes:");
    SerialBT.println(uidCodes);

    // Wait 1 second before continuing
    delay(1000);
  }
}
