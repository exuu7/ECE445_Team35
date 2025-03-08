// #include "MQ7.h"
const int analogInPin = 13; 
const int ledPin = 3;// LED connected to digital pin 13
int sensorValue = 0; // value read from the sensor
float GasFactor = 24.57143; 
// MQ7 mq7(13, 5);
void setup() {
// initialize serial communications at 9600 bps:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // sets the digital pin as output

  // Serial.println("Calibrating MQ7");
  // mq7.calibrate();    // calculates R0
  // Serial.println("Calibration done!");
}

void loop() {
    sensorValue = analogRead(analogInPin);   
    Serial.print("CO Sensor   : ");
    Serial.print(sensorValue / GasFactor);  
    Serial.println(" ppm");
    delay(1750);
// read the analog in value:
  // sensorValue = analogRead(analogInPin);
  // Serial.print("PPM = ");
  //   float ppm = mq7.readPpm();
  //   Serial.println(ppm);
  // determine alarm status
  // if (sensorValue >= 0){
  //   digitalWrite(ledPin, HIGH); // sets the LED on
  // }
  // else{
  //   digitalWrite(ledPin, LOW); // sets the LED off
  // }

  // print the results to the serial monitor:
  // Serial.print("sensor = ");
  // Serial.println(sensorValue);

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  // delay(100);
}
