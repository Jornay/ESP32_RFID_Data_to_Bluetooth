#include <Wire.h>
#include <Adafruit_PN532.h>
#include "BluetoothSerial.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define LED_PIN 18  // Pino do LED

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);
const char *pin = "1234";  // Senha para conexão Bluetooth.

String device_name = "ESP32-BT-Slave";
BluetoothSerial SerialBT;

bool cardDetected = false;  // Variável para rastrear se um cartão foi detectado

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello! NFC reader with ESP32");
  SerialBT.begin(device_name);

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
  pinMode(19, OUTPUT);
#endif

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  nfc.SAMConfig();
  Serial.println("Waiting for an NFC card ...");

  pinMode(LED_PIN, OUTPUT);    // Configura o pino do LED como saída
  digitalWrite(LED_PIN, LOW);  // Desliga o LED inicialmente
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  char uidCodes[64] = "";

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an NFC card!");

    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");

    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);

      char hexCode[3];
      sprintf(hexCode, "%02X", uid[i]);
      strcat(uidCodes, hexCode);
    }

    Serial.println("");
    SerialBT.println("RFID Codes:");
    SerialBT.println(uidCodes);
    if (cardDetected) {
      digitalWrite(LED_PIN, LOW);
      cardDetected = false;
      
    } else {
      digitalWrite(LED_PIN, HIGH);
      cardDetected = true;

    }
    delay(1000);
  }
}
