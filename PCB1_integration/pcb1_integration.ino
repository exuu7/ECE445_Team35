//Citation for temperature sensor: https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/basic_projects/ar_thermistor.html
//Citation for proximity sensor : https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-arduino/
/*
   Citation for Weight Sensor----------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/
//Citation: https://kursatsayhan.medium.com/using-bluetooth-low-energy-ble-with-flutter-3c70469af814
// Citation: https://forum.arduino.cc/t/how-do-you-convert-a-float-to-string-solved/237090/8

#include <stdlib.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//--------------------------------------------------------------Bluetooth------------------------------------------------------------------
BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
//--------------------------------------------------------------Microcontroller Pins--------------------------------------------------
const int tempPin = 4; // ESP32 Temperature Sensor Analog Pin 
const int trigPin =12 ; // pin that the trigger pin - Prox PCB
const int echoPin = 11; // pin that the echo pin - Prox PCB
const int HX711_dout = 5; //mcu > HX711 dout pin - Weight PCB
const int HX711_sck = 6; //mcu > HX711 sck pin - Weight PCB
int redLED = 15; 
int greenLED = 14;
int blueLED = 13;
// Motor A
int motor1Pin1 = 41; 
int motor1Pin2 = 40; 
int enable1Pin = 42; 
//----------------------------------------------------------------Motor Constants--------------------------------------------------------
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

//-------------------------------------------------------------Temperature Sensor Constants----------------------------------------------
const int resistorValue = 10000; // 10k resistor used with the thermistor
const int nomTemp = 25; // temperature that the thermistor was tested under
const int nomRes = 10000; // resistance of the thermistor standard temp
const float sthh_const = 3950.0; // constant used in steinhart-hart equation
const float kel_const = 273.15; // kelvin constant
const int thresholdTemp = 85; // threshold temperature

//-----------------------------------------------------------Proximity Constants----------------------------------------------------------
// threshold for how far away the window should be to confirm whether the window is present in front
const int thresholdProx = 3; 
#define SOUND_SPEED 0.034 // velocity of sound in cm/us
#define CM_TO_INCH 0.393701 // allows for conversion for centimeters to inches
long objDectTime;
float distCm;
float distIn;
int windowLowering; // flag to indicate the status of the window
// windowLowering = 1 -> window is lowering
// windowLowering = 0 -> window is up
int confirmWindow = 0; 
// 1 means that the window lowered when told to
// 0 means that the window did not lower when told to

//------------------------------------------------------Weight Constants---------------------------------------------------------------
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
unsigned long t = 0;

// sets up the esp32 with the pins and serial monitor
void setup() { 
  Serial.begin(115200); // sets the correct serial baud rate
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output - emits the ultrasound
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input - recieves the reflected wave
  pinMode(greenLED, OUTPUT); // led for weight
  pinMode(redLED, OUTPUT); // led for temp
  pinMode(blueLED, OUTPUT); // led for temp
  delay(10);
  Serial.println();
  Serial.println("Starting...");

     BLEDevice::init("Seri");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                        );


  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = -101.87; // uncomment this if you want to set the calibration value in the sketch
  // calibrationValue = -100.66; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}

void loop() {
  int sensorValue = analogRead(tempPin); // stores the raw resistance value of the thermistor
  // Clears the trigPin to ensure a clear signal is sent when trigpin is high (prox)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Calculation: The thermistor and 10k ohm form a voltage divider circuit. 
  // The first part of the equation is finding the volatge across the thermistor. Vin * (thermistor res/(thermistor res + 10k)).
  // The 4095 is to convert to 12 bits. This is to return the voltage value
  float volt = (sensorValue * 3.3)/ 4095; // converts the resistance to a voltage value
  float resistance = (volt * resistorValue)/ (3.3 - volt); //this finds the updated resistance value using the above voltage 
  // converts resistance to temperature using the Steinhart-Hart equation
  float kelvin = 1 / (((log(resistance / nomRes)) / sthh_const) + (1 / (nomTemp + kel_const)));
  float Cels = kelvin - kel_const; // convert Kelvin to Celsius 
  float Fah = 1.8 * Cels + 32.0; // convert temp to Fahrenheit
  // this function reads the pulse that has been sent to the echopin and returns the time taken to travel to and from the object
  objDectTime = pulseIn(echoPin, HIGH);
  distCm = objDectTime * SOUND_SPEED/2;   // converts the time to distance
  distIn = distCm * CM_TO_INCH; // Convert to distance from cm to inches
  int confirmWindow = 0;

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);
  Serial.print("Testing DC Motor...");

  // weight sensor code
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity
  int childInCar = 0;
  // flag to indicate that the child is in the car 
  // 0 - no child
  // 1 - child present
  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      float pounds = i/(453.592);
      Serial.print("Load_cell output val: ");
      Serial.println(pounds);
      char weightBuff[10];
      String weight = "";
      dtostrf(pounds, 4, 6, weightBuff);  //4 is mininum width, 6 is precision
      weight = weightBuff;
      // pCharacteristic->setValue();
      pCharacteristic->setValue("Load Cell: " + weight);
      if(pounds >= 3){
        childInCar = 1;
        digitalWrite(greenLED, HIGH);
      }
      else{
        digitalWrite(greenLED, LOW);
      }
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
  
  char tempBuff[10];
  String temp = "";
  dtostrf(Fah, 4, 6, tempBuff);  //4 is mininum width, 6 is precision
  temp = tempBuff;
  // pCharacteristic->setValue();
  // pCharacteristic->setValue("temperature: " + temp);
  // setting temperature threshold for when to lower the windows.
  if(Fah >= thresholdTemp ){
    // digitalWrite(7,HIGH); 
    // Serial.println("THE CAR IS TOOOOOO HOT ");
    Serial.println("The car is over 85. Lower windows!");
    windowLowering = 1; // lower the window
    digitalWrite(redLED, HIGH);
  }
  else{
    // digitalWrite(7,LOW); 
    Serial.println("The car is under 85");
    windowLowering = 0; // dont lower the window 
    digitalWrite(redLED, LOW);
  }

  // window only lowers if child is in car
  if(childInCar == 1){
    // the first if is the scenario where you told the window to go down but the window does not lower.
    // if(windowLowering == 1 && confirmWindow == 0 && distIn <= thresholdProx){
    //   Serial.println("The window lowering mechanism is broken");
    // }
    // the window has been sent a signal to lower and it has lowered past the proximity sensor. 
    // This confirms that the window has lowered
    if(windowLowering == 1 && distIn <= thresholdProx){ 
      // digitalWrite(blueLED, HIGH);
      Serial.print("The window is lowering.");
      digitalWrite(blueLED, HIGH);
      Serial.println("Moving Upwward");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
      while (dutyCycle <= 255){
        ledcWrite(enable1Pin, dutyCycle);   
        Serial.print("Forward with duty cycle: ");
        Serial.println(dutyCycle);
        dutyCycle = dutyCycle + 5;
        delay(500);
      }
      dutyCycle = 200;
      // delay(69000); // wait three seconds for the window to get to the right level
      Serial.println("The window has been lowered to the correct level");
      windowLowering = 0; // reset signal
      Serial.println("The window is lowered after request");
      confirmWindow = 1; // reset signal
      digitalWrite(blueLED, LOW);
    }
    // digitalWrite(blueLED, LOW);
  }
  else{
    Serial.println("Child is not in car");
    
  }
  
  // else if(windowLowering == 1 && distIn >= thresholdProx){ // The window is up and has not lowered.
  //   Serial.println("The window is lowered after request");
  //   confirmWindow = 1; // reset signal
  //   // digitalWrite(blueLED, LOW);
  // }
 


  // Print values to serial monitor
  // Serial.print("Voltage: ");
  // Serial.print(volt);
  // Serial.print("| SensorVal: ");
  // Serial.print(sensorValue);
  Serial.print("Temperature: ");
  // Serial.print(kelvin);
  // Serial.print(" K | ");
  Serial.print(Cels);
  Serial.print(" °C | ");
  Serial.print(Fah);
  Serial.println(" °F");
  // Serial.print("Distance (cm): ");
  // Serial.println(distCm);
  Serial.print("Distance (inch): ");
  Serial.println(distIn);
  
  delay(2000); // wait 0.5 seconds in between readings
}
