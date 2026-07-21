# Digital Weight Scale using ESP32, HX711 & OLED Display

## 1. Project Name
**Digital Weight Scale using ESP32, HX711 & OLED Display**

---

## 2. Material Required

| Component | Quantity |
|-----------|----------|
| ESP32 Development Board | 1 |
| HX711 Module | 1 |
| Load Cell (5kg/10kg/20kg) | 1 |
| OLED Display (SSD1306, 128x64, I2C) | 1 |
| Jumper Wires | Several |
| Breadboard | 1 |
| USB Cable | 1 |

---

## 3. Pin Connections

| ESP32   | HX711 | OLED |
|---------|-------|------|
| 3.3v    | VCC   | VCC  |
| GND     | GND   | GND  |
| GPIO 22 | -     | SDA  |
| GPIO 21 | -     | SCL  |
| GPIO 26 | DATA  | -    |
| GPIO 32 | SCL   | -    |

**Load Cell to HX711:**
- **Red** → E+
- **Black** → E-
- **White** → A-
- **Green** → A+

---

## 4. Circuit Image

![Circuit Diagram](images/circuit_diagram.jpg)

*[Replace with actual path to your circuit image]*

---

## 5. Arduino Code

```c

/*
 * Digital Weight Scale using ESP32, HX711 & OLED Display
 * 
 * This code reads weight from a load cell via HX711,
 * displays rounded value on OLED and Serial Monitor
 */

//-This code include oled and hx711 ADC With load cell.

#include <Wire.h>                // I2C library for OLED communication
#include <Adafruit_GFX.h>        // Graphics library for OLED display
#include <Adafruit_SH110X.h>     // OLED display driver for SH1106
#include "HX711.h"               // HX711 library for load cell ADC

// ---------------- HX711 Pin Definitions ----------------
#define DOUT 32                  // Data pin for HX711 (ESP32 GPIO 32)
#define CLK 26                   // Clock pin for HX711 (ESP32 GPIO 26)

HX711 scale;                     // Create HX711 object

// ---------------- OLED Pin Definitions ----------------
// OLED uses I2C: SDA -> GPIO 21, SCL -> GPIO 22 (default for Wire)
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);  // Create OLED object (128x64)

// ---------------- Calibration ----------------
float calibration_factor = 2027.425;   // Calibration factor for accurate weight reading
// Formula: New Factor = (Current Factor × Known Weight) / Displayed Weight

void setup()
{
  Serial.begin(115200);          // Initialize serial communication for debugging
  Serial.println("Initializing...");

  // ---------- HX711 Initialization ----------
  scale.begin(DOUT, CLK);        // Initialize HX711 with DOUT and CLK pins
  scale.set_scale(calibration_factor);  // Apply calibration factor
  scale.tare();                  // Zero the scale (remove any offset)
  Serial.println("HX711 calibrated and tared");

  // ---------- OLED Initialization ----------
  display.begin(0x3C, true);     // Initialize OLED with I2C address 0x3C
  display.clearDisplay();        // Clear any previous content
  
  display.setTextSize(1);        // Set text size to small
  display.setTextColor(SH110X_WHITE);  // Set text color to white
  display.setCursor(0, 0);       // Set cursor to top-left corner
  display.println("Weight Scale");  // Display startup message
  display.display();             // Update OLED screen

  delay(1000);                   // Wait 1 second before starting main loop
}

void loop()
{
  // ---------- Read Weight ----------
  // Read average of 20 readings for stability and accuracy
  float weight = scale.get_units(20);
  
  // ---------- Handle Negative Values ----------
  if(weight < 0)                 // If weight is negative
    weight = weight;             // Keep the value (no change)
  // Note: This line does nothing. Better to use: weight = abs(weight);
  // Or: if(weight < 0) weight = 0;

  // ---------- Print on Serial Monitor ----------
  Serial.print("Weight: ");      // Print label
  Serial.print(weight, 1);       // Print weight with 1 decimal place
  Serial.println(" g");          // Print unit

  // ---------- Update OLED Display ----------
  display.clearDisplay();        // Clear OLED screen for new data

  // Display header
  display.setTextSize(1);        // Small text for header
  display.setCursor(0, 0);       // Top-left position
  display.println("ESP32 SCALE"); // Display title

  // Display weight value (large text)
  display.setTextSize(3);        // Large text for weight
  display.setCursor(0, 25);      // Position below header
  display.print(weight, 1);      // Print weight with 1 decimal
  display.print("g");            // Print unit

  display.display();             // Send data to OLED screen

  delay(500);                    // Wait 500ms before next reading
}
```

## 6. Notes

1. **Calibration:** 
   - The `calibration_factor` (2027.425) is a sample value
   - To calibrate, place a known weight and adjust the factor until the reading matches
   - Formula: `New Factor = (Current Factor × Known Weight) / Displayed Weight`

2. **OLED Address:**
   - This code uses SH1106 OLED with I2C address `0x3C`
   - If it doesn't work, try `0x3D`

3. **Pin Configuration:**
   - HX711: DOUT → GPIO 32, CLK → GPIO 26
   - OLED: SDA → GPIO 21, SCL → GPIO 22 (default I2C pins)

4. **Libraries Required:**
   - Install via Arduino Library Manager:
     - HX711 by Bogdan Necula
     - Adafruit GFX Library
     - Adafruit SH110X

5. **Troubleshooting:**
   - If readings fluctuate, increase averaging: `scale.get_units(20)` or higher
   - If weight shows negative, swap A+ and A- wires on HX711
   - Always tare the scale with no weight on it before taking measurements
   - If OLED doesn't display, check I2C connections and address

---

## 7. References

1. HX711 Datasheet - Avia Semiconductor
2. ESP32 Technical Reference Manual
3. Adafruit SH1106 OLED Documentation
4. Arduino HX711 Library: https://github.com/bogde/HX711
5. Adafruit GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
6. Adafruit SH110X Library: https://github.com/adafruit/Adafruit_SH110X


---