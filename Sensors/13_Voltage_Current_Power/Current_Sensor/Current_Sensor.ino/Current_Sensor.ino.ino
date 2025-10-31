const int sensorPin = A0;
float sensitivity = 0.185;  // for HW-872A (5A)
float Vref = 2.5;
float Vcc = 5.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int adcValue = analogRead(sensorPin);
  float voltage = (adcValue / 1023.0) * Vcc;
  float current = (voltage - Vref) / sensitivity;  // in Amperes

  Serial.print("Current: ");
  Serial.print(current, 3);
  Serial.println(" A");

  delay(500);
}
