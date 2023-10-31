/***************************************************
 Created by Kelompok Despro:
 - Bryan Indarto Giovanni Firjatulloh
 ****************************************************/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>
#include "DFRobot_PH.h"
#include <EEPROM.h>
 
#define TEMP_PIN 32 // Yellow cable on temp-liquid sensor pin
#define TURB_PIN 34
#define TDS_PIN 35
#define PH_PIN 33
#define PIN_SERVO1 25
#define PIN_SERVO2 26

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

// pH Sensor Parameter
#define Offset 0.00    
#define ArrayLength  40    //times of collection
static unsigned long start_pHSamplingTime = 0;
static unsigned long start_pHPrintTime = 0;
int pHArray[ArrayLength];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
static float pHValue,pHvoltage;
float temperature = 25;
DFRobot_PH ph;

// Flags
bool exec_measurement = false;
int counter = 0;

// Servo
Servo servo1;  
Servo servo2;
const int sudut_trigger_open = 180;
const int sudut_close = 0;
const int sudut_trigger_close = 0;
const int sudut_open = 180;

void setup()
{
  Serial.begin(115200);
  Serial.println("setup begin");
  pinMode(TURB_PIN, INPUT);
  pinMode(TDS_PIN,INPUT);
  servo1.attach(PIN_SERVO1); 
//    servo2.attach(PIN_SERVO2); 

  sensors.begin();
  startTime = millis();
  startTimeBuffer = millis();
  startTimeTDS = millis();
  start_pHSamplingTime = millis();
  start_pHPrintTime = millis();
  servo1.write(0);
  Serial.println("setup end");
  delay(2000);
}

void loop()
{
  add_buffer(); // Data Additional Sensor TDS
  if(counter == 0 || (millis() - startTime >= 30000) || exec_measurement == true){
    if (exec_measurement == false){
      servo1.write(sudut_trigger_open);
//      servo2.write(sudut_close);
      delay(10000);
      servo1.write(sudut_trigger_close);
      exec_measurement = true;
    }

    delay(10);
    
    if (exec_measurement == true){
      Serial.println("start measurement");
      
      // Sensor Suhu
      sensors.requestTemperatures();
      tempValue = sensors.getTempCByIndex(0);
      Serial.println("=====================================");
      Serial.print("Temp Celsius: ");
      Serial.print(tempValue);
      Serial.println(" C \n");
  
      // Sensor Turbidity
      int sensorValue = analogRead(TURB_PIN);// read the input on analog pin 0:
      Serial.print("Analog Value: ");
      Serial.println(sensorValue);
      float voltage = (sensorValue / 4095.0) * 5.0; // Convert the analog reading (which goes from 0 - 4096) to a voltage (0 - 4.5V):
      float ntu = -1120.4*voltage*voltage + 5742.3*voltage - 4352.9;
      Serial.print("Turbidity Voltage: ");
      Serial.println(voltage); // print out the value you read:
      Serial.print("Turbidity NTU: ");
      Serial.println(ntu);
      Serial.print("\n");
      
      // Sensor TDS
      print_tds();
  
      // Sensor pH
//      pH_buffer; // Old version
      print_pH();

      Serial.println("=====================================");
      
      if(counter == 4){
        exec_measurement = false;
        startTime = millis();
        servo1.write(sudut_trigger_close);
//        servo2.write(sudut_open);
        counter = 0;
      }
      counter += 1;
    }
  }
}

void add_buffer(void){
//  Serial.println("Adding Buffer for TDS");
  currentTimeBuffer = millis();
  if(currentTimeBuffer - startTimeBuffer > 40UL)     //every 40 milliseconds,read the analog value from the ADC
  {
   analogBuffer[analogBufferIndex] = analogRead(TDS_PIN);    //read the analog value and store into the buffer
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
  
  averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 4096.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
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

void print_pH(void) {
//  Serial.print(" pHVoltage: "); // Old version
//  Serial.print(pHvoltage, 2);
//  Serial.print(" pH value: ");
//  Serial.println(pHValue, 2);

  //temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
  voltage = analogRead(PH_PIN)/4096.0*5000;  // read the voltage
  phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
  Serial.print("temperature:");
  Serial.print(temperature,1);
  Serial.print("^C  pH:");
  Serial.println(phValue,2);
  
  ph.calibration(voltage,temperature);           // calibration process by Serail CMD
  }
}

void pH_buffer(void) {
 while(true)
  {
    pHArray[pHArrayIndex++]=analogRead(PH_PIN);
    if(pHArrayIndex==ArrayLength){
      pHArrayIndex=0;
      pHvoltage = averagearray(pHArray, ArrayLength)*5.0/4096;
      pHValue = 3.5*pHvoltage+Offset;
      break;
    }
    pHvoltage = averagearray(pHArray, ArrayLength)*5.0/4096;
    pHValue = 3.5*pHvoltage+Offset;
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
