#include <Wire.h>

const int HMC5883L_ADDR = 0x1E;

int16_t x, y, z;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Initialize HMC5883L
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x00); // Configuration Register A
  Wire.write(0x70); // 8-average, 15 Hz default, normal measurement
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x01); // Configuration Register B
  Wire.write(0xA0); // Gain = 5
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x02); // Mode Register
  Wire.write(0x00); // Continuous measurement mode
  Wire.endTransmission();
}

void loop() {
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x03); // Start with X MSB register
  Wire.endTransmission();
  Wire.requestFrom(HMC5883L_ADDR, 6);

  if (Wire.available() == 6) {
    x = Wire.read() << 8 | Wire.read();
    z = Wire.read() << 8 | Wire.read();
    y = Wire.read() << 8 | Wire.read();

    Serial.print("X: "); Serial.print(x);
    Serial.print(" | Y: "); Serial.print(y);
    Serial.print(" | Z: "); Serial.println(z);
  }

  delay(500);
}
