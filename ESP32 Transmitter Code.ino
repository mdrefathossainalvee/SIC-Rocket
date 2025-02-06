#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <LoRa.h>

#define SS 5
#define RST 14
#define DIO0 26

Adafruit_BMP085 bmp;

void setup() {
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println("LoRa BMP180 Sender");

    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while (1);
    }

    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(915E6)) {  // Change frequency if needed (e.g., 433E6, 868E6)
        Serial.println("LoRa init failed. Check your connections.");
        while (1);
    }
    
    Serial.println("LoRa initialized.");
}

void loop() {
    float temperature = bmp.readTemperature();  // Read temperature in Celsius
    float pressure = bmp.readPressure() / 100.0; // Convert to hPa

    String dataToSend = String(temperature) + "," + String(pressure);

    Serial.print("Sending: ");
    Serial.println(dataToSend);

    LoRa.beginPacket();
    LoRa.print(dataToSend);
    LoRa.endPacket();

    delay(5000); // Send every 5 seconds
}
