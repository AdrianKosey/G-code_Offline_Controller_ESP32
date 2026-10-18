#include <Arduino.h>
#include <nvs_flash.h>
#include "app/app.h"

App* app = nullptr;

void setup()
{
  Serial.begin(115200);
  pinMode(TFT_CS, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  pinMode(CH376_CS_PIN, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(SD_CS_PIN, HIGH);
  digitalWrite(CH376_CS_PIN, HIGH);
  digitalWrite(TOUCH_CS, HIGH);
  app = new App();

  app->begin();
}

void loop()
{
  app->update();
}