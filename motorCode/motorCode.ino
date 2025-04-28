//Citation: https://randomnerdtutorials.com/esp32-dc-motor-l298n-motor-driver-control-speed-direction/

// Motor A
// int motor1Pin1 = 17; 
// int motor1Pin2 = 18; 
// int enable1Pin = 13; 
int motor1Pin1 = 13; 
int motor1Pin2 = 14; 
int enable1Pin = 15; 
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 250;

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
}

void loop() {
  // Move the DC motor forward at maximum speed
  // Serial.println("Moving Downward");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, HIGH); 
  // delay(2000);

  // // // Stop the DC motor
  // Serial.println("Motor stopped");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, LOW);
  // delay(1000);

  // // Move DC motor backwards at maximum speed
  // Serial.println("Moving Upward");
  // digitalWrite(motor1Pin1, HIGH);
  // digitalWrite(motor1Pin2, LOW); 
  // delay(2000);

  // // // Stop the DC motor
  // Serial.println("Motor stopped");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, LOW);
  // delay(1000);

  // // Serial.println("Moving Upward");
  // digitalWrite(motor1Pin1, HIGH);
  // digitalWrite(motor1Pin2, LOW); 

  // // Move DC motor forward with increasing speed
  // Serial.println("Moving down");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, HIGH);
  // while (dutyCycle <= 255){
  //   ledcWrite(enable1Pin, dutyCycle);   
  //   Serial.print("Forward with duty cycle: ");
  //   Serial.println(dutyCycle);
  //   dutyCycle = dutyCycle + 5;
  //   delay(500);
  // }
  // dutyCycle = 200;


  Serial.println("Moving Upward");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  //   while (dutyCycle <=400){
  //   ledcWrite(enable1Pin, dutyCycle);   
  //   Serial.print("Forward with duty cycle: ");
  //   Serial.println(dutyCycle);
  //   dutyCycle = dutyCycle + 5;
  //   delay(500);
  // }
  dutyCycle = 255;
  // digitalWrite(enable1Pin, HIGH);
  ledcWrite(enable1Pin, dutyCycle);


}
