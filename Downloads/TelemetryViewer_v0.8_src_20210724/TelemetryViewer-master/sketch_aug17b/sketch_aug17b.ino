#include "WiFi.h"
#include "Wire.h"


WiFiClient client;
const int    HTTP_PORT   =80;
const String HTTP_METHOD = "POST"; // or "POST"
const char   HOST_NAME[] = "192.168.0.131"; // hostname of web server:
const String PATH_NAME   = "/mpu6050proj/testdata.php";


const int N = 25;
const int MPU = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

String Body = "";
 String test = "457";
int BodyLen;
int temp = 76.88;
//String queryString = String("&temperature=") + String(temp);
const char* ssid = "NCAIR IOT";
const char* password =  "Asim@123Tewari";

const char* Server = "192.168.0.131";
int port = 80;
void readdata() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.requestFrom(MPU, 14); // request 14 bytes of data from MPU
  while (Wire.available() < 14); // wait for all data to be available
  AcX = Wire.read() << 8 | Wire.read(); // combine the two bytes into a 16-bit int
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  // Tmp = Wire.read() << 8 | Wire.read(); // temperature data in two's complement
  // GyX[0] = Wire.read() << 8 | Wire.read();
  // GyY[0] = Wire.read() << 8 | Wire.read();
  // GyZ[0] = Wire.read() << 8 | Wire.read();

 String data = "AcX=" + String(AcX) + "&AcY=" + String(AcY) + "&AcZ=" + String(AcZ);
Body = data;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Body = "data=" + test;
   if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
    return;
  }
  
}
void loop() {
  readdata();
  sendSensorDataToServer();
  delay(1000);

}
void sendSensorDataToServer() {
 
 

client.println("POST " + PATH_NAME + " HTTP/1.1");

  client.print("Host: "); client.println(Server);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: "); client.println(Body.length());
  client.println("Connection: Close");
  client.println();
  client.println(Body);


  // Wait for server response
  while (client.available() == 0);

  while (client.available()) {
    char c = client.read();
    client.write(c);
  }
  client.stop();
}