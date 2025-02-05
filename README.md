# **📌 Complete Guide: Connecting ESP32, LoRa, BMP180, OLED & Remote Monitoring via PC**  

This guide will cover:  
✅ **Powering ESP32 with a battery**  
✅ **Connecting BMP180 sensor to ESP32**  
✅ **Transmitting sensor data via LoRa**  
✅ **Receiving LoRa data on a PC using Arduino Uno**  
✅ **Displaying data on Serial Monitor & OLED screen**  

---

## **🛠 Required Components:**  
- **ESP32 DevKit V1** (Transmitter)  
- **Arduino Uno** (Receiver on PC)  
- **LoRa SX1278 Module** (2 units)  
- **BMP180 Sensor** (for temperature & pressure)  
- **0.96" OLED Display** (Optional)  
- **18650 Li-ion or LiPo Battery with TP4056 Charger**  

---

## **1️⃣ Powering ESP32 with a Battery**  

**🔹 Using 18650 Li-ion Battery**  
1. **Battery** → Connect **B+ & B-** to **TP4056 Module**.  
2. **TP4056 OUT+ & OUT-** → Connect to **Boost Converter (MT3608 or XL6009)**.  
3. **Boost Converter Output (5V)** → Connect to **ESP32 VIN & GND**.  

Alternatively, use a **3.7V LiPo battery** with a **LiPo charging shield**.

---

## **2️⃣ Wiring ESP32 (Transmitter)**
### **ESP32 + LoRa + BMP180 + OLED**
| **Component** | **ESP32 GPIO** |
|-------------|--------------|
| **LoRa VCC** | 3.3V |
| **LoRa GND** | GND |
| **LoRa NSS (CS)** | GPIO5 |
| **LoRa MOSI** | GPIO23 |
| **LoRa MISO** | GPIO19 |
| **LoRa SCK** | GPIO18 |
| **LoRa RST** | GPIO14 |
| **LoRa DIO0** | GPIO26 |
| **BMP180 VCC** | 3.3V |
| **BMP180 GND** | GND |
| **BMP180 SDA** | GPIO21 |
| **BMP180 SCL** | GPIO22 |
| **OLED VCC** | 3.3V |
| **OLED GND** | GND |
| **OLED SDA** | GPIO21 |
| **OLED SCL** | GPIO22 |

---

## **3️⃣ Wiring Arduino Uno (Receiver)**
| **LoRa SX1278** | **Arduino Uno** |
|-------------|--------------|
| **VCC** | 3.3V |
| **GND** | GND |
| **NSS (CS)** | 10 |
| **MOSI** | 11 |
| **MISO** | 12 |
| **SCK** | 13 |
| **RST** | 9 |
| **DIO0** | 2 |

---

## **4️⃣ ESP32 Transmitter Code**
```cpp
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
```

---

## **5️⃣ Arduino Uno Receiver Code**
```cpp
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
```

---

## **6️⃣ Uploading Code to ESP32**
### **Method 1: Normal Upload**
1. Open **Arduino IDE**.  
2. Select **Board: ESP32 Dev Module**.  
3. Select **Port: COMx** (your ESP32’s port).  
4. Click **Upload**.  

### **Method 2: If Upload Fails**
1. Press and hold the **BOOT** button on ESP32.  
2. Click **Upload** in Arduino IDE.  
3. Release **BOOT** when uploading starts.  
4. If needed, press the **EN (Reset)** button after uploading.  

---

## **7️⃣ Uploading Code to Arduino Uno**
1. Open **Arduino IDE**.  
2. Select **Board: Arduino Uno**.  
3. Select **Port: COMx** (your Arduino’s port).  
4. Click **Upload**.  

---

## **8️⃣ Viewing Data on PC**
### **Using Serial Monitor**
1. **Connect Arduino Uno (Receiver) to PC via USB**.  
2. Open **Arduino IDE** → **Tools** → **Serial Monitor**.  
3. Set baud rate to **115200**.  
4. You should see real-time data from ESP32 via LoRa.  

---

## **🎯 Final Summary**
✅ **ESP32 Powered by Battery:** Li-ion with TP4056 & boost converter or LiPo with shield.  
✅ **LoRa Sends Data Wirelessly:** ESP32 **(transmitter)** → LoRa → Arduino Uno **(receiver on PC)**.  
✅ **Display Data on Serial Monitor & OLED Screen.**  
✅ **Press "BOOT" if upload fails on ESP32.**  

---

## **💡 Next Steps**
Would you like to:  
🔹 Send data to a **web dashboard** (e.g., Firebase, Thingspeak)?  
🔹 Store data on an **SD card**?  
🔹 Improve **LoRa range** with an external antenna?  

Let me know! 🚀
