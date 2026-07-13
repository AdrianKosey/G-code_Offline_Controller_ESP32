#include <Arduino.h>
#include <nvs_flash.h>
#include "app/app.h"

App* app = nullptr;

void setup()
{
  Serial.begin(115200);

  esp_err_t err = nvs_flash_init();

  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    Serial.println("Corrupted NVS, erasing and restarting...");
    nvs_flash_erase();
    err = nvs_flash_init();
  }

  Serial.printf("nvs_flash_init() = %s\n", err == ESP_OK ? "OK" : "FAIL");

  app = new App();

  app->begin();
}

void loop()
{
  app->update();
}