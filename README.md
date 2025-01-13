# ESP32 RTSP cam

a brain-dead simple WiFi camera using `esp32-cam` module.
video stream is sent via RTSP stream to a client.

board is a WiFi client, to an existing network.
WiFi PSK is to be provided in the sources.

## client

use:
```
./receive_stream <IP_of_your_esp32-cam>
```
to view the live stream with `vlc`.
