// ---------------------------------------------------------------------------------------
// For installation, the following library needs to be installed under Sketch -> Include Library -> Manage Libraries:
// * ArduinoJson by Benoit Blanchon
//
// The following to libraries need to be downloaded, unpacked and copied to the "Arduino\libraries" folder
// (Required for ESPAsyncWebServer)
// https://github.com/me-no-dev/ESPAsyncWebServer
// https://github.com/me-no-dev/AsyncTCP
//
// Required to make SPIFFS.h work:
// https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
// see as well: https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/
// ---------------------------------------------------------------------------------------

/***************************************************
 Created by Kelompok Despro:
 - Bryan Indarto Giovanni Firjatulloh
 - Aldy Raja Sipahutar 
 ****************************************************/

// Library for Webserver
#include <WiFi.h>                                     // needed to connect to WiFi
#include <ESPAsyncWebServer.h>                        // needed to create a simple webserver (make sure tools -> board is set to ESP32, otherwise you will get a "WebServer.h: No such file or directory" error)
#include <WebSocketsServer.h>                         // needed for instant communication between client and server through Websockets
#include <ArduinoJson.h>                              // needed for JSON encapsulation (send multiple variables with one string)
#include <SPIFFS.h>
#include <AsyncElegantOTA.h>
// Hardware Initialization
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>

#define TEMP_PIN 23 // Yellow cable on temp-liquid sensor pin

#define PIN_SERVO1 25
#define PUMP_PIN 12
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_ADS1115 ads;

int16_t adc0, adc1, adc2;
float volts0, volts1, volts2;

// SSID and password of Wifi connection:
const char* ssid = "STAR 1";
const char* password = "passwordlama";

const int ARRAY_LENGTH = 5;  // final_quality, temperature_value, ph_value, ntu_value, tds_value (Currently only for 5 sensors on 1 Niagara place)
int sensors_val[ARRAY_LENGTH];

// We want to periodically send values to the clients, so we need to define an "interval" and remember the last time we sent data to the client (with "previousMillis")
int interval = 25000;                                  // send data to the client every 2000ms -> 2s
unsigned long previousMillis = 0;                     // we use the "millis()" command for time reference and this will output an unsigned long

// Initialization of webserver and websocket
AsyncWebServer server(80);                            // the server uses port 80 (standard port for websites
WebSocketsServer webSocket = WebSocketsServer(81);    // the websocket uses port 81 (standard port for websockets

// Initialization of the hardware
unsigned long startTime;

// Temperature Sensor Parameter
float tempValue;
OneWire oneWire(TEMP_PIN); // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature sensor 

// TDS Sensor Parameter
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;
unsigned long startTimeBuffer = 0;
unsigned long currentTimeBuffer;
unsigned long startTimeTDS = 0;

float result;

// NTU
float ntu = 0;

// pH Sensor Parameter
#define Offset 0.00    
#define ArrayLength  40    //times of collection
static unsigned long start_pHSamplingTime = 0;
static unsigned long start_pHPrintTime = 0;
int pHArray[ArrayLength];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
float pHValue,pHvoltage;
DFRobot_PH ph;

// Flags
bool exec_measurement = false;
int counter = 0;

// Servo
Servo servo1;
const int sudut_trigger_open = 135;
const int sudut_close = 0;
const int sudut_trigger_close = 0;
const int sudut_open = 135;
int pos = 0;    // variable to store the servo position

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);

void setup() {
  Serial.begin(115200);                               // init serial port for debugging
  ads.begin();

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS could not initialize");
  }
  
