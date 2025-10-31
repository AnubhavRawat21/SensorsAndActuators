#include <Wire.h>

#define AS5600_ADDR 0x36 // Default I2C address

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

uint16_t readRawPosition() {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(0x0C); // Position high byte register
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, 2);

  if (Wire.available() == 2) {
    uint16_t pos = Wire.read() << 8;
    pos |= Wire.read();
    return pos; // 12-bit value: 0–4095
  }
  return 0;
}

void loop() {
  uint16_t rawPos = readRawPosition();
  float angle = (rawPos * 360.0) / 4096.0; // Convert to degrees
  Serial.print("Angle: ");
  Serial.println(angle);
  delay(200);
}
