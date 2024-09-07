# Water Quality Monitoring Device
![Banner Github Water Quality Monitoring](https://github.com/user-attachments/assets/98fe4ae8-0078-4f49-86a2-befdf3e57dbe)
## Introduction
This project is **a Water Quality Monitoring System** supported by both a web interface and an OLED display. The device utilizes the SPIFFS (SPI Flash File System) feature of the ESP32 microcontroller to deploy a native web application (HTML, CSS, JavaScript) over a wide-area local network. The system directly links the web interface to real-time data received by the ESP32, providing users with seamless access to water quality metrics. 
 
### Concerns
The Drinking Fountain is essential for students at the Faculty of Engineering, Universitas Indonesia, providing water and reducing plastic use. However, concerns about water quality have led 60% of students to still purchase bottled water. A survey revealed that 90% of respondents believe the water quality in certain buildings is poor, particularly due to its taste. Addressing these concerns is crucial to encouraging students to trust and use the water fountains.

### Usage
1. The device measures all test parameters on drinking water samples.
2. It displays results in a human-readable format via an OLED screen.
3. The device sends water test data to a designed webpage.
4. Its microcontroller deploys a server-based webpage accessible to anyone on the UI Wi-Fi, providing water fountain quality information.

## PCB Design
PCB made using Altium and printed using PCB printing machine.
\
<img src="https://github.com/user-attachments/assets/054bda38-7e51-4148-addd-fec819a8c62f" width="500" height="400">

> Here are the made PCB picture
> \
> <img src="https://github.com/user-attachments/assets/55516c79-7192-41cb-ae5c-0f6a9279875f" width="500" height="400">
## Hardware Design
![HardwareDesign-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/5ff89c2c-1abf-486b-88cd-431fd34b3db4)

## Requirements Software
For installation, within Arduino IDE, the following libraries needs to be installed under Sketch -> Include Library -> Manage Libraries:
* ArduinoJson by Benoit Blanchon
* WebSockets by Markus Sattler
* WiFi 

The following to libraries need to be downloaded, unpacked and copied to the "Arduino" folder (Required for ESPAsyncWebServer)
- https://github.com/me-no-dev/ESPAsyncWebServerWater
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
- Put the asset folder into tools folder in Arduino Folder (usually in C:/Users/Documents/Arduino)
- After putting asset folder into tools folder, restart the Arduino IDE
- Check if in the dropdown section of Tools there is ESP32 Sketch Data Upload
- [X] if there is, then you are set to go

## Code Pre-configuration
In the ino. files in this section,
```
// SSID and password of Wifi connection:
const char* ssid = "[Your Wifi Name]";
const char* password = "[Your Wifi Password]";
```
ssid and password need to be changed according to the router/wifi that is used.

> Before uploading the code, **please upload data sketch first using ESP32 Sketch Data Upload button on tools dropdown section**

## Hardware Requirement
Before making the actual device, there are some items that you need to have:
- Analog pH Sensor kit by DFRobot
- OleD White 0.96 inch I2C IIC 128x64 display module lcd
- Oled Display 0.91 inch 128x32 LCD Screen
- Micro DC step down 4v-24v to 3.3v 5v 3A Buck super mini
- ESP32
- KY-023 Analog Dual Axis XY Joystick
- Calibration Buffer Powder/liquid
- PH Meter Sensor board ph4502C for Arduino
- Robotdyn e 3.3v/5v logic 4A/16A 600V for Arduino - 4A
- PTC Thermostat
- Heatsink
- Hi-Link HLK-5M05
- Modul 16bit I2C ADS1115
- Jumper set
- NTC Thermistor
- TDS Sensor
- Turbidity Sensor
- Servo

> Before using all the sensor, please configurate and calibrate first.
## Created by
Bryan Indarto, Aldy Raja, Theonaldo, dan Reza Febrian