// ================================================== ganti wifi ==================================================
  WiFi.begin(ssid, password);                         // start WiFi interface
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));     // print SSID to the serial interface for debugging
 
  while (WiFi.status() != WL_CONNECTED) {             // wait until WiFi is connected
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP());                     // show IP address that the ESP32 has received from router
// ================================================================================================================
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {    // define here what the webserver needs to do
    request->send(SPIFFS, "/webpage.html", "text/html");           
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "File not found");
  });

  server.serveStatic("/", SPIFFS, "/");
  
  webSocket.begin();                                  // start websocket
  webSocket.onEvent(webSocketEvent);                  // define a callback function -> what does the ESP32 need to do when an event from the websocket is received? -> run function "webSocketEvent()"

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  
  server.begin();                                     // start server -> best practise is to start the server after the websocket

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawRect(6, 6, display.width()-6*2, display.height()-6*2, WHITE);
  display.fillRect(6, 6, display.width()-6*2, 14, WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(10, 10);
  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.print(F("NIAGARA MONITORING"));
  display.setCursor(10, 24);
  display.setTextColor(WHITE);        // Draw white text
  display.print(F("IP: ")); display.print(WiFi.localIP());
  display.display();

  // Device Wiring Part
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  servo1.attach(PIN_SERVO1);

  sensors.begin();
  startTime = millis();
  startTimeBuffer = millis();
  startTimeTDS = millis();
  start_pHSamplingTime = millis();
  start_pHPrintTime = millis();
  servo1.write(0);
  delay(1000);
}

