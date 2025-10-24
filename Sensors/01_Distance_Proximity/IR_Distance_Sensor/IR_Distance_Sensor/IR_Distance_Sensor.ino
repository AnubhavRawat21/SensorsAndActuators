int IRSensor = 9;
int LED = 13;

void setup() {
  Serial.begin(9600);
  pinMode(IRSensor, INPUT);
  pinMode(LED, OUTPUT)
}

void loop() {
  int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input
  if (sensorStatus == 1) // Check if the pin high or not
  {
    // if the pin is high turn off the onboard Led
    digitalWrite(LED, LOW); // LED LOW
    Serial.println("Object Detected!"); // print Motion Detected! on the serial monitor window
  }
  else  {
    //else turn on the onboard LED
    digitalWrite(LED, HIGH); // LED High
    Serial.println("Object Removed"); // print Motion Ended! on the serial monitor window
  }
}
