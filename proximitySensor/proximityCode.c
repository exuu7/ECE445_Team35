//Citation: https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-arduino/
const int trigPin = 7; // pin that the trigger pin is connected to on the microcontroller
const int echoPin = 12; // pin that the echo pin is connected to on the microcontroller
// this pin is for the led. The LED indicates whether or not the window is lowered or not.
// LED ON: The window is not lowered
// LED OFF: The window is lowered
const int blueLED = 20; 
const int thresholdProx = 2; // threshold for how far away the window should be to confirm whether the window is present in front

#define SOUND_SPEED 0.034 // velocity of sound in cm/us
#define CM_TO_INCH 0.393701 // allows for conversion for centimeters to inches

long objDectTime;
float distCm;
float distIn;
int windowLowering; // flag to indicate the status of the window
// windowLowering = 1 -> window is lowering
// windowLowering = 0 -> window is up


void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output - emits the ultrasound
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input - recieves the reflected wave
  pinMode(blueLED, OUTPUT); // Sets the led as an output
}

void loop() {
  // Clears the trigPin to ensure a clear signal is sent when trigpin is high
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
 
  // this function reads the pulse that has been sent to the echopin and returns the time taken to travel to and from the object
  objDectTime = pulseIn(echoPin, HIGH);
  

  // converts the time to distance
  distCm = duration * SOUND_SPEED/2;
  
  // Convert to distance from cm to inches
  distIn = distanceCm * CM_TO_INCH;


  windowLowering = 1; // the window is lowering
  // the window has been sent a signal to lower and it has lowered past the proximity sensor. 
  // This confirms that the window has lowered
  if(windowLowering == 1 && distanceInch >= thresholdProx){ 
    digitalWrite(blueLED, LOW);
    Serial.print("The window is lowered and is not lowering further");
    windowLowering = 0; // reset signal
  }
  else{ // The window is up and has not lowered.
    Serial.print("The window is not lowered");
    digitalWrite(blueLED, HIGH);
  }
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distCm);
  Serial.print("Distance (inch): ");
  Serial.println(distIn);
  
  delay(500); // delay time between checking. 
}
