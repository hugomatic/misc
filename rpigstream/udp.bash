# https://qengineering.eu/install-gstreamer-1.18-on-raspberry-pi-4.html

gst-launch-1.0 -v udpsrc port=5200 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert ! autovideosink
