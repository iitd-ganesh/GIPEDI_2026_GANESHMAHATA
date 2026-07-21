#include <Wire.h>                  // I2C communication library
#include <RTClib.h>                // RTC DS1307 library
#include <DHT.h>                   // DHT temperature/humidity sensor library
#include <Adafruit_GFX.h>          // Graphics library for OLED
#include <Adafruit_SSD1306.h>      // SSD1306 OLED driver

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED display object
Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1                            // Reset pin not used
);

// Create RTC object
RTC_DS1307 rtc;

// DHT11 sensor configuration
#define DHTPIN 7
#define DHTTYPE DHT11

// Create DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// RGB LED pin definitions
#define RED_LED      2
#define GREEN_LED    3
#define BLUE_LED     5

// Push button used to turn system ON/OFF
#define POWER_BUTTON 8

// Stores current system state
bool systemState = false;

// Used for button edge detection
bool lastButtonState = HIGH;

void setup()
{
  // Configure RGB LED pins as outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Configure button input with internal pull-up resistor
  pinMode(POWER_BUTTON, INPUT_PULLUP);

  // Initialize I2C communication
  Wire.begin();

  // Initialize RTC module
  rtc.begin();

  // Initialize DHT11 sensor
  dht.begin();

  // Uncomment once to set RTC time from PC compile time
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    // Halt execution if OLED initialization fails
    while (1);
  }

  // Clear display buffer
  display.clearDisplay();
  display.display();
}

void loop()
{
  // Read current button state
  bool buttonState = digitalRead(POWER_BUTTON);

  // Detect button press (falling edge)
  if (
      buttonState == LOW &&
      lastButtonState == HIGH
     )
  {
    // Debounce delay
    delay(50);

    // Toggle system state
    systemState = !systemState;

    if (systemState == true)
    {
      // Display POWER ON message
      display.clearDisplay();

      display.setTextSize(2);
      display.setTextColor(WHITE);

      display.setCursor(10, 25);
      display.println("POWER ON");

      display.display();

      delay(1000);
    }
    else
    {
      // Display POWER OFF message
      display.clearDisplay();

      display.setTextSize(2);
      display.setTextColor(WHITE);

      display.setCursor(10, 25);
      display.println("POWER OFF");

      display.display();

      delay(1000);

      // Clear screen after shutdown message
      display.clearDisplay();
      display.display();
    }
  }

  // Save current button state for next loop iteration
  lastButtonState = buttonState;

  // If system is OFF, disable all LEDs and exit loop
  if (systemState == false)
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    return;
  }

  // Read current date and time from RTC
  DateTime now = rtc.now();

  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Variable to store weather status text
  String statusText;

  // Temperature-based status indication
  if (temperature > 35)
  {
    // Hot condition → Red LED
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    statusText = "EXTREME HOT";
  }

  else if (temperature < 20)
  {
    // Cold condition → Blue LED
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);

    statusText = "COLD";
  }

  else
  {
    // Normal condition → Green LED
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);

    statusText = "NORMAL";
  }

  // ---------------- OLED DISPLAY ----------------

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Project title
  display.setCursor(0, 0);
  display.println("WEATHER SYSTEM");

  // Display current time
  display.setCursor(0, 15);
  display.print("TIME : ");

  print2digit(now.hour());
  display.print(":");

  print2digit(now.minute());
  display.print(":");

  print2digit(now.second());

  // Display temperature
  display.setCursor(0, 30);
  display.print("TEMP : ");
  display.print(temperature);
  display.println(" C");

  // Display humidity
  display.setCursor(0, 42);
  display.print("HUM  : ");
  display.print(humidity);
  display.println(" %");

  // Display weather status
  display.setCursor(0, 55);
  display.print("STATUS:");
  display.println(statusText);

  // Update OLED screen
  display.display();

  // Refresh delay
  delay(50);
}

// Function to print numbers with leading zero
// Example: 7 → 07
void print2digit(int number)
{
  if (number < 10)
  {
    display.print("0");
  }

  display.print(number);
}