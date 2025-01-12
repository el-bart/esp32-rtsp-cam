#pragma once
#include <WiFi.h>
#include "watchdog.hpp"
#include "wifi_creds.hpp"


namespace detail
{
inline char const* enc_type(int i)
{
  switch( WiFi.encryptionType(i) )
  {
    case WIFI_AUTH_OPEN:            return "open network";
    case WIFI_AUTH_MAX:             return "MAX";
    case WIFI_AUTH_WAPI_PSK:        return "WAPI PSK";
    case WIFI_AUTH_WEP:             return "WEP";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2 enterprise";
    case WIFI_AUTH_WPA2_PSK:        return "WPA2 PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3 PSK";
    case WIFI_AUTH_WPA3_ENT_192:    return "WPA3 enterprise 192";
    case WIFI_AUTH_WPA3_PSK:        return "WPA3 PSK";
    case WIFI_AUTH_WPA_PSK:         return "WPA PSK";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2 PSK";
  }
  return "unknown";
}


inline void wifi_disconnect()
{
  Serial.println("disconnecting from WiFi");
  WiFi.mode(WIFI_STA);  // client mode
  WiFi.disconnect();
  watchdog_reset();
}


inline bool wifi_wait_for_connection()
{
  for(auto i=0; i<30/*s*/; ++i)
  {
    for(auto j=0; j<10; ++j)
      if( WiFi.status() == WL_CONNECTED )
        return true;
      else
        delay(100);
    Serial.print(".");
    watchdog_reset();
  }
  return false;
}
} // namespace detail



inline void wifi_init()
{
  Serial.println("initializing WiFi");
  WiFi.mode(WIFI_STA);  // client mode
  WiFi.disconnect();
}


inline void wifi_scan()
{
  watchdog_reset();
  Serial.println("scanning for nearby WiFi networks");
  auto const n = WiFi.scanNetworks();
  if( n < 0)
  {
    Serial.println("WiFi scanning failed");
    return;
  }
  Serial.printf("WiFi scanning completed - got %d instances:\r\n", n);
  watchdog_reset();

  for(auto i=0; i<n; ++i)
  {
    auto const& ssid = WiFi.SSID(i);
    auto const& enc = detail::enc_type(i);
    auto const  rssi = WiFi.RSSI(i);
    Serial.printf("%30s | %4d | %s\r\n", ssid.c_str(), rssi, enc);
  }
  Serial.println();
  watchdog_reset();
}


inline bool wifi_connect(char const* ssid, char const* pass)
{
  Serial.printf("connecting to WiFi: %s: ", ssid);
  WiFi.begin(ssid, pass);

  if( not detail::wifi_wait_for_connection() )
  {
    Serial.println("connection FAILED");
    detail::wifi_disconnect();
    return false;
  }
  Serial.println();
  Serial.printf("connected! local IP: %s\r\n", WiFi.localIP().toString().c_str());
  return true;
}
