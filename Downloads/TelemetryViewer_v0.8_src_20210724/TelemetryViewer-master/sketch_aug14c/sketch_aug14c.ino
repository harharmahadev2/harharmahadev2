  #include "WiFi.h"
  #include "Wire.h"


  WiFiClient client;
  const int    HTTP_PORT   =80;
  const String HTTP_METHOD = "POST"; // or "POST"
  const char   HOST_NAME[] = "192.168.0.121"; // hostname of web server:
  const String PATH_NAME   = "http://192.168.0.121/mpu6050proj/testdata.php";


  const int N = 25;
  const int MPU = 0x68;
  float AcX[N], AcY[N], AcZ[N], Tmp, GyX[N], GyY[N], GyZ[N];

  String Body = "";
  String test = "457";
  int BodyLen;
  int temp = 76.88;
  //String queryString = String("&temperature=") + String(temp);
  const char* ssid = "NCAIR IOT";
  const char* password =  "Asim@123Tewari";

  const char* Server = "192.168.0.121";
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
    // Tmp = Wire.read() << 8 | Wire.read(); // temperature data in two's complement
    // GyX[0] = Wire.read() << 8 | Wire.read();
    // GyY[0] = Wire.read() << 8 | Wire.read();
    // GyZ[0] = Wire.read() << 8 | Wire.read();

  String data = "AcX=" + String(AcX[0]) + "&AcY=" + String(AcY[0]) + "&AcZ=" + String(AcZ[0]) + "&Tmp=" + String(Tmp);
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
  }
  void loop() {
    readdata();
    sendSensorDataToServer();
    delay(1000);

  }
  void sendSensorDataToServer() {
  Serial.print("REACHED HERE");
  Serial.println(HOST_NAME);
  
  Serial.println(HTTP_PORT);
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection failed");
    return;
  }

client.println("POST " + PATH_NAME + " HTTP/1.1");

  client.print("Host: "); client.println(Server);
  client.println("Content-Type: application/json");
  client.print("Content-Length: "); client.println(Body.length());
  client.println("Connection: Close");
  client.println();

  // Create a JSON object
  JSONObject data;
  data["AcX"] = AcX[0];
  data["AcY"] = AcY[0];
  data["AcZ"] = AcZ[0];
  // data["Tmp"] = Tmp;

  // Convert the JSON object to a string
  String jsonString = data.toString();

  // Send the JSON string to the server
  client.println(jsonString);

  Serial.println(jsonString);

  // Wait for server response
  while (client.available() == 0);

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  client.stop();
}