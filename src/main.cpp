#include <Arduino.h>
#include "app/app.h"

App app;

void setup() {
  Serial.begin(115200);
  app.begin();
}

void loop() {
  app.update();
}
