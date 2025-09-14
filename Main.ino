#include "DHT.h"
#include <WiFi.h>
#include "ThingSpeak.h"
#include <ESPAsyncWebServer.h>
#define RELAY_PIN 23
#define DHTPIN 18       // DATA của DHT22 nối vào GPIO4
#define DHTTYPE DHT22   // Chọn loại DHT22 (nếu DHT11 thì đổi lại)
AsyncWebServer server(80);
int relayState = LOW;  // Start OFF (for active LOW relay)

// Function to generate the HTML and CSS code for the web page
String getHTML() {
  String html = "<!DOCTYPE HTML><html><head>";
  html += "<style>";
  html += "body {background-color:#F0F0F0; font-family:Arial; text-align:center;}";
  html += "h1 {color:#333333;}";
  html += "button {width:150px; height:50px; font-size:20px; margin:10px;}";
  html += "</style></head><body>";
  html += "<h1>ESP32 Relay Control</h1>";
  html += "<p>Relay state: <span style='color:red;'>";
  if (relayState == LOW) html += "ON";
  else html += "OFF";
  html += "</span></p>";
  html += "<button onclick=\"window.location.href='/relay/on'\">Turn ON</button>";
  html += "<button onclick=\"window.location.href='/relay/off'\">Turn OFF</button>";
  html += "</body></html>";
  return html;
}
// Function to handle HTTP requests
void handleRequest(AsyncWebServerRequest *request) {
  String path = request->url();

  if (path == "/relay/on") {
    digitalWrite(RELAY_PIN, LOW);   // Active LOW
    relayState = LOW;
    request->send(200, "text/plain", "Relay turned ON");
  }
  else if (path == "/relay/off") {
    digitalWrite(RELAY_PIN, HIGH);  // Active LOW
    relayState = HIGH;
    request->send(200, "text/plain", "Relay turned OFF");
  }
  else {
    request->send(200, "text/html", getHTML());
  }
}

void connectWiFi() {
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// Khởi tạo DHT
const int aoPinSmoke = 34;  
const int doPinSmoke = 27;
const char* WIFI_NAME = "BKSTAR_T2_02"; // Name of the WiFi network
const char* WIFI_PASSWORD = "stemstar"; // Password of the WiFi network
const int myChannelNumber = 3031362; // ThingSpeak channel number
const char* myApiKey = "478ALT2ADMHGME1K"; // ThingSpeak API key
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
 Serial.begin(115200);
 pinMode(doPinSmoke, INPUT);
 pinMode(aoPinSmoke, INPUT);
 pinMode(RELAY_PIN, OUTPUT);
 digitalWrite(RELAY_PIN, HIGH);
 dht.begin();
 WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
 while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Wifi not connected"); // Print a message if WiFi is not connected
  }
 Serial.println("Wifi connected !"); // Print a message if WiFi is connected
 Serial.println("Local IP: " + String(WiFi.localIP())); // Print the local IP address
 WiFi.mode(WIFI_STA); // Set the WiFi mode to station mode
   connectWiFi();

 ThingSpeak.begin(client); // Initialize the ThingSpeak library
 server.onNotFound(handleRequest);
 server.begin();
 delay(600000);
}

void loop() {
  int gasValue = analogRead(aoPinSmoke);
  int gasDigital = digitalRead(doPinSmoke);
  float Humidity = dht.readHumidity();
  float Temperature = dht.readTemperature();


  Serial.print("MP-2 Analog Value: ");
  Serial.print(gasValue);
  Serial.print(" | Digital: ");
  Serial.println(gasDigital == LOW ? "Gas detected!" : "No gas");
  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.print(" % | Temperature: ");
  Serial.print(Temperature);
  Serial.println(" °C");
  ThingSpeak.setField(1,Temperature);
  ThingSpeak.setField(2,Humidity);
  ThingSpeak.setField(3,gasValue);
  int x = ThingSpeak.writeFields(myChannelNumber,myApiKey); // Write the data to the ThingSpeak channel
  if(x == 200){
    Serial.println("Data pushed successfully"); // Print a message if the data was successfully pushed to ThingSpeak
  }else{
    Serial.println("Push error" + String(x)); // Print an error message with the HTTP status code if there was an error pushing the data
  }
  Serial.println("---"); // Print a separator line
  if(gasDigital == LOW || Temperature >= 60){
    digitalWrite(RELAY_PIN, LOW);   // Active
    relayState = LOW;
  }
  else{
    digitalWrite(RELAY_PIN, HIGH);   // Turn off
    relayState = HIGH;
  }
  delay(20000);
}