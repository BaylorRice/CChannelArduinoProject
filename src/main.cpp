#include <Arduino.h>

void loop() {
  // put your main code here, to run repeatedly:
  btnList.handle();
  detect data;
  NewPing selection(0,0,0);
  bool i = true;

  Serial.println("Start");
  
  while (i) {
    if(greenStart.resetClicked()) {
      Serial.print("Green\n");
      i = false;
    }
    if (goldStart.resetClicked()) {
      Serial.print("Gold\n");
      i = false;
    }

  }
  delay(1000);
  
  

  Serial.println("\nStop");
  delay(1000);
}