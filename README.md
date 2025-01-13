# ESP32 RTSP cam

a brain-dead simple WiFi camera using `esp32-cam` module.
video stream is sent via RTSP stream to a client.

board is a WiFi client, to an existing network.
WiFi PSK is to be provided in the sources.

## configuring WiFi access

WPA2/3 PSK is supported.
```
cd sw/src/
cp wifi_creds.hpp.template wifi_creds.hpp
chmod 600 wifi_creds.hpp
vi wifi_creds.hpp
```
fill your in SSID and password for the network.

note that `wifi_creds.hpp` is marked as ignored in `git`, so that it does not get accudentally committed.


## flahsing

connect via serial adapter.
short `IO0` to `GND` with a jumper and press reset button.

flash with:
```
cd sw
./make
```

remove jumper and press reset button again.
boot sequence should show up on a serial console.

example boot sequence:
```
##################################
booting
disabling brownout detection
watchdog enabled
initializing camera
initializing WiFi
connecting to WiFi: my-wifi-network:
connected! local IP: 192.168.1.42
starting RTSP server on rtsp://192.168.1.42:8554/mjpeg/2
initialization sequence completed!
----------------------------------
```


## client

use:
```
./receive_stream <IP_of_your_esp32-cam>
```
to view the live stream with `vlc`.
