// //PCB
// int canBus = 9;


// void setup() {
//   // put your setup code here, to run once:
//   pinMode(canBus, OUTPUT);

//   Serial.begin(115200);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(canBus, HIGH);
//   Serial.println("canBus is high");
//   delay(10000);
//   digitalWrite(canBus, LOW);
//   Serial.println("canBus is low");
//   delay(10000);
// }

// breakout board
int canBusIn = 18;


void setup() {
  // put your setup code here, to run once:
  pinMode(canBusIn, INPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int canBusState = digitalRead(canBusIn);
  if(canBusState == HIGH){
    Serial.println("Motors lower window");
  }
  else{
    Serial.println("Motors chill");
  }

}
