#include <OV2640.h>
#include "watchdog.hpp"
#include "wifi.hpp"
#include "wifi_creds.hpp"
#include "RTSP_server.hpp"


std::unique_ptr<RTSP_server> rtsp_server;
OV2640 camera;

void setup()
{
  try
  {
    Serial.begin(115200);
    Serial.println();
    Serial.println("##################################");
    Serial.println("booting");

    watchdog_init();
    Serial.println("watchdog enabled");

    Serial.println("initializing camera");
    if( camera.init(esp32cam_aithinker_config) != ESP_OK )
      throw std::runtime_error{"setup(): failed to initialize camera"};

    wifi_init();
    while( not wifi_connect(wifi_ssid, wifi_pass) )
    {
      Serial.println("failed to connect to WiFi");
      wifi_scan();
    }

    Serial.printf("starting RTSP server on %s:%d\r\n", WiFi.localIP().toString().c_str(), RTSP_server::port());
    rtsp_server.reset(new RTSP_server{camera});

    Serial.println("initialization sequence completed!");
    Serial.println("----------------------------------");
  }
  catch(std::exception const& ex)
  {
    Serial.printf("setup(): exception caught: %s\r\n", ex.what());
    throw;
  }
}

void loop()
{
  try
  {
    watchdog_reset();
    rtsp_server->update();
    delay( 1000 / RTSP_server::fps() );
  }
  catch(std::exception const& ex)
  {
    Serial.printf("loop(): exception caught: %s\r\n", ex.what());
    throw;
  }
}
