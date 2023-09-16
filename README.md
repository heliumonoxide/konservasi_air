# Project Konservasi Air
## Description
This project act as an conserver of blablabalbalbalablaba


## Requirements
For installation, the following libraries needs to be installed under Sketch -> Include Library -> Manage Libraries:
* ArduinoJson by Benoit Blanchon
* WebSockets by Markus Sattler
* WiFi 

The following to libraries need to be downloaded, unpacked and copied to the "Arduino" folder (Required for ESPAsyncWebServer)
- https://github.com/me-no-dev/ESPAsyncWebServer
- https://github.com/me-no-dev/AsyncTCP

Required to make SPIFFS.h work:
https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/

### How to Install Packages Manually
- Click on the link provided in each package
- Download zip file of the packages through the 'code' dropdown
- Unzip/extract the downloaded file
- Put the packages folder in to libraries folder in Arduino Folder (usually in C:/Users/Documents/Arduino)

### How to Install Tools Manually
- Click on the link provided in each package
- Download zip file of the tools in the asset section of the page
- Unzip/extract the downloaded file
- Put the asset folder in to tools folder in Arduino Folder (usually in C:/Users/Documents/Arduino)

## Code Pre-configuration
In the ino. files in this section,
```
// SSID and password of Wifi connection:
const char* ssid = "Arduino_ssid";
const char* password = "password";
```
ssid and password need to be changed according to the router/wifi that is used.

## Created by
Bryan Indarto and Aldy Raja
