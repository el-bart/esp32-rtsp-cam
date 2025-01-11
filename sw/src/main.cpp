#include <WiFi.h>
#include "watchdog.hpp"
#include "wifi_creds.hpp"

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("booting");

  watchdog_init();
  Serial.println("watchdog enabled");

  // TODO
}

void loop()
{
  Serial.println("i'm alive!");
  watchdog_reset();
  delay(1000);
  // TODO
}
