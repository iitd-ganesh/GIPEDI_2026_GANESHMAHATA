/*
   =====================================================
      Alarm Based Motor ON/OFF System using RTC
   =====================================================

   COMPONENTS USED
   -----------------------------------------------------
   Arduino UNO
   DS1307 RTC Module
   SSD1306 OLED Display
   Water Supply Switch Sensor
   Pump Indicator LED

   -----------------------------------------------------
   PIN CONNECTIONS
   -----------------------------------------------------

   Water Supply Input  -> D1
   Pump LED            -> D3

   RTC SDA             -> A4
   RTC SCL             -> A5

   OLED SDA            -> A4
   OLED SCL            -> A5

   -----------------------------------------------------
   WORKING
   -----------------------------------------------------

   1. When water supply is available:
      - D1 receives HIGH signal
      - Pump LED turns ON

   2. If water supply remains HIGH continuously
      for 1 hour:
      - Alarm condition achieved

   3. If water supply becomes LOW before 1 hour:
      - Wait 2 seconds
      - Turn OFF Pump LED

   4. OLED Displays:
      - Real Time
      - Water Supply Status
      - Pump Status

*/


#include <Wire.h>
#include <RTClib.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// =====================================================
// OLED SETTINGS
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,
                         SCREEN_HEIGHT,
                         &Wire,
                         -1);


// =====================================================
// RTC OBJECT
// =====================================================

RTC_DS1307 rtc;


// =====================================================
// PIN DEFINITIONS
// =====================================================

#define WATER_SENSOR 2
#define PUMP_LED     3


// =====================================================
// VARIABLES
// =====================================================

bool pumpState = false;

bool timerStarted = false;

unsigned long waterOffTime = 0;


// Time when water supply started

DateTime waterStartTime;


// =====================================================
// SETUP
// =====================================================

void setup()
{
  pinMode(WATER_SENSOR, INPUT_PULLUP);

  pinMode(PUMP_LED, OUTPUT);

  digitalWrite(PUMP_LED, LOW);

  Wire.begin();

  rtc.begin();


  // Uncomment first time only
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


  // OLED START

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();

  delay(1000);
}



// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  DateTime now = rtc.now();

  bool waterAvailable = digitalRead(WATER_SENSOR);


  // =================================================
  // WATER AVAILABLE
  // =================================================

  if (waterAvailable == HIGH)
  {
    // Turn Pump ON

    digitalWrite(PUMP_LED, HIGH);

    pumpState = true;


    // Start Timer Only Once

    if (!timerStarted)
    {
      waterStartTime = now;

      timerStarted = true;
    }


    // Calculate Time Difference

    TimeSpan elapsed = now - waterStartTime;


    // If Water Supply Continues More Than 1 Hour

    if (elapsed.totalseconds() >= 3600)
    {
      // Alarm or additional logic can be added here

      digitalWrite(PUMP_LED, LOW);

      // Example:
      // Pump continues ON

    }
  }



  // =================================================
  // WATER NOT AVAILABLE
  // =================================================

  else
  {
    timerStarted = false;


    // Wait 2 seconds before OFF

    delay(2000);

    digitalWrite(PUMP_LED, LOW);

    pumpState = false;
  }



  // =================================================
  // UPDATE OLED DISPLAY
  // =================================================

  displayData(now, waterAvailable);


  delay(200);
}



// =====================================================
// OLED DISPLAY FUNCTION
// =====================================================

void displayData(DateTime now, bool waterAvailable)
{
  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(WHITE);



  // =================================================
  // REAL TIME
  // =================================================

  display.setCursor(0, 0);

  display.println("REAL TIME");

  display.setTextSize(2);

  display.setCursor(0, 12);

  print2digit(now.hour());

  display.print(":");

  print2digit(now.minute());

  display.print(":");

  print2digit(now.second());



  // =================================================
  // WATER STATUS
  // =================================================

  display.setTextSize(1);

  display.setCursor(0, 40);

  display.print("WATER: ");

  if (waterAvailable)
  {
    display.println("AVAILABLE");
  }
  else
  {
    display.println("OFF");
  }



  // =================================================
  // PUMP STATUS
  // =================================================

  display.setCursor(0, 54);

  display.print("PUMP : ");

  if (pumpState)
  {
    display.println("ON");
  }
  else
  {
    display.println("OFF");
  }


  display.display();
}



// =====================================================
// PRINT 2 DIGIT NUMBER
// =====================================================

void print2digit(int number)
{
  if (number < 10)
  {
    display.print("0");
  }

  display.print(number);
}
