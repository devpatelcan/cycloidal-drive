#include <Arduino.h>

#define FOC_RX_PIN 32
#define FOC_TX_PIN 33

void setup() {

  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, FOC_RX_PIN, FOC_TX_PIN);
}

void loop() {
  while (Serial.available()) {
    Serial2.write(Serial.read());
  }

  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}
