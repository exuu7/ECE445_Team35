//Citation: https://kursatsayhan.medium.com/using-bluetooth-low-energy-ble-with-flutter-3c70469af814
// Citation: https://forum.arduino.cc/t/how-do-you-convert-a-float-to-string-solved/237090/8

#include <stdlib.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

const int tempPin = 4; // ESP32 Temperature Sensor Analog Pin 

//-------------------------------------------------------------Temperature Sensor Constants----------------------------------------------
const int resistorValue = 10000; // 10k resistor used with the thermistor
const int nomTemp = 25; // temperature that the thermistor was tested under
const int nomRes = 10000; // resistance of the thermistor standard temp
const float sthh_const = 3950.0; // constant used in steinhart-hart equation
const float kel_const = 273.15; // kelvin constant
const int thresholdTemp = 85; // threshold temperature
float Fah;

void setup(){
   Serial.begin(115200);
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

}

void loop(){
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(tempPin); // stores the raw resistance value of the thermistor
  float volt = (sensorValue * 3.3)/ 4095; // converts the resistance to a voltage value
  float resistance = (volt * resistorValue)/ (3.3 - volt); //this finds the updated resistance value using the above voltage 
  // converts resistance to temperature using the Steinhart-Hart equation
  float kelvin = 1 / (((log(resistance / nomRes)) / sthh_const) + (1 / (nomTemp + kel_const)));
  float Cels = kelvin - kel_const; // convert Kelvin to Celsius 
  Fah = 1.8 * Cels + 32.0; // convert temp to Fahrenheit

  //  Serial.println(Fah);
    // int i;
    // float val;
    char buff[10];
    String valueString = "";
    Serial.begin(115200);
    // val = 0.0;
    // for (i = 0; i < 10; i++) {
    dtostrf(Fah, 4, 6, buff);  //4 is mininum width, 6 is precision
    Serial.print("val: ");
    Serial.println(Fah);
      // val += 5.0;
    valueString = buff;
    // valueString += ", ";
    // }
    Serial.println(valueString);
  
  //  string temp = String(Fah)
   pCharacteristic->setValue(valueString);
   delay(2000);

}
