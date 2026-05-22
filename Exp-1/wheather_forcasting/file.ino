#include <Wire.h>
#include <RTClib.h>

#include <DHT.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



// =====================================================
// OLED SETTINGS
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);



// =====================================================
// RTC OBJECT
// =====================================================

RTC_DS1307 rtc;



// =====================================================
// DHT11 SETTINGS
// =====================================================

#define DHTPIN 7

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);



// =====================================================
// LED PINS
// =====================================================

#define RED_LED      2

#define GREEN_LED    3

#define BLUE_LED     5



// =====================================================
// POWER BUTTON
// =====================================================

#define POWER_BUTTON 8



// =====================================================
// VARIABLES
// =====================================================

bool systemState = false;

bool lastButtonState = HIGH;



// =====================================================
// SETUP
// =====================================================

void setup()
{
  pinMode(RED_LED, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);

  pinMode(BLUE_LED, OUTPUT);

  pinMode(POWER_BUTTON, INPUT_PULLUP);



  Wire.begin();

  rtc.begin();

  dht.begin();



  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));



  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }



  // START WITH BLACK SCREEN

  display.clearDisplay();

  display.display();
}



// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // =================================================
  // BUTTON TOGGLE
  // =================================================

  bool buttonState = digitalRead(POWER_BUTTON);



  if (
      buttonState == LOW &&
      lastButtonState == HIGH
     )
  {
    delay(200);

    systemState = !systemState;



    // =============================================
    // POWER ON MESSAGE
    // =============================================

    if (systemState == true)
    {
      display.clearDisplay();

      display.setTextSize(2);

      display.setTextColor(WHITE);

      display.setCursor(10, 25);

      display.println("POWER ON");

      display.display();

      delay(2000);
    }



    // =============================================
    // POWER OFF MESSAGE
    // =============================================

    else
    {
      display.clearDisplay();

      display.setTextSize(2);

      display.setTextColor(WHITE);

      display.setCursor(10, 25);

      display.println("POWER OFF");

      display.display();

      delay(2000);



      // BLACK SCREEN AFTER POWER OFF

      display.clearDisplay();

      display.display();
    }
  }



  lastButtonState = buttonState;



  // =================================================
  // SYSTEM OFF
  // =================================================

  if (systemState == false)
  {
    digitalWrite(RED_LED, LOW);

    digitalWrite(GREEN_LED, LOW);

    digitalWrite(BLUE_LED, LOW);

    return;
  }



  // =================================================
  // RTC TIME
  // =================================================

  DateTime now = rtc.now();



  // =================================================
  // SENSOR DATA
  // =================================================

  float temperature = dht.readTemperature();

  float humidity = dht.readHumidity();



  // =================================================
  // WEATHER LOGIC
  // =================================================

  String statusText;



  // EXTREME HOT

  if (temperature > 35)
  {
    digitalWrite(RED_LED, HIGH);

    digitalWrite(GREEN_LED, LOW);

    digitalWrite(BLUE_LED, LOW);

    statusText = "EXTREME HOT";
  }



  // COLD WEATHER

  else if (temperature < 20)
  {
    digitalWrite(RED_LED, LOW);

    digitalWrite(GREEN_LED, LOW);

    digitalWrite(BLUE_LED, HIGH);

    statusText = "COLD";
  }



  // NORMAL WEATHER

  else
  {
    digitalWrite(RED_LED, LOW);

    digitalWrite(GREEN_LED, HIGH);

    digitalWrite(BLUE_LED, LOW);

    statusText = "NORMAL";
  }



  // =================================================
  // OLED DISPLAY
  // =================================================

  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(WHITE);



  // TITLE

  display.setCursor(0, 0);

  display.println("WEATHER SYSTEM");



  // TIME

  display.setCursor(0, 15);

  display.print("TIME : ");

  print2digit(now.hour());

  display.print(":");

  print2digit(now.minute());

  display.print(":");

  print2digit(now.second());



  // TEMPERATURE

  display.setCursor(0, 30);

  display.print("TEMP : ");

  display.print(temperature);

  display.println(" C");



  // HUMIDITY

  display.setCursor(0, 42);

  display.print("HUM  : ");

  display.print(humidity);

  display.println(" %");



  // STATUS

  display.setCursor(0, 55);

  display.print("STATUS:");

  display.println(statusText);



  display.display();



  delay(1000);
}



// =====================================================
// PRINT 2 DIGIT FUNCTION
// =====================================================

void print2digit(int number)
{
  if (number < 10)
  {
    display.print("0");
  }

  display.print(number);
}