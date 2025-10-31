const int ldrPin = 23; // DO pin
int ldrState = 0;

void setup() {
  pinMode(ldrPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  ldrState = digitalRead(ldrPin);
  if (ldrState == LOW) {
    Serial.println("Bright");
  } else {
    Serial.println("Dark");
  }
  delay(200);
}
