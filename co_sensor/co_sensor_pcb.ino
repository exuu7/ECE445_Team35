// #include "MQ7.h"
//Citation: https://forum.arduino.cc/t/how-to-run-for-loop-for-given-amount-of-time/1284182
//Citation: https://www.teachmemicro.com/use-mq-7-carbon-monoxide-sensor/

const int analogInPin = 1;
const int CO_DigitalOut = 2;

// const int ledPin = 3;// LED connected to digital pin 13
int sensorValue = 0; // value read from the sensor
float RL = 10000; // 10K - known resistance
float slope = -0.562; // slope of figure 3 on the datasheet
float constant = 5.973;
float GasFactor = 24.57143; 
// MQ7 mq7(13, 5);
void setup() {
// initialize serial communications at 9600 bps:
  Serial.begin(115200);
  // pinMode(ledPin, OUTPUT); // sets the digital pin as output
  pinMode(CO_DigitalOut,OUTPUT);
  // Serial.println("Calibrating MQ7");
  // mq7.calibrate();    // calculates R0
  // Serial.println("Calibration done!");
}



void loop() {
  // Using modular allows the code to continously repeat the 60-90 cycle instead of it only running once.
  unsigned long time = millis() % 150000;

  if(time < 60000){ // 0 to 60 seconds - heating cycle
    heatingCycle();
  }
  else if(time < 150000){ // 60 to 150 seconds - sensing cycle for 90 seconds
    sensingCycle();
  }


}

void heatingCycle(){
  Serial.println("Heating Cycle, Milliseconds: ");
  Serial.println(millis());
  digitalWrite(CO_DigitalOut,LOW);

  // callibrate and find R0
  // sensorValue = analogRead(analogInPin);   
  // float voltage = (float)sensorValue/1024*5.0; // convert sensor value to voltage
  // // float Rs = ((5 - voltage)/voltage) * RL;
  // float Rs = ((5.0 * RL)/voltage) - RL;

  // Serial.print("Rs Val: ");
  // Serial.println(Rs);
  // Serial.print("voltage: ");
  // Serial.println(voltage);

}

void sensingCycle(){
  //Sensing Cycle
  digitalWrite(CO_DigitalOut,HIGH);
  sensorValue = analogRead(analogInPin);   
  float coLevel = sensorValue / GasFactor;
  // float voltage = sensorValue * (1.5/1023.0); // convert sensor value to voltage
  // float Rs = ((1.5 - voltage)/voltage) * RL;
  // float ratioRsRL = Rs/;
  // float coLevel = constant * pow(ratioRsRL, slope);


  Serial.println("Sensing Cycle, Milliseconds: ");
  Serial.println(millis());
  Serial.print("CO Level : ");
  Serial.print(coLevel);  
  Serial.println(" ppm");
  if(coLevel >= 9){
    // digitalWrite(3,HIGH);
    Serial.println("CO Level is too high. Get your exhaust checked. Exit vehicle asap!!");
  }
  else{
    Serial.println("CO levels are safe.");
  }
}
