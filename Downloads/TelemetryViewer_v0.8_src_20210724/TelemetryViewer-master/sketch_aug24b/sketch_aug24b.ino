#include "Wire.h"

const int MPU = 0x68;
int16_t AcX, AcY, AcZ;
String Body = "";

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Set to zero to wake up the MPU-6050
  Wire.endTransmission(true);
}

void loop() {
  readData();
  sendSensorDataToSerial(); // Print data to Serial for debugging
  delay(1000);
}

void readData() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.requestFrom(MPU, 6); // request 6 bytes of data from MPU

  if (Wire.available() == 6) {
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
  }
}

void sendSensorDataToSerial() {
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.println(AcZ);
}
