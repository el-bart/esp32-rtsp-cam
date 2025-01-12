# ESP32 RTSP cam

a brain-dead simple WiFi camera using `esp32-cam` module.
video stream is sent via RTSP stream to a client.

board is a WiFi client, to an existing network.
WiFi PSK is to be provided in the sources.

## client

use:
```
vlc -vvv --rtsp-tcp --rtsp-timeout=-1 --rtsp-session-timeout=-1 --ipv4-timeout=-1 rtsp://<esp32-cam-IP>:8554/mjpeg/2
```
to view the live stream.
