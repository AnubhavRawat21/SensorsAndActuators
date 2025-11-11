thconst int touchPin = 11;   // KY-036 digital output
const int ledPin = 13;    // Onboard LED

void setup() {
  pinMode(touchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int state = digitalRead(touchPin);

  if (state == LOW) {
    Serial.println("Touched!");
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
