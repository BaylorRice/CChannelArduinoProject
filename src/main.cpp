#include <Arduino.h>
#include "detect.cpp"

void loop() {
  // put your main code here, to run repeatedly:
  detect data;
  NewPing test(23,22,200);
  Serial.println("Start");
  delay(1000);
  data.caseDetect(test);
  Serial.print(data.getCaseReady());

  Serial.println("\nStop");
  delay(1000);
}