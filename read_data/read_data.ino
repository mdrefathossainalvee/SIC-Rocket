#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!bmp.begin()) {
        Serial.println("Could not find BMP180 sensor!");
        while (1);
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
        Serial.println("SSD1306 allocation failed");
        while (1);
    }

    display.clearDisplay();
}

void loop() {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0;  // Convert to hPa

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temperature);
    display.println(" C");
    display.print("Pressure: ");
    display.print(pressure);
    display.println(" hPa");
    display.display();

    delay(1000);
}
