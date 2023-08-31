#include "WiFi.h"
#include "Wire.h"

WiFiClient client;
// const int    HTTP_PORT   = 80;
// const String HTTP_METHOD = "GET"; // or "POST"
// hostname of web server:
//String queryString = String("&temperature=") + String(temp);
const char* ssid = "NCAIR IOT";
const char* password =  "Asim@123Tewari";
// const char* ssid = "Tanmay";
// const char* password =  "123456781808";


const char* Server = "192.168.0.121";
int port = 80;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }


  Serial.println("Connected to the WiFi network");
 
//     readdata();
  if (client.connect(Server, port)) {  //check esp 32 connected to server or not
    Serial.println("Connected to server");
  } else {
    Serial.println("connection failed");
  }
}
void loop(){


}
