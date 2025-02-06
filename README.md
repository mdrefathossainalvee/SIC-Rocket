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
```

---

## **5️⃣ Arduino Uno Receiver Code**
```cpp
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
