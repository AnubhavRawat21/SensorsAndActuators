const int sensorPin = 23;  // S pin connected here
int sensorState = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  sensorState = digitalRead(sensorPin);
  if(sensorState == HIGH) {
    Serial.println("No obstacle detected");
  } else {
    Serial.println("Obstacle detected!");
  }
  delay(100);
}
