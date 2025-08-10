#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define RELAY_PIN  13
Adafruit_BME280 bme;
const int sensorPin = 25;

void setup() {
    Serial.begin(9600);
    Serial.println("Setup started");
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);
}
void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println("*C");
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println("hPa");
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println("m");
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println("%");
    if (digitalRead(sensorPin) == LOW) {
    Serial.println("⚠️ Gas detected!");
    } else {
    Serial.println("✅ Air is clean");
    }
    Serial.print("Gas concentration = ");
    Serial.println(analogRead(sensorPin));
    delay(1000);
    digitalWrite(RELAY_PIN, LOW);   // Relay ON
    delay(1000);
    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
    delay(1000);

}