#include <Wire.h>
#include <Adafruit_PN532.h>
#include "BluetoothSerial.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define LED_PIN 13 // Pino do LED

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);
const char *pin = "1234";  // Altere isso para um PIN mais seguro.

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

  pinMode(LED_PIN, OUTPUT); // Configura o pino do LED como saída

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  nfc.SAMConfig();  // Configura a placa para ler tags NFC
  Serial.println("Waiting for an NFC card ...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer para armazenar o UID retornado
  uint8_t uidLength;                        // Comprimento do UID (4 ou 7 bytes dependendo do tipo de cartão ISO14443A)

  // Armazena os códigos lidos em uma matriz de caracteres
  char uidCodes[64] = ""; // Tamanho máximo da string

  // Aguarda a detecção de um cartão NFC
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

    // Acende o LED por 500 milissegundos (meio segundo)
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);

    // Aguarda 1 segundo antes de continuar
    delay(1000);
  }
}
