# ESP32 RTSP cam

a brain-dead simple WiFi camera using `esp32-cam` module.
video stream is sent via RTSP stream to a client.

board is a WiFi client, to an existing network.
WiFi PSK is to be provided in the sources.

## client

use:
```
vlc \
  -vvv \
  --network-caching=0 --file-caching=0 --rtsp-caching=0 --live-caching=0 \
  --no-drop-late-frames --no-skip-frames \
  --rtsp-tcp \
  --rtsp-timeout=20000 --rtsp-session-timeout=20000 --ipv4-timeout=20000 \
  rtsp://<esp32-cam-IP>:8554/mjpeg/2
```
to view the live stream.
