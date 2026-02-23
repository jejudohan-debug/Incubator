#include <Arduino.h>
#include "D_Logic/IncubatorLogic.h"

IncubatorLogic _incubator;

void setup() {
  Serial.begin(9600);
  Serial.println("main Setup");

  _incubator.init();
}

void loop() {
  Serial.println("main Loop");


  delay(2000);
}
