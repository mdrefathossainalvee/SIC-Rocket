#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SS 5
#define RST 14
#define DI0 26
#define OLED_RESET -1

Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);
    if (!LoRa.begin(915E6)) {
        Serial.println("LoRa init failed!");
        while (1);
    }
    Serial.println("LoRa Transmitter Ready!");

    if (!bmp.begin()) {
        Serial.println("BMP180 not found!");
        while (1);
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED init failed!");
        while (1);
    }
    display.clearDisplay();
}

void loop() {
    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0;

    String data = "Temp=" + String(temp) + "C, Pressure=" + String(pressure) + "hPa";

    Serial.println("Sending: " + data);
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(data);
    display.display();

    delay(2000);
}
