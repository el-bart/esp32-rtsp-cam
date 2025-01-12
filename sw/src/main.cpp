#include <OV2640.h>
#include "watchdog.hpp"
#include "wifi.hpp"
#include "wifi_creds.hpp"
#include "RTSP_server.hpp"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


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

#if 1
    Serial.println("disabling brownout detection");
    // this is a hack for a broken ESP32-cam design, where voltage is too low
    // and decoupling capacitors are not enough. ugly, but seems to be working
    // for now at least...
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
#endif


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

    // NOTE: trailing slash in RTSP is crucial, as otherwise library crashes...
    Serial.printf("starting RTSP server on rtsp://%s:%d/mjpeg/2\r\n", WiFi.localIP().toString().c_str(), RTSP_server::port());
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
  watchdog_reset();
  try
  {
    rtsp_server->update();
  }
  catch(std::exception const& ex)
  {
    Serial.printf("loop(): exception caught: %s\r\n", ex.what());
    throw;
  }
}
