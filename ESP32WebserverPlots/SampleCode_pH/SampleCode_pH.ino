///*
// # This sample code is used to test the pH meter V1.0.
// # Editor : YouYou
// # Ver    : 1.0
// # Product: analog pH meter
// # SKU    : SEN0161
//*/
//#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
//#define Offset 3.79            //deviation compensate
//#define LED 13
//#define samplingInterval 20
//#define printInterval 800
//#define ArrayLenth  40    //times of collection
//int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
//int pHArrayIndex=0;
//void setup(void)
//{
//  pinMode(LED,OUTPUT);
//  Serial.begin(115200);
//  Serial.println("pH meter experiment!");    //Test the serial monitor
//  pinMode(SensorPin, INPUT);
//}
//void loop(void)
//{
//  static unsigned long samplingTime = millis();
//  static unsigned long printTime = millis();
//  static float pHValue,voltage;
//  if(millis()-samplingTime > samplingInterval)
//  {
//      pHArray[pHArrayIndex++]=analogRead(SensorPin);
//      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
//      voltage = avergearray(pHArray, ArrayLenth)*5.0/4096;
//      pHValue = 3.5*voltage+Offset;
//      samplingTime=millis();
//  }
//  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
//  {
//    Serial.print("Voltage:");
//        Serial.print(voltage,2);
//        Serial.print("    pH value: ");
//    Serial.println(pHValue,2);
//        digitalWrite(LED,digitalRead(LED)^1);
//        printTime=millis();
//  }
//}
//double avergearray(int* arr, int number){
//  int i;
//  int max,min;
//  double avg;
//  long amount=0;
//  if(number<=0){
//    Serial.println("Error number for the array to avraging!/n");
//    return 0;
//  }
//  if(number<5){   //less than 5, calculated directly statistics
//    for(i=0;i<number;i++){
//      amount+=arr[i];
//    }
//    avg = amount/number;
//    return avg;
//  }else{
//    if(arr[0]<arr[1]){
//      min = arr[0];max=arr[1];
//    }
//    else{
//      min=arr[1];max=arr[0];
//    }
//    for(i=2;i<number;i++){
//      if(arr[i]<min){
//        amount+=min;        //arr<min
//        min=arr[i];
//      }else {
//        if(arr[i]>max){
//          amount+=max;    //arr>max
//          max=arr[i];
//        }else{
//          amount+=arr[i]; //min<=arr<=max
//        }
//      }//if
//    }//for
//    avg = (double)amount/(number-2);
//  }//if
//  return avg;
//}


//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the pH circuit.
//This code was written in the Arduino 2.0 IDE
//An Arduino UNO was used to test this code.
//This code was last tested 10/2022


//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the pH circuit.
//This code was written in the Arduino 2.0 IDE
//An Arduino UNO was used to test this code.
//This code was last tested 10/2022


#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float pH;                                             //used to hold a floating point number that is the pH



void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  myserial.begin(9600);                               //set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


void loop() {                                         //here we go...

  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }


  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    Serial.println(sensorstring);                     //send that string to the PC's serial monitor
    
    /*                                                //uncomment this section to see how to convert the pH reading from a string to a float 
    if (isdigit(sensorstring[0])) {                   //if the first character in the string is a digit
      pH = sensorstring.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
      if (pH >= 7.0) {                                //if the pH is greater than or equal to 7.0
        Serial.println("high");                       //print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
      }
      if (pH <= 6.999) {                              //if the pH is less than or equal to 6.999
        Serial.println("low");                        //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
      }
    }
    */
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
}
