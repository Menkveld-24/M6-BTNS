#include <Arduino.h>

#define LED_PIN 2 //gpio2 is built in led

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Turned led on");
  delay(500);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Turned led off");
  delay(500);
}
