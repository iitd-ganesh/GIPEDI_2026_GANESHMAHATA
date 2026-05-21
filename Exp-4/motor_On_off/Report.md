# EXPERIMENT – 5 Alarm Based Motor ON/OFF System

---

# PROJECT NAME

Alarm Based Motor ON/OFF System Using Arduino UNO, DS1307 RTC Module and SSD1306 OLED Display

---

# OBJECTIVE / PROBLEM STATEMENT

To design and implement an automatic motor ON/OFF control system using Arduino UNO. The system detects water supply using a switch sensor and controls a pump indicator LED based on water availability.

The DS1307 RTC module is used for real-time tracking and timing operations. The SSD1306 OLED display continuously displays real time, water supply status, and pump status.

The system also checks whether water supply continues for more than one hour.

---

# COMPONENTS USED

| Component | Quantity |
|---|---|
| Arduino UNO | 1 |
| DS1307 RTC Module | 1 |
| SSD1306 OLED Display | 1 |
| Push Button / Water Sensor Switch | 1 |
| LED (Pump Indicator) | 1 |
| Jumper Wires | Required |
| Breadboard | 1 |

---

# PIN CONNECTIONS

## WATER SENSOR CONNECTION

| Device | Arduino Pin |
|---|---|
| Water Supply Switch | D2 |

---

## PUMP LED CONNECTION

| Device | Arduino Pin |
|---|---|
| Pump LED | D3 |

---

## DS1307 RTC CONNECTIONS

| RTC Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

---

## SSD1306 OLED DISPLAY CONNECTIONS

| OLED Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

---

# SOFTWARE USED

- Arduino IDE
- SimulIDE / Proteus

---

# CIRCUIT DIAGRAM
### When Water supply available
![Circuit_Diagram](/Exp-4/motor_On_off/Cicuit_On.png)

### when water supply is off
![](/Exp-4/motor_On_off/Circuit_Off.png)
---

# REQUIRED LIBRARIES

```cpp
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
```
---

# MAIN ARDUINO CODE

## Code With Comment ----> [Click Here](/Exp-4/motor_On_off/file.ino)

```cpp
/*
   =====================================================
      Alarm Based Motor ON/OFF System using RTC
   =====================================================
*/


#include <Wire.h>
#include <RTClib.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// OLED SETTINGS

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,
                         SCREEN_HEIGHT,
                         &Wire,
                         -1);


// RTC OBJECT

RTC_DS1307 rtc;


// PIN DEFINITIONS

#define WATER_SENSOR 2
#define PUMP_LED     3


// VARIABLES

bool pumpState = false;

bool timerStarted = false;


// Time when water supply started

DateTime waterStartTime;



// SETUP

void setup()
{
  pinMode(WATER_SENSOR, INPUT_PULLUP);

  pinMode(PUMP_LED, OUTPUT);

  digitalWrite(PUMP_LED, LOW);

  Wire.begin();

  rtc.begin();


  // Uncomment first time only
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



// MAIN LOOP

void loop()
{
  DateTime now = rtc.now();

  bool waterAvailable = digitalRead(WATER_SENSOR);


  // WATER AVAILABLE

  if (waterAvailable == LOW)
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


    // Check 1 Hour Condition

    if (elapsed.totalseconds() >= 3600)
    {
      // Additional logic can be added here
    }
  }



  // WATER NOT AVAILABLE

  else
  {
    timerStarted = false;


    // Wait 2 Seconds

    delay(2000);

    digitalWrite(PUMP_LED, LOW);

    pumpState = false;
  }



  // UPDATE OLED DISPLAY

  displayData(now, waterAvailable);


  delay(200);
}



// OLED DISPLAY FUNCTION

void displayData(DateTime now, bool waterAvailable)
{
  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(WHITE);



  // REAL TIME

  display.setCursor(0, 0);

  display.println("REAL TIME");

  display.setTextSize(2);

  display.setCursor(0, 12);

  print2digit(now.hour());

  display.print(":");

  print2digit(now.minute());

  display.print(":");

  print2digit(now.second());



  // WATER STATUS

  display.setTextSize(1);

  display.setCursor(0, 40);

  display.print("WATER: ");

  if (waterAvailable == LOW)
  {
    display.println("AVAILABLE");
  }
  else
  {
    display.println("OFF");
  }



  // PUMP STATUS

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



// PRINT 2 DIGIT NUMBER

void print2digit(int number)
{
  if (number < 10)
  {
    display.print("0");
  }

  display.print(number);
}
```

---

# WORKING PRINCIPLE

1. Water sensor continuously checks water supply.
2. If water is available:
   - Pump LED turns ON.
   - RTC timer starts.
3. If water supply continues for more than 1 hour:
   - Timer condition becomes true.
4. If water supply stops before 1 hour:
   - System waits 2 seconds.
   - Pump LED turns OFF.
5. OLED continuously displays:
   - Real Time
   - Water Supply Status
   - Pump Status

---

# OUTPUT

## OLED Display

```text
REAL TIME
12:45:30

WATER: AVAILABLE

PUMP : ON
```

---

## Water Supply OFF

```text
REAL TIME
12:46:10

WATER: OFF

PUMP : OFF
```

---

# FEATURES

- Automatic pump control
- Real-time clock display
- OLED status monitoring
- Water detection system
- 1-hour timing condition
- I2C communication
- Simple embedded automation system

---

# ADVANTAGES

- Easy to implement
- Low-cost project
- Real-time monitoring
- Automatic control system
- Useful for water management systems

---

# APPLICATIONS

- Water tank automation
- Motor protection systems
- Smart irrigation systems
- Industrial timing systems
- Embedded automation projects

---

# CONCLUSION

The Alarm Based Motor ON/OFF System using Arduino UNO, DS1307 RTC module, and SSD1306 OLED display was successfully implemented and tested. The system correctly detects water supply conditions, controls pump operation, and displays real-time status information on the OLED display.

During this experiment, I learned:
- RTC interfacing
- OLED interfacing
- I2C communication
- Digital input/output control
- Embedded automation logic

---

# REFERENCES

1. Arduino UNO Official Documentation  
https://docs.arduino.cc/hardware/uno-rev3/

2. DS1307 RTC Datasheet  
https://datasheets.maximintegrated.com/en/ds/DS1307.pdf

3. RTClib Library  
https://github.com/adafruit/RTClib

4. Adafruit SSD1306 Library  
https://github.com/adafruit/Adafruit_SSD1306

5. Adafruit GFX Library  
https://github.com/adafruit/Adafruit-GFX-Library

6. Arduino Wire Library Documentation  
https://www.arduino.cc/en/reference/wire