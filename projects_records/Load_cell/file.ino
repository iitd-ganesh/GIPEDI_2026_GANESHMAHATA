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








//For calibrating use the below  commented code + Serial Monitor
// #include "HX711.h"

// #define DOUT 32   // GPIO13
// #define CLK  26   // GPIO15

// HX711 scale;

// void setup() {
//   Serial.begin(115200);

//   scale.begin(DOUT, CLK);

//   Serial.println("HX711 Raw Value Test");
// }

// void loop() {

//   if (scale.is_ready()) {

//     long raw = scale.read();

//     Serial.print("RAW = ");
//     Serial.println(raw);

//   } else {
//     Serial.println("HX711 not found!");
//   }

//   delay(500);
// }
