//Citation for Temp Sensor: https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/basic_projects/ar_thermistor.html
//Citation for Proximity Sensor:https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-arduino/
// ---------------------------------- Temp Sensor Constants----------------------------------------------------------
const int analogPin = 4; // ESP32 Analog Pin 
const int resistorValue = 10000; // 10k resistor used with the thermistor
const int nomTemp = 25; // temperature that the thermistor was tested under
const int nomRes = 10000; // resistance of the thermistor standard temp
const float sthh_const = 3950.0; // constant used in steinhart-hart equation
const float kel_const = 273.15; // kelvin constant
const int thresholdTemp = 78; // threshold temperature
// this pin is for the led. The led indicates whether or not the temperature is past the threshold.
// This is to mimic the signal sent to lower the window
// LED ON: Temperature is too high
// LED OFF: Temperature is safe
const int redLED = 10; 

// ---------------------------------- Prox Sensor Constants----------------------------------------------------------
const int trigPin = 7; // pin that the trigger pin is connected to on the microcontroller
const int echoPin = 12; // pin that the echo pin is connected to on the microcontroller
// this pin is for the led. The LED indicates whether or not the window is lowered or not.
// LED ON: The window is not lowered
// LED OFF: The window is lowered
const int blueLED = 20; 
// threshold for how far away the window should be to confirm whether the window is present in front
const int thresholdProx = 4; 

#define SOUND_SPEED 0.034 // velocity of sound in cm/us
#define CM_TO_INCH 0.393701 // allows for conversion for centimeters to inches

long objDectTime;
float distCm;
float distIn;
int windowLowering; // flag to indicate the status of the window
// windowLowering = 1 -> lower the window
// windowLowering = 0 -> window is up
int confirmWindow = 0; 
// 1 means that the window lowered when told to
// 0 means that the window did not lower when told to

// sets up the esp32 with the pins and serial monitor
void setup() { 
  Serial.begin(115200); //     sets the correct serial baud rate
  pinMode(redLED, OUTPUT); // LED for temp sensor
  pinMode(trigPin, OUTPUT);// Sets the trigPin as an Output - emits the ultrasound (prox)
  pinMode(echoPin, INPUT); //  Sets the echoPin as an Input - recieves the reflected wave (prox)
  pinMode(blueLED, OUTPUT);// LED for prox sensor
}

void loop() {
  
  int sensorValue = analogRead(analogPin); // stores the raw resistance value of the thermistor
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
  // setting temperature threshold for when to lower the windows.
  if(Fah >= thresholdTemp ){
    digitalWrite(7,HIGH); // led is set to on
    // Serial.println("THE CAR IS TOOOOOO HOT ");
    Serial.println("The car is over 85. Lower windows!");
    windowLowering = 1; // lower the window
  }
  else{
    digitalWrite(7,LOW); // led is set to on
    Serial.println("The car is under 85");
    windowLowering = 0; // dont lower the window 
  }

  // the first if is the scenario where you told the window to go down but the window does not lower.
  if(windowLowering == 1 && confirmWindow == 0 && distIn <= thresholdProx){
    Serial.println("The window lowering mechanism is broken");
  }
  // the window has been sent a signal to lower and it has lowered past the proximity sensor. 
  // This confirms that the window has lowered
  else if(windowLowering == 1 && distIn <= thresholdProx){ 
    digitalWrite(blueLED, HIGH);
    Serial.print("The window is lowering.");
    delay(3000); // wait three seconds for the window to get to the right level
    Serial.println("The window has been lowered to the correct level");
    windowLowering = 0; // reset signal
  }
  else if(windowLowering == 1 && distIn >= thresholdProx){ // The window is up and has not lowered.
    Serial.println("The window is lowered after request");
    confirmWindow = 1; // reset signal
    digitalWrite(blueLED, LOW);
  }
 


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
