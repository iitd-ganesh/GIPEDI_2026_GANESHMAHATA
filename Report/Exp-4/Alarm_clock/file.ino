/*
   ============================================
        Experiment - 4 : Alarm Clock System
   ============================================

   Components Used:
   ----------------
   Arduino UNO
   DS1307 RTC Module
   SSD1306 OLED Display (I2C)
   3 Push Buttons
   Buzzer

   Pin Connections:
   ----------------
   P-adj  -> D1   (Mode Change)
   P+ve   -> D2   (Increase Value)
   P-ve   -> D3   (Decrease Value)

   Buzzer -> D12

   OLED + RTC:
   SDA -> A4
   SCL -> A5

   ============================================
   OPERATION
   ============================================

   P-adj Button Sequence:
   ----------------------
   1st Push -> Activate Alarm Setting Mode
   2nd Push -> Set Hour
   3rd Push -> Set Minute
   4th Push -> Set Second
   5th Push -> Exit Alarm Setting Mode

   P+ve Button:
   ------------
   Increase selected value

   P-ve Button:
   ------------
   Decrease selected value

   Alarm:
   -------
   When Alarm Time == RTC Time
   Buzzer will ring

   ============================================
*/


#include <Wire.h>
#include <RTClib.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ================= OLED SETTINGS =================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ================= RTC OBJECT ====================

RTC_DS1307 rtc;


// ================= BUTTON PINS ===================

#define BTN_ADJ 1
#define BTN_INC 2
#define BTN_DEC 3


// ================= BUZZER PIN ====================

#define BUZZER 12


// ================= ALARM VARIABLES ===============

int alarmHour = 0;
int alarmMinute = 0;
int alarmSecond = 0;


// ================= MODE VARIABLES ================

/*
   mode = 0 -> Normal display
   mode = 1 -> Set Hour
   mode = 2 -> Set Minute
   mode = 3 -> Set Second
*/

int mode = 0;

bool alarmSettingActive = false;


// ================= BUTTON STATE ==================

bool lastAdjState = HIGH;
bool lastIncState = HIGH;
bool lastDecState = HIGH;


// =================================================
//                  SETUP
// =================================================

void setup()
{
  pinMode(BTN_ADJ, INPUT_PULLUP);
  pinMode(BTN_INC, INPUT_PULLUP);
  pinMode(BTN_DEC, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);

  Wire.begin();

  rtc.begin();

  // Uncomment only first time to set RTC time
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));



  // OLED START

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();

  delay(1000);
}



// =================================================
//                  MAIN LOOP
// =================================================

void loop()
{
  DateTime now = rtc.now();

  handleButtons();

  checkAlarm(now);

  displayData(now);

  delay(150);
}



// =================================================
//            HANDLE BUTTON OPERATIONS
// =================================================

void handleButtons()
{
  bool adjState = digitalRead(BTN_ADJ);
  bool incState = digitalRead(BTN_INC);
  bool decState = digitalRead(BTN_DEC);



  // =========================================
  // P-ADJ BUTTON
  // =========================================

  if (adjState == LOW && lastAdjState == HIGH)
  {
    delay(200);

    mode++;

    // 1st Push
    if (mode == 1)
    {
      alarmSettingActive = true;
    }

    // 5th Push
    if (mode > 4)
    {
      mode = 0;
      alarmSettingActive = false;
    }
  }



  // =========================================
  // P+ BUTTON
  // =========================================

  if (incState == LOW && lastIncState == HIGH)
  {
    delay(200);

    if (mode == 1)
    {
      alarmHour++;

      if (alarmHour > 23)
        alarmHour = 0;
    }

    else if (mode == 2)
    {
      alarmMinute++;

      if (alarmMinute > 59)
        alarmMinute = 0;
    }

    else if (mode == 3)
    {
      alarmSecond++;

      if (alarmSecond > 59)
        alarmSecond = 0;
    }
  }



  // =========================================
  // P- BUTTON
  // =========================================

  if (decState == LOW && lastDecState == HIGH)
  {
    delay(200);

    if (mode == 1)
    {
      alarmHour--;

      if (alarmHour < 0)
        alarmHour = 23;
    }

    else if (mode == 2)
    {
      alarmMinute--;

      if (alarmMinute < 0)
        alarmMinute = 59;
    }

    else if (mode == 3)
    {
      alarmSecond--;

      if (alarmSecond < 0)
        alarmSecond = 59;
    }
  }



  // SAVE BUTTON STATES

  lastAdjState = adjState;
  lastIncState = incState;
  lastDecState = decState;
}



// =================================================
//               CHECK ALARM
// =================================================

void checkAlarm(DateTime now)
{
  // Alarm Match
  if (
    now.hour() == alarmHour &&
    now.minute() == alarmMinute &&
    now.second() >= alarmSecond
  )
  {
    tone(BUZZER, 1000);
  }
  else
  {
    noTone(BUZZER);
  }
}



// =================================================
//               DISPLAY FUNCTION
// =================================================

void displayData(DateTime now)
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);



  // =========================================
  // REAL TIME
  // =========================================

  display.setCursor(0, 0);
  display.println("REAL TIME");

  display.setTextSize(2);
  display.setCursor(0, 12);

  print2digit(now.hour());
  display.print(":");

  print2digit(now.minute());
  display.print(":");

  print2digit(now.second());



  // =========================================
  // ALARM TIME
  // =========================================

  display.setTextSize(1);

  display.setCursor(0, 40);
  display.println("ALARM");

  display.setTextSize(2);

  display.setCursor(0, 50);

  print2digit(alarmHour);
  display.print(":");

  print2digit(alarmMinute);
  display.print(":");

  print2digit(alarmSecond);



  // =========================================
  // MODE DISPLAY
  // =========================================

  display.setTextSize(1);

  display.setCursor(90, 0);

  if (mode == 0)
  {
    display.println("NORMAL");
  }

  else if (mode == 1)
  {
    display.println("SET HR");
  }

  else if (mode == 2)
  {
    display.println("SET MIN");
  }

  else if (mode == 3)
  {
    display.println("SET SEC");
  }



  display.display();
}



// =================================================
//         PRINT TWO DIGIT NUMBER
// =================================================

void print2digit(int number)
{
  if (number < 10)
  {
    display.print("0");
  }

  display.print(number);
}