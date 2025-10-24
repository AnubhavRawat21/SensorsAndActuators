// CODE FOR RLS-08.
// THE CODE WILL PRINT 1 WHEN THE SENSOR DETECTS A BLACK LINE.

int sensorPins[8] = {10, A0, A1, A2, A3, A4, A5, 11};
int values[8];

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 8; i++) {
    values[i] = analogRead(sensorPins[i]);
    Serial.print(values[i]);
    Serial.print('\t');
  }
  Serial.println();
  delay(100);
}