// sendJsonArray used for final_quality, temperature_value, ph_value, ntu_value, tds_value
void loop()
{
  add_buffer(); // Data Additional Sensor TDS
  if(counter == 0 || (millis() - startTime >= 30000) || exec_measurement == true){
    if (exec_measurement == false){
  for (pos = sudut_trigger_close; pos <= sudut_trigger_open; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
  delay(10000);
  for (pos = sudut_trigger_open; pos >= sudut_trigger_close; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo1.write(pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
      exec_measurement = true;
    }

    delay(10);
    
    if(exec_measurement == true){
      Serial.println("start measurement");
      
      // Sensor Suhu
      sensors.requestTemperatures();
      tempValue = sensors.getTempCByIndex(0);
      Serial.println("=====================================");
      Serial.print("Temp Celsius: ");
      Serial.print(tempValue);
      Serial.println(" C \n");
  
      // Sensor Turbidity
      int sensorValue = ads.readADC_SingleEnded(1);// read the input on analog pin ads A1:
      Serial.print("Analog Value: ");
      Serial.println(sensorValue);
      float voltage = ads.computeVolts(sensorValue); // Convert the analog reading (which goes from 0 - 4096) to a voltage (0 - 3.3V):
      ntu = (-1120.4*(voltage*voltage)) + (5742.3*voltage) - 4352.9;
      Serial.print("Turbidity Voltage: ");
      Serial.println(voltage); // print out the value you read:
      Serial.print("Turbidity NTU: ");
      Serial.println(ntu);
      Serial.print("\n");
      
      // Sensor TDS
      print_tds();
  
      // Sensor pH
      pH_buffer;

      Serial.println("=====================================");
      
      if(counter == 4){
        exec_measurement = false;
        startTime = millis();
        digitalWrite(PUMP_PIN, HIGH);
        delay(10000);
        digitalWrite(PUMP_PIN, LOW);
        delay(500)
        counter = 0;
      }
      counter += 1;
    }
  }
  webSocket.loop();                                   // Update function for the webSockets 
  unsigned long now = millis();                       // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if ((unsigned long)(now - previousMillis) > interval) { // check if "interval" ms has passed since last time the clients were updated
    previousMillis = now;                             // reset previousMillis

//    result = random(10);
    result = 8.0;
    sensors_val[0] = result;
    sensors_val[1] = tempValue;
    sensors_val[2] = pHValue;
    sensors_val[3] = ntu;
    sensors_val[4] = tdsValue;

    sendJsonArray("dashboard_update", sensors_val);

  }
  drawSensVal();    // Draw sensor value
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
  switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
      Serial.println("Client " + String(num) + " connected");
      
      // send variables to newly connected web client -> as optimization step one could send it just to the new client "num", but for simplicity I left that out here
      // sendJson("random_intensity", String(random_intensity));
      sendJsonArray("dashboard_update", sensors_val);

      break;
    case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
      // try to decipher the JSON string received
      StaticJsonDocument<200> doc;                    // create JSON container 
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
         Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else {
        // JSON string was received correctly, so information can be retrieved:
        const char* l_type = doc["type"];
        const int l_value = doc["value"];
        Serial.println("Type: " + String(l_type));
        Serial.println("Value: " + String(l_value));
      }
      Serial.println("");
      break;
  }
}

// Simple function to send information to the web clients
void sendJsonArray(String l_type, int l_array_values[]) {
    String jsonString = "";                           // create a JSON string for sending data to the client
    const size_t CAPACITY = JSON_ARRAY_SIZE(ARRAY_LENGTH) + 100;
    StaticJsonDocument<CAPACITY> doc;                      // create JSON container
    
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["type"] = l_type;                          // write data into the JSON object
    JsonArray value = object.createNestedArray("value");
    for(int i=0; i<ARRAY_LENGTH; i++) {
      value.add(l_array_values[i]);
    }
    serializeJson(doc, jsonString);                   // convert JSON object to string
    webSocket.broadcastTXT(jsonString);               // send JSON string to all clients
}


void add_buffer(void){
//  Serial.println("Adding Buffer for TDS");
  currentTimeBuffer = millis();
  if(currentTimeBuffer - startTimeBuffer > 40UL)     //every 40 milliseconds,read the analog value from the ADC
  {
   analogBuffer[analogBufferIndex] = ads.readADC_SingleEnded(0);    //read the analog value and store into the buffer
   analogBufferIndex++;
   if(analogBufferIndex == SCOUNT){
    analogBufferIndex = 0; 
   }
   startTimeBuffer = currentTimeBuffer;
  }
}

void print_tds(void){
  unsigned long currentTimeTDS = millis();
  for(copyIndex=0;copyIndex<SCOUNT;copyIndex++){
    analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
  }
  
  averageVoltage = ads.computeVolts(getMedianNum(analogBufferTemp,SCOUNT)); // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float compensationVolt=averageVoltage/compensationCoefficient;  //temperature compensation
  tdsValue=(133.42*compensationVolt*compensationVolt*compensationVolt - 255.86*compensationVolt*compensationVolt + 857.39*compensationVolt)*0.5; //convert voltage value to tds value
  //Serial.print("voltage:");
  //Serial.print(averageVoltage,2);
  //Serial.print("V   ");
  Serial.print("TDS Value:");
  Serial.print(tdsValue);
  Serial.println("ppm");
  startTimeTDS = currentTimeTDS;
}


void pH_buffer(void) {
 while(true)
  {
    pHArray[pHArrayIndex++]=ads.readADC_SingleEnded(2);  // pH
    pHvoltage = ads.computeVolts(averagearray(pHArray, ArrayLength));
    pHValue = 3.5*pHvoltage+Offset;
    if(pHArrayIndex==ArrayLength){
      pHArrayIndex=0;
      break;
    }
  }
}

int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

double averagearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

void drawSensVal(void) {
  display.clearDisplay();
  display.drawRect(6, 6, display.width()-6*2, display.height()-6*2, WHITE);
  display.fillRect(6, 6, display.width()-6*2, 14, WHITE);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(10, 10);
  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.print(F("NIAGARA MONITORING"));

  display.setCursor(10, 24);
  display.setTextColor(WHITE);        // Draw white text
  display.print(F("IP: ")); display.print(WiFi.localIP());

  display.setTextSize(2);             // Draw 2X-scale text
  display.setCursor(10, 37);
  display.print(F("Stat:")); display.println("GOOD");

  display.display();
  delay(2000);


  display.clearDisplay();
  display.drawRect(6, 6, display.width()-6*2, display.height()-6*2, WHITE);
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.println(F("pH: "));
  display.setTextSize(2);
  display.print("  "); display.println(pHValue);

  display.setTextSize(1);
  display.setCursor(10, 33);
  display.println(F("Turb: "));
  display.setTextSize(2);
  display.print("  "); display.println(ntu);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.drawRect(6, 6, display.width()-6*2, display.height()-6*2, WHITE);
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.println(F("Temp: "));

  display.setTextSize(2);
  display.print("  "); display.println(tempValue);

  display.setTextSize(1);
  display.setCursor(10, 33);
  display.println(F("TDS: "));
  display.setTextSize(2);
  display.print("  "); display.println(tdsValue);
  display.display();
  delay(2000);
}
