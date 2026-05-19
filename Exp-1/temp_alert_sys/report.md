# EXPERIMENT – 1 INTERFACING WITH ARDUINO

---

## PROJECT NAME

 Temperature Monitoring Alert System

---

## OBJECTIVE / PROBLEM STATEMENT

This project monitors temperature using a DHT11 sensor and activates a buzzer when the temperature exceeds the threshold value of 50°C. The system is designed using Arduino UNO in SimulIDE.

---

## COMPONENTS USED

| Component | Quantity |
|---|---|
| Arduino UNO | 1 |
| DHT11 Temperature Sensor | 1 |
| Buzzer | 1 |
| Jumper Wires | Required |

---

## PIN CONNECTIONS

| Device | Arduino Pin |
|---|---|
| DHT11 Data Pin | D1 |
| Buzzer | D8 |
| DHT11 VCC | 5V |
| DHT11 GND | GND |

---

## SOFTWARE USED

- Arduino IDE
- SimulIDE


---
## circuit diagram
![Circuit](/Exp-1/temp_alert_sys/Circuit%20diagram.png)
---

## REQUIRED LIBRARY

```cpp
#include <DHT.h>
```

---

## MAIN ARDUINO CODE
[Code](/Exp-1/temp_alert_sys/temp_alert_sys.ino)

```cpp
#include <DHT.h>

#define DHTPIN 1
#define DHTTYPE DHT11
#define BUZZER 8
#define THRESHOLD 50

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    pinMode(BUZZER, OUTPUT);
    dht.begin();
}

void loop()
{
    float temp = dht.readTemperature();

    if(temp >= THRESHOLD)
    {
        digitalWrite(BUZZER, HIGH);
    }
    else
    {
        digitalWrite(BUZZER, LOW);
    }

    delay(1000);
}
```

---

## WORKING PRINCIPLE

1. DHT11 sensor continuously reads temperature.
2. Arduino UNO compares the temperature with the threshold value.
3. If temperature is greater than or equal to 50°C:
   - Buzzer turns ON.
4. If temperature is below 50°C:
   - Buzzer remains OFF.
5. The process repeats continuously.

---

## OUTPUT

- Buzzer remains OFF below 50°C.
- Buzzer turns ON at or above 50°C.
- System performs real-time temperature monitoring.

---

## ADVANTAGES

- Simple and low-cost system
- Easy to implement
- Real-time monitoring
- Useful for safety applications

---

## APPLICATIONS

- Fire warning systems
- Temperature alert systems
- Industrial monitoring
- Home safety projects
- Embedded system learning

---

## CONCLUSION

The Digital Temperature Monitoring System using Arduino UNO and DHT11 sensor was successfully implemented in SimulIDE. The buzzer activated whenever the temperature crossed the threshold value of 50°C.

---
## References
 1. Arduino Project hub- With DHT11 sensor https://projecthub.arduino.cc/arcaegecengiz/using-dht11-12f621
2. DHT sensor Library - https://docs.arduino.cc/libraries/dht-sensor-library/#Releases

3. Arduino Uno board Pin Out - 
https://deepbluembedded.com/arduino-uno-pinout/

