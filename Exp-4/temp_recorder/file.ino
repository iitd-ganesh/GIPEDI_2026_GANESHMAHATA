/*
   =====================================================
        TEMPERATURE LOGGER EVERY 15 MINUTES
   =====================================================

   FEATURES
   -----------------------------------------------------
   1. Reads temperature from DHT11
   2. Displays time and temperature on OLED
   3. Records temperature every 15 minutes
   4. Stores data in Arduino EEPROM
   5. Transfers data to Laptop through USB Serial

   -----------------------------------------------------
   COMPONENTS
   -----------------------------------------------------
   Arduino UNO
   DHT11 Sensor
   DS1307 RTC Module
   SSD1306 OLED Display

   -----------------------------------------------------
   CONNECTIONS
   -----------------------------------------------------

   DHT11 DATA  -> D2

   OLED SDA    -> A4
   OLED SCL    -> A5

   RTC SDA     -> A4
   RTC SCL     -> A5

*/


#include <Wire.h>

#include <RTClib.h>

#include <EEPROM.h>

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
// DHT SETTINGS
// =====================================================

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);



// =====================================================
// EEPROM VARIABLES
// =====================================================

int eepromAddress = 0;

int lastRecordedMinute = -1;



// =====================================================
// SETUP
// =====================================================

void setup()
{
  // SERIAL COMMUNICATION

  Serial.begin(9600);



  // START I2C

  Wire.begin();



  // START RTC

  rtc.begin();



  // START DHT11

  dht.begin();



  // Uncomment only first time to set RTC

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));



  // START OLED

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }



  display.clearDisplay();

  display.display();

  delay(1000);



  // SERIAL HEADER

  Serial.println("================================");

  Serial.println("TEMPERATURE LOGGER STARTED");

  Serial.println("TIME,TEMP");

  Serial.println("================================");
}



// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // GET CURRENT TIME

  DateTime now = rtc.now();



  // READ TEMPERATURE

  float temperature = dht.readTemperature();



  // =================================================
  // DISPLAY DATA ON OLED
  // =================================================

  displayData(now, temperature);



  // =================================================
  // RECORD EVERY 15 MINUTES
  // =================================================

  /*
      Records at:
      00 min
      15 min
      30 min
      45 min
  */


  if (
      now.minute() % 1 == 0 &&
      now.minute() != lastRecordedMinute
     )
  {

      // ============================================
      // STORE DATA INTO EEPROM
      // ============================================

      // STORE HOUR

      EEPROM.write(eepromAddress, now.hour());

      eepromAddress++;



      // STORE MINUTE

      EEPROM.write(eepromAddress, now.minute());

      eepromAddress++;



      // STORE TEMPERATURE

      EEPROM.write(eepromAddress, (int)temperature);

      eepromAddress++;



      // ============================================
      // SEND DATA TO LAPTOP THROUGH USB SERIAL
      // ============================================

      Serial.print(now.hour());

      Serial.print(":");

      print2digitSerial(now.minute());

      Serial.print(":");

      print2digitSerial(now.second());

      Serial.print(",");

      Serial.print(temperature);

      Serial.println(" C");



      // SAVE LAST RECORDED MINUTE

      lastRecordedMinute = now.minute();
  }



  // SMALL REFRESH DELAY

  delay(1000);
}



// =====================================================
// OLED DISPLAY FUNCTION
// =====================================================

void displayData(DateTime now, float temperature)
{
  // CLEAR OLED

  display.clearDisplay();



  // TEXT SETTINGS

  display.setTextSize(1);

  display.setTextColor(WHITE);



  // =================================================
  // TITLE
  // =================================================

  display.setCursor(0, 0);

  display.println("TEMP LOGGER");



  // =================================================
  // TIME DISPLAY
  // =================================================

  display.setCursor(0, 15);

  display.print("TIME: ");



  print2digit(now.hour());

  display.print(":");

  print2digit(now.minute());

  display.print(":");

  print2digit(now.second());



  // =================================================
  // TEMPERATURE DISPLAY
  // =================================================

  display.setCursor(0, 35);

  display.print("TEMP: ");

  display.print(temperature);

  display.println(" C");



  // =================================================
  // EEPROM ADDRESS DISPLAY
  // =================================================

  display.setCursor(0, 55);

  display.print("ADDR: ");

  display.print(eepromAddress);



  // REFRESH OLED

  display.display();
}



// =====================================================
// PRINT 2 DIGIT FUNCTION FOR OLED
// =====================================================

void print2digit(int number)
{
  if (number < 10)
  {
    display.print("0");
  }

  display.print(number);
}



// =====================================================
// PRINT 2 DIGIT FUNCTION FOR SERIAL
// =====================================================

void print2digitSerial(int number)
{
  if (number < 10)
  {
    Serial.print("0");
  }

  Serial.print(number);
}