#include <SPI.h>
#include <MFRC522.h>

const byte numReaders = 3;
const byte slaveSelPins[] = {2, 3, 4};
const byte resetPin = 7;
MFRC522 mfrc522[numReaders];
const String solutionIds[numReaders] = {"ID1", "ID2", "ID3"};
String currentIds[numReaders];
const byte relayPin = A0;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  SPI.begin();

  for (uint8_t i=0; i<numReaders; i++) {
    // Inicializa el lector
    mfrc522[i].PCD_Init(slaveSelPins[i], resetPin);
    // Un pequeño delay antes de activar
    delay(100);
  }
}

void loop() {
  bool solved = true;
  bool changedValue = false;
  for (uint8_t i=0; i<numReaders; i++) {
    mfrc522[i].PCD_Init();

    String readRFID = "";

    // Si se ha detectado una tarjeta NFC
    if (mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()) {
      // Obtenemos el valor del NFC
      // TODO No queda claro como se obtiene el valor 
      readRFID = mfrc522[i].PICC_ReadCardSerial();
    }

    if (readRFID != currentIds[i]) {
      changedValue = true;
      currentIds[i] = readRFID;
    }

    if (currentIds[i] != solutionIds[i]) {
      solved = false;
    }

    mfrc522[i].PICC_HaltA();
    mfrc522[i].PCD_StopCrypto1();
  }

  if (solved) {
    digitalWrite(relayPin, LOW);
  } else {
    digitalWrite(relayPin, HIGH);
  }
}
