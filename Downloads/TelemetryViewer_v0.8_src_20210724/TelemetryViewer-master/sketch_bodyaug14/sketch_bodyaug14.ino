#include "WiFi.h"
#include "Wire.h"


WiFiClient client;
const int    HTTP_PORT   = 443;
const String HTTP_METHOD = "POST"; // or "POST"
const char   HOST_NAME[] = "192.168.137.1"; // hostname of web server:
const String PATH_NAME   = "http://192.168.137.1/mpu6050proj/testdata.php";


const int N = 25;
const int MPU = 0x68;
int16_t AcX[N], AcY[N], AcZ[N], Tmp, GyX[N], GyY[N], GyZ[N];

String Body = "";
String test = "457";
int BodyLen;
int temp = 76.88;
//String queryString = String("&temperature=") + String(temp);
const char* ssid = "NCAIR IOT";
const char* password =  "Asim@123Tewari";

const char* Server = "192.168.137.1";
int port = 80;
void readdata() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  Wire.requestFrom(MPU, 14); // request 14 bytes of data from MPU
  while (Wire.available() < 14); // wait for all data to be available
  AcX[0] = Wire.read() << 8 | Wire.read(); // combine the two bytes into a 16-bit int
  AcY[0] = Wire.read() << 8 | Wire.read();
  AcZ[0] = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read(); // temperature data in two's complement
  GyX[0] = Wire.read() << 8 | Wire.read();
  GyY[0] = Wire.read() << 8 | Wire.read();
  GyZ[0] = Wire.read() << 8 | Wire.read();

  String data = "data=" + String(AcX[0]) + "&" + String(AcY[0]) + "&" + String(AcZ[0]);
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
    readdata();
  
sendSensorDataToServer();
  // Close the client connection
  client.stop();
}
void loop() {
  readdata();
  sendSensorDataToServer();
  delay(1000);

}
void sendSensorDataToServer() {
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
    return;
  }

  client.println("POST /mpu6050proj/testdata.php HTTP/1.1");

  // Headers
  client.print("Host: "); client.println(Server);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: "); client.println(Body.length());
  client.println("Connection: Close");
  client.println();
  client.println(Body);

  // Print the data being sent
  Serial.println(Body);

  // Wait for server response
  while (client.available() == 0);

  // Print Server Response
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}