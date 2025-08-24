#include "DHT.h"

#define RELAY_PIN 23
#define DHTPIN 18       // DATA của DHT22 nối vào GPIO4
#define DHTTYPE DHT22   // Chọn loại DHT22 (nếu DHT11 thì đổi lại)

// Khởi tạo DHT
DHT dht(DHTPIN, DHTTYPE);
const int aoPinSmoke = 34;  
const int doPinSmoke = 27;

void setup() {
 Serial.begin(115200);
 pinMode(doPinSmoke, INPUT);
 pinMode(aoPinSmoke, INPUT);
 pinMode(RELAY_PIN, OUTPUT);
 digitalWrite(RELAY_PIN, HIGH);
 dht.begin();
}

void loop() {
  int gasValue = analogRead(aoPinSmoke);
  int gasDigital = digitalRead(doPinSmoke);
  float Humidity = dht.readHumidity();
  float Temperature = dht.readTemperature();


  Serial.print("MQ-2 Analog Value: ");
  Serial.print(gasValue);
  Serial.print(" | Digital: ");
  Serial.println(gasDigital == LOW ? "Gas detected!" : "No gas");
  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.print(" % | Temperature: ");
  Serial.print(Temperature);
  Serial.println(" °C");
  Serial.println("Relay ON");
  digitalWrite(RELAY_PIN, LOW);  // turn relay ON
  delay(2000);

  Serial.println("Relay OFF");
  digitalWrite(RELAY_PIN, HIGH); // turn relay OFF
  delay(2000);

  delay(1000);
}
