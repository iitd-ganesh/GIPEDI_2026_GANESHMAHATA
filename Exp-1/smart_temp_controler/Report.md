# Exp 1- ​Interfacing​ ​with​ ​Arduino

## Project Name

Digital Temperature Monitoring System

---

## Objective / Problem Statement

This project monitors temperature using a DHT11 sensor and displays live temperature data on an SSD1306 OLED display. The system can be turned ON/OFF using a push button. Based on the temperature value, LEDs, fan, and heater are controlled automatically.

---

## Components Used

| Component                | Quantity |
| ------------------------ | -------- |
| Arduino UNO              | 1        |
| DHT11 Temperature Sensor | 1        |
| SSD1306 OLED Display     | 1        |
| Red LED                  | 1        |
| Green LED                | 1        |
| DC Fan / Motor           | 1        |
| Heater / Bulb            | 1        |
| Push Button              | 1        |
| Jumper Wires             | Required |

---|---|
| Arduino UNO | 1 |
| DHT11 / DHT22 Sensor | 1 |
| SSD1306 OLED Display | 1 |
| Buzzer | 1 |
| Push Button | 1 |
| Motor / Relay Module | 1 |
| Jumper Wires | Required |

---

## Pin Connections

| Device         | Arduino Pin |
| -------------- | ----------- |
| DHT11 Data Pin | D2          |
| Red LED        | D3          |
| Green LED      | D4          |
| Push Button    | D7          |
| Heater         | D8          |
| Fan            | D9          |
| OLED SDA       | A4          |
| OLED SCL       | A5          |

---|---|
| DHT Sensor Data | D2 |
| OLED SDA | A4 |
| OLED SCL | A5 |
| Buzzer | D8 |
| Push Button | D7 |
| Motor / Relay | D9 |


---
## Circuit Diagram
![Circuit Diagram](temp_sys_circuit.png)


---

## Required Libraries

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
```

---

## Main Arduino Code

````cpp
// Include required libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED display width
#define SCREEN_WIDTH 128

// OLED display height
#define SCREEN_HEIGHT 64

// DHT11 data pin
#define DHTPIN 2

// DHT sensor type
#define DHTTYPE DHT11

// Create DHT object
DHT dht(DHTPIN, DHTTYPE);

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variable to store ON/OFF state
bool systemON = false;

void setup()
{
  // Start DHT sensor
  dht.begin();

  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear OLED
  display.clearDisplay();

  // Red LED pin
  pinMode(3, OUTPUT);

  // Green LED pin 
  pinMode(4, OUTPUT);

  // Fan pin
  pinMode(9, OUTPUT);

  // Heater pin
  pinMode(8, OUTPUT);

  // Push button pin with internal pull-up resistor
  pinMode(7, INPUT_PULLUP);
}

void loop()
{
  // Check if button is pressed
  if(digitalRead(7) == LOW)
  {
    // Toggle system state
    systemON = !systemON;

    // Small delay to prevent multiple toggles
    delay(300);
  }

  // If system is ON
  if(systemON)
  {
    // Read temperature
    float temp = dht.readTemperature();

    // Clear display
    display.clearDisplay();

    // Set text size
    display.setTextSize(2);

    // Set text color
    display.setTextColor(WHITE);

    // Create temperature text
    String text = String(temp) + " C";

    // Calculate center X position
    int x = (128 - (text.length() * 12)) / 2;

    // Calculate center Y position
    int y = (64 - 16) / 2;

    // Set cursor
    display.setCursor(x, y);

    // Print temperature
    display.print(text);

    // Update OLED
    display.display();

    // Temperature greater than or equal to 28
    if(temp >= 28)
    {
      // Turn ON red LED
      digitalWrite(3, HIGH);

      // Turn OFF green LED
      digitalWrite(4, LOW);

      // Turn ON fan
      digitalWrite(9, HIGH);

      // Turn OFF heater
      digitalWrite(8, LOW);
    }
    else
    {
      // Turn OFF red LED
      digitalWrite(3, LOW);

      // Turn ON green LED
      digitalWrite(4, HIGH);

      // Turn OFF fan
      digitalWrite(9, LOW);

      // Turn ON heater
      digitalWrite(8, HIGH);
    }
  }

  // If system is OFF
  else
  {
    // Turn OFF red LED
    digitalWrite(3, LOW);

    // Turn OFF green LED
    digitalWrite(4, LOW);

    // Turn OFF fan
    digitalWrite(9, LOW);

    // Turn OFF heater
    digitalWrite(8, LOW);

    // Clear OLED
    display.clearDisplay();

    // Update OLED
    display.display();
  }

  // Loop delay
  delay(1000);
}


---

## Working Principle

1. Push button toggles the system ON/OFF.
2. DHT11 sensor reads room temperature.
3. OLED display shows live temperature at the center of the screen.
4. If temperature is greater than or equal to 28°C:

   * Red LED turns ON
   * Fan turns ON
   * Heater turns OFF
5. If temperature is below 28°C:

   * Green LED turns ON
   * Heater turns ON
   * Fan turns OFF
6. When the system is OFF, all devices remain OFF.

---

## Output

* OLED displays current temperature.
* Red LED indicates high temperature.
* Green LED indicates low temperature.
* Fan runs automatically at high temperature.
* Heater runs automatically at low temperature.
* Push button controls complete system power state.

---

## Applications

* Smart room temperature control
* Automatic cooling and heating system
* Home automation projects
* Industrial temperature monitoring
* Embedded system learning projects

---

## Author

GANESH MAHATA
