/*********
  This is a fork from work of:
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
  Controlling two garage doors VIA IAoT
  
  Project file:
  https://www.codemacs.com/iaot/applications/esp8266-garage-door-opener.4002131.htm
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "SSID_PASSWORD";

ESP8266WebServer server(80);

String webPage = "";

static const int LeftGPIO = D1;
static const int RightGPIO = D2;

void setup(void){
  webPage += "<h1>Garage Doors</h1><p><a href=\"left\"><button>Left Gate</button></a>&nbsp;<a href=\"right\"><button>Right Gate</button></a></p>";  
  // preparing GPIOs
  pinMode(LeftGPIO, OUTPUT);
  digitalWrite(LeftGPIO, HIGH);
  
  pinMode(RightGPIO, OUTPUT);
  digitalWrite(RightGPIO, HIGH);
 
  Serial.begin(115200); 
  delay(5000);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  // Plain server response without any action
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  // Server response is to emulate a button press by shortening connection for half a second
  server.on("/left", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(LeftGPIO, LOW);
    delay(500);
    digitalWrite(LeftGPIO, HIGH);
    delay(1000);
  });
  // Server response is to emulate a button press by shortening connection for half a second
  server.on("/right", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(RightGPIO, LOW);
    delay(500);
    digitalWrite(RightGPIO, HIGH);
    delay(1000);
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
