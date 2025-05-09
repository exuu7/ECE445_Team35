//Citation: https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/basic_projects/ar_thermistor.html
const int analogPin = 4; // ESP32 Analog Pin 
const int resistorValue = 10000; // 10k resistor used with the thermistor
const int nomTemp = 25; // temperature that the thermistor was tested under
const int nomRes = 10000; // resistance of the thermistor standard temp
const float sthh_const = 3950.0; // constant used in steinhart-hart equation
const float kel_const = 273.15; // kelvin constant
const int threshold = 85; // threshold temperature
// this pin is for the led. The led indicates whether or not the temperature is past the threshold.
// This is to mimic the signal sent to lower the window
// LED ON: Temperature is too high
// LED OFF: Temperature is safe
const int redLED = 10; 
// sets up the esp32 with the pins and serial monitor
void setup() { 
  Serial.begin(115200); // sets the correct serial baud rate
  pinMode(redLED, OUTPUT); 
}

void loop() {
  
  
  
  int sensorValue = analogRead(analogPin); // stores the raw resistance value of the thermistor

  // Calculation: The thermistor and 10k ohm form a voltage divider circuit. 
  // The first part of the equation is finding the volatge across the thermistor. Vin * (thermistor res/(thermistor res + 10k)).
  // The 4095 is to convert to 12 bits. This is to return the voltage value
  float volt = (sensorValue * 3.3)/ 4095; // converts the resistance to a voltage value


  float resistance = (volt * resistorValue)/ (3.3 - volt); //this finds the updated resistance value using the above voltage 

  // converts resistance to temperature using the Steinhart-Hart equation
  float kelvin = 1 / (((log(resistance / nomRes)) / sthh_const) + (1 / (nomTemp + kel_const)));

  float Cels = kelvin - kel_const; // convert Kelvin to Celsius 
  float Fah = 1.8 * Cels + 32.0; // convert temp to Fahrenheit

  // setting temperature threshold for when to lower the windows.
  if(Fah >= threshold ){
    digitalWrite(7,HIGH); // led is set to on
    // Serial.println("THE CAR IS TOOOOOO HOT ");
    Serial.println("The car is over 85. Lower windows!");
  }
  else{
    digitalWrite(7,LOW); // led is set to on
    Serial.println("The car is under 85");
  }
  // Print values to serial monitor
  Serial.print("Voltage: ");
  Serial.print(volt);
  Serial.print("| SensorVal: ");
  Serial.print(sensorValue);
  Serial.print("Temperature: ");
  Serial.print(kelvin);
  Serial.print(" K | ");
  Serial.print(Cels);
  Serial.print(" °C | ");
  Serial.print(Fah);
  Serial.println(" °F");

  delay(1000); // wait one second in between readings
}
