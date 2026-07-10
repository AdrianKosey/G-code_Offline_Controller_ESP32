#include <Arduino.h>
#include "app/app.h"

App app;

void setup() {
  app.begin();
}

void loop() {
  app.update();
}
