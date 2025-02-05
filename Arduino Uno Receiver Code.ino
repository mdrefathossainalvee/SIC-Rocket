#include <SPI.h>
#include <LoRa.h>

#define SS 10
#define RST 9
#define DI0 2

void setup() {
    Serial.begin(115200);
    if (!LoRa.begin(915E6)) {
        Serial.println("LoRa Receiver Failed!");
        while (1);
    }
    Serial.println("LoRa Receiver Ready!");
}

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        Serial.print("Received: ");
        while (LoRa.available()) {
            Serial.print((char)LoRa.read());
        }
        Serial.println();
    }
}
