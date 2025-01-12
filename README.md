# ESP32 RTSP cam

a brain-dead simple WiFi camera using `esp32-cam` module.
video stream is sent via RTSP stream to a client.

board is a WiFi client, to an existing network.
WiFi PSK is to be provided in the sources.

## client

use:
```
vlc -vvv --rtsp-timeout=30 --rtsp-tcp rtsp://<esp32-cam-IP>:8554/mjpeg/2
```
to view the live stream.
