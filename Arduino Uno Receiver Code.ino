#include <SPI.h>
#include <LoRa.h>

#define SS 10
#define RST 9
#define DIO0 2

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("LoRa BMP180 Receiver");

    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(915E6)) {  // Ensure frequency matches the transmitter
        Serial.println("LoRa init failed. Check your connections.");
        while (1);
    }

    Serial.println("LoRa initialized.");
}

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        Serial.print("Received: ");
        
        String receivedData = "";
        while (LoRa.available()) {
            receivedData += (char)LoRa.read();
        }

        Serial.println(receivedData);

        // Parse temperature and pressure
        int commaIndex = receivedData.indexOf(',');
        if (commaIndex != -1) {
            String tempString = receivedData.substring(0, commaIndex);
            String pressureString = receivedData.substring(commaIndex + 1);

            float temperature = tempString.toFloat();
            float pressure = pressureString.toFloat();

            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" °C");

            Serial.print("Pressure: ");
            Serial.print(pressure);
            Serial.println(" hPa");
        }

        Serial.println(); // For better readability
    }
}
