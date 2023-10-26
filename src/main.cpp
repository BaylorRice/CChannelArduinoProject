#include <Arduino.h>
#include "detect.cpp"

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Start");
  delay(1000);


  detect data;
  bool cases, pallet, button;
  NewPing selection(0,0,0);


  cases = data.getCaseReady();
  data.setCaseReady(true);
  Serial.print(cases);
  pallet = data.getPalletReady();
  data.setPalletReady(true);
  Serial.print(pallet);
  button = data.getButtonReady();
  data.setButtonReady(true);
  Serial.print(button);
  selection = data.detectPress(&btnList);


  Serial.println("\nStop");
  delay(1000);
}