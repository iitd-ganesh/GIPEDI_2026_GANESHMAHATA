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