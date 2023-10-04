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

// Defina os pinos dos LEDs
const int LED_PIN_1 = 18;
const int LED_PIN_2 = 19;
const int LED_PIN_3 = 23;
const int LED_PIN_4 = 25;

// Variáveis para armazenar os IDs dos cartões
char card1[64] = "0";
char card2[64] = "0";
char card3[64] = "0";
char card4[64] = "0";

// Variáveis para rastrear o estado dos LEDs
bool led1On = false;
bool led2On = false;
bool led3On = false;
bool led4On = false;
void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello! NFC reader with ESP32");

  SerialBT.begin(device_name);  // Inicializa o Bluetooth com o nome do dispositivo

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

  nfc.SAMConfig();  // Configure board to read NFC tags
  Serial.println("Waiting for an NFC card ...");

  // Configure os pinos dos LEDs como saída
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);

  // Desliga todos os LEDs inicialmente
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_3, LOW);
  digitalWrite(LED_PIN_4, LOW);
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Armazena os códigos lidos em uma matriz de caracteres
  char uidCodes[64] = "";  // Tamanho máximo da string

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

    

    // Verifica o código do cartão e controla o LED correspondente
    if (strcmp(uidCodes, card1) == 0 || strlen(card1) == 1) {
      if (led1On) {
        digitalWrite(LED_PIN_1, LOW);
        led1On = false;
        SerialBT.println("Code User 1 disconnected:");
        SerialBT.println(uidCodes);
      } else {
        digitalWrite(LED_PIN_1, HIGH);
        led1On = true;
        SerialBT.println("Code User 1 initiate:");
        SerialBT.println(uidCodes);
      }
      

      strcpy(card1, uidCodes);
    } else if (strcmp(uidCodes, card2) == 0 || strlen(card2) == 1) {
      if (led2On) {
        digitalWrite(LED_PIN_2, LOW);
        led2On = false;
        SerialBT.println("Code User 2 disconnected:");
        SerialBT.println(uidCodes);
      } else {
        digitalWrite(LED_PIN_2, HIGH);
        led2On = true;
        SerialBT.println("Code User 2 initiate:");
        SerialBT.println(uidCodes);
      }

      strcpy(card2, uidCodes);
    } else if (strcmp(uidCodes, card3) == 0 || strlen(card3) == 1) {
      if (led3On) {
        digitalWrite(LED_PIN_3, LOW);
        led3On = false;
        SerialBT.println("Code User 3 disconnected:");
        SerialBT.println(uidCodes);
      } else {
        digitalWrite(LED_PIN_3, HIGH);
        led3On = true;
        SerialBT.println("Code User 3 initiate:");
        SerialBT.println(uidCodes);
      }
      
      strcpy(card3, uidCodes);
    } else if (strcmp(uidCodes, card4) == 0 || strlen(card4) == 1) {
      if (led4On) {
        digitalWrite(LED_PIN_4, LOW);
        led4On = false;
        SerialBT.println("Code User 4 disconnected:");
        SerialBT.println(uidCodes);
      } else {
        digitalWrite(LED_PIN_4, HIGH);
        led4On = true;
        SerialBT.println("Code User 4 initiate:");
        SerialBT.println(uidCodes);
      }
      strcpy(card4, uidCodes);
    }

    // Aguarda 1 segundo antes de continuar
    delay(1000);
  }
}
