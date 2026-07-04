#include <Arduino.h>

// Safe pins for ESP32-WROVER (avoids internal PSRAM conflict)
#define FOC_RX_PIN 32
#define FOC_TX_PIN 33

void setup() {
  // 1. Start USB Serial for Teleplot 
  Serial.begin(115200);

  // 2. Start hardware Serial2 for the FOC board
  Serial2.begin(115200, SERIAL_8N1, FOC_RX_PIN, FOC_TX_PIN);
}

void loop() {
  // If you type a command in Teleplot, push it out to the FOC board
  while (Serial.available()) {
    Serial2.write(Serial.read());
  }

  // If the FOC board sends telemetry, push it up to Teleplot
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}