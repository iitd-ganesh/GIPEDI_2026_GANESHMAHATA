#include <Wire.h>                  // I2C communication library
#include <Adafruit_GFX.h>          // Graphics library for OLED display
#include <Adafruit_SSD1306.h>      // SSD1306 OLED display driver
#include <DHT.h>                   // DHT temperature and humidity sensor library

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// DHT11 sensor configuration
#define DHTPIN 2                   // DHT11 data pin connected to Arduino pin 2
#define DHTTYPE DHT11              // Sensor type

// Create DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Create OLED display object
Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1                             // Reset pin not used
);

// Variable to store system ON/OFF state
bool systemON = false;

void setup()
{
  // Initialize DHT sensor
  dht.begin();

  // Initialize OLED display at I2C address 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear display buffer
  display.clearDisplay();

  // Output pins for LEDs / indicators
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  // Push button input with internal pull-up resistor
  pinMode(7, INPUT_PULLUP);
}

void loop()
{
  // Toggle system state when button is pressed
  if (digitalRead(7) == LOW)
  {
    systemON = !systemON;

    // Simple debounce delay
    delay(300);
  }

  // Execute only when system is ON
  if (systemON)
  {
    // Read temperature from DHT11 sensor
    float temp = dht.readTemperature();

    // Clear previous display content
    display.clearDisplay();

    // Set text size and color
    display.setTextSize(2);
    display.setTextColor(WHITE);

    // Create temperature string
    String text = String(temp) + " C";

    // Calculate coordinates to center the text
    int x = (128 - (text.length() * 12)) / 2;
    int y = (64 - 16) / 2;

    // Set cursor position
    display.setCursor(x, y);

    // Print temperature value
    display.print(text);

    // Update OLED display
    display.display();

    // Temperature threshold check
    if (temp >= 28)
    {
      // High temperature indication
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);

      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
    }
    else
    {
      // Normal temperature indication
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);

      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
    }
  }
  else
  {
    // Turn OFF all outputs when system is OFF
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);

    digitalWrite(9, LOW);
    digitalWrite(8, LOW);

    // Clear OLED display
    display.clearDisplay();
    display.display();
  }

  // Update interval
  delay(1000);
}