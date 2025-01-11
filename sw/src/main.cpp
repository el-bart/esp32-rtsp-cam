#include "watchdog.hpp"
#include "wifi.hpp"
#include "wifi_creds.hpp"

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("-----------");
  Serial.println("booting");

  watchdog_init();
  Serial.println("watchdog enabled");

  wifi_init();
  do
  {
    wifi_scan();
  }
  while(not wifi_connect(wifi_ssid, wifi_pass));

  // TODO
}

void loop()
{
  Serial.println("i'm alive!");
  watchdog_reset();
  delay(1000);
  // TODO
}
