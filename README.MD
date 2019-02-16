esp32 soil module
===========================
![](1.png)

### Install the following dependency library files:
- [Adafruit-DHT](https://github.com/adafruit/DHT-sensor-library)
- [ESP-DASH](https://github.com/lewisxhe/ESP-DASH)
- [ArduinoJson 6.8](https://github.com/bblanchon/ArduinoJson)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
- [Button2](https://github.com/lewisxhe/Button2)

### Network Configuration
- [EspTouch](https://www.espressif.com/en/products/software/esp-touch/resources)

### Upload webserver resource
- Use Arduino ESP32 Sketch data Upload files,if you not install,[download ESP32FS-vX.zip](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases),Extract to <C:\Users\Your User Name\Documents\Arduino\tools>,Open Ardunio IDE,  Tools -> ESP32 Sketch data Upload -> Upload

### Web page view
- Enter the IP address in your browser, or view the data at http://soil.local


### Peripheral description
- BOOT Button long press enter smartconfig mode,LED Fast blink
- LED Slow blink is lost connect 