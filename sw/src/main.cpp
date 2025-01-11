#include <WiFi.h>
#include <esp_task_wdt.h>
#include "wifi_creds.hpp"

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("booting");

  esp_task_wdt_init(5/*s*/, true);
  esp_task_wdt_add(NULL);   // add current task to watchdog watch
  Serial.println("watchdog enabled");

  // TODO
}

void loop()
{
  Serial.println("i'm alive!");
  esp_task_wdt_reset();
  delay(1000);
  // TODO
}
