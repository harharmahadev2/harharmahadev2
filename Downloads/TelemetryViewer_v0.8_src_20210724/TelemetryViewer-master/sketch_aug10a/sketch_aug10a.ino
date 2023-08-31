#include <WiFi.h>
#include <HTTPClient.h>
#include<Wire.h>
const char* ssid="NCAIR IOT";
const char* password="Asim@123Tewari";

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
const int8_t SYNC_BYTE = 0xAA;

void setup() {
  Serial.begin(115200);
  delay(4000);
  
  Wire.begin(21, 22, 100000); // sda, scl, clock speed
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU−6050)
  Wire.endTransmission(true);
  Serial.println("Setup complete");


  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to wifi");
  }
  Serial.println("Connected to the network:");



}
 void sendPostRequest(int AcX, int AcY, int AcZ)
{
  if(WiFi.status()==WL_CONNECTED){
HTTPClient http;
http.begin("http://192.168.137.1/mpu6050proj/testdata.php");//dest for post

String data="AcX="+ String(AcX)+"&AcY="+ String(AcY)+"&AcZ="+ String(AcZ);
int httpResponseCode= http.POST(data);
Serial.print("HTTP Response Code: ");
Serial.println(httpResponseCode);

String response = http.getString();
Serial.print("HTTP Response: ");
Serial.println(response);


http.end();

}
else{
  Serial.println("Error in Wifi connection");
}
delay(1000);


}
void loop() {
Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU_ADDR);
  Wire.requestFrom(MPU_ADDR, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)  MSB and LSB |
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) &  0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (ACCEL_XOUT_H) & 0x44 (ACCEL_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (ACCEL_YOUT_H) & 0x46 (ACCEL_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (ACCEL_ZOUT_H) & 0x48 (ACCEL_ZOUT_L)

  Serial.write(SYNC_BYTE); // Send the start/sync byte
  Serial.write((uint8_t*) & (AcX), sizeof(AcX));
  Serial.write((uint8_t*) & (AcY), sizeof(AcY));
  Serial.write((uint8_t*) & (AcZ), sizeof(AcZ));
  Serial.write((uint8_t*) & (GyX), sizeof(GyX));  // casting the intital values to an unsigned data type to avoid overflow
  Serial.write((uint8_t*) & (GyY), sizeof(GyY));
  Serial.write((uint8_t*) & (GyZ), sizeof(GyZ));
  sendPostRequest(AcX,AcY,AcZ);
delay(4000);
}


