// Simple Position Reader with Averaging
// This helps smooth out small, fast "jitter"

const int FEEDBACK_PIN = A0; // Purple wire
const int NUM_READINGS = 10; // How many samples to average

void setup() {
  Serial.begin(9600);
}

void loop() {
  long total = 0; // Use 'long' to prevent overflow
  
  // Take 10 quick readings
  for (int i = 0; i < NUM_READINGS; i++) {
    total = total + analogRead(FEEDBACK_PIN);
  }
  
  // Calculate the average
  int position = total / NUM_READINGS;
  
  Serial.print("Averaged Position: ");
  Serial.println(position);
  
  delay(250); // Print 4 times a second
}