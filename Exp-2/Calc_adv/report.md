**EXPERIMENT – 2 (GO FURTHER – 1)**

## PROJECT NAME

**BODMAS Method Based Calculator**

---

## OBJECTIVE

To design a calculator using Arduino UNO, OLED display, and keypad that performs arithmetic calculations according to the BODMAS rule.

---

## COMPONENTS USED

| Component | Quantity |
|---|---|
| Arduino UNO | 1 |
| SSD1306 OLED Display | 1 |
| 5×4 Matrix Keypad | 1 |
| Breadboard | 1 |
| Jumper Wires | Required |

---

## PIN CONNECTIONS

### OLED

| OLED Pin | Arduino Pin |
|---|---|
| SDA | A4 |
| SCL | A5 |

---

### Keypad

| Keypad Pin | Arduino Pin |
|---|---|
| R1–R5 | D2–D6 |
| C1–C4 | D7–D10 |

---

## KEYPAD FUNCTIONS

| Key | Function |
|---|---|
| A | + |
| S | - |
| M | * |
| D | / |
| = | Calculate |
| # | Clear |
| B | Backspace |
| H | History |

---

## SOFTWARE USED

- Arduino IDE
- SimulIDE

---

## CIRCUIT DIAGRAM

<image src='./circuit1.png' width='205'>
<image src='./circuit2.png' width='200'>




---

## MAIN ARDUINO CODE

```cpp
//======================================================
// BODMAS Calculator using Arduino UNO
// OLED Display + 5x4 Keypad
//
// Features:
// - Addition
// - Subtraction
// - Multiplication
// - Division
// - Brackets
// - History
// - Backspace
// - BODMAS Rule
//======================================================


//==================== LIBRARIES ====================

// I2C Communication Library
#include <Wire.h>

// OLED Graphics Library
#include <Adafruit_GFX.h>

// OLED SSD1306 Library
#include <Adafruit_SSD1306.h>

// Matrix Keypad Library
#include <Keypad.h>


//==================== OLED SETTINGS ====================

// OLED width
#define SCREEN_WIDTH 128

// OLED height
#define SCREEN_HEIGHT 64

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//==================== KEYPAD SETTINGS ====================

// Number of rows in keypad
const byte ROWS = 5;

// Number of columns in keypad
const byte COLS = 4;


// Keypad button layout
char keys[ROWS][COLS] = {

  {'1','2','3','A'},
  {'4','5','6','S'},
  {'7','8','9','M'},
  {'0','=','#','D'},
  {'(',')','H','B'}
};


// Arduino pins connected to keypad rows
byte rowPins[ROWS] = {2,3,4,5,6};

// Arduino pins connected to keypad columns
byte colPins[COLS] = {7,8,9,10};


// Create keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


//==================== VARIABLES ====================

// Stores current mathematical expression
String expression = "";

// Pointer used by parser
const char* expr;


// Stores last 5 calculations
String history[5];

// Current history index
int historyIndex = 0;


//==================== OLED DISPLAY FUNCTION ====================

// Function to refresh OLED screen
void refreshDisplay() {

  // Clear OLED screen
  display.clearDisplay();

  // Set text size
  display.setTextSize(1);

  // Set text color
  display.setTextColor(SSD1306_WHITE);

  // Title
  display.setCursor(0,0);
  display.print("BODMAS CALC");

  // Expression display position
  display.setCursor(0,20);

  // Display current expression
  display.print(expression);

  // Small delay for stable Proteus simulation
  delay(50);

  // Show everything on OLED
  display.display();
}


//==================== PARSER FUNCTIONS ====================

// Skip blank spaces
void skipSpaces() {

  while(*expr == ' ')
    expr++;
}


// Forward declaration
double parseExpression();


//==================== NUMBER PARSER ====================

// Reads numbers from expression
double parseNumber() {

  double number = 0;

  // Continue while digit found
  while(isdigit(*expr)) {

    // Build number digit by digit
    number = number * 10 + (*expr - '0');

    expr++;
  }

  return number;
}


//==================== FACTOR PARSER ====================

// Handles brackets
double parseFactor() {

  // Ignore spaces
  skipSpaces();

  // Check for opening bracket
  if(*expr == '(') {

    // Move to next character
    expr++;

    // Solve expression inside bracket
    double result = parseExpression();

    // Skip closing bracket
    if(*expr == ')')
      expr++;

    return result;
  }

  // Otherwise parse normal number
  return parseNumber();
}


//==================== TERM PARSER ====================

// Handles multiplication and division
double parseTerm() {

  // Read first factor
  double result = parseFactor();

  while(true) {

    skipSpaces();

    // Multiplication
    if(*expr == '*') {

      expr++;

      result *= parseFactor();
    }

    // Division
    else if(*expr == '/') {

      expr++;

      result /= parseFactor();
    }

    else {
      break;
    }
  }

  return result;
}


//==================== EXPRESSION PARSER ====================

// Handles addition and subtraction
double parseExpression() {

  // Read first term
  double result = parseTerm();

  while(true) {

    skipSpaces();

    // Addition
    if(*expr == '+') {

      expr++;

      result += parseTerm();
    }

    // Subtraction
    else if(*expr == '-') {

      expr++;

      result -= parseTerm();
    }

    else {
      break;
    }
  }

  return result;
}


//==================== EVALUATE FUNCTION ====================

// Evaluates full mathematical expression
double evaluate(String s) {

  // Convert String to character pointer
  expr = s.c_str();

  // Start parsing expression
  return parseExpression();
}


//==================== SETUP ====================

void setup() {

  // Start serial monitor
  Serial.begin(9600);

  // Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear OLED
  display.clearDisplay();

  // Text size
  display.setTextSize(1);

  // Text color
  display.setTextColor(SSD1306_WHITE);

  // Cursor position
  display.setCursor(0,20);

  // Startup message
  display.print("READY");

  // Show message
  display.display();

  // Wait 2 seconds
  delay(2000);

  // Show empty calculator screen
  refreshDisplay();
}


//==================== MAIN LOOP ====================

void loop() {

  // Read keypad button
  char key = keypad.getKey();

  // If any key pressed
  if(key) {

    // Print pressed key on serial monitor
    Serial.print("Key: ");
    Serial.println(key);


    //================ CALCULATE =================

    // If "=" pressed
    if(key == '=') {

      // Evaluate expression
      double result = evaluate(expression);

      // Save calculation in history
      history[historyIndex] =
      expression + "=" + String(result);

      // Move to next history slot
      historyIndex++;

      // Reset history index
      if(historyIndex >= 5) {

        historyIndex = 0;
      }

      // Replace expression with result
      expression = String(result);

      // Update OLED
      refreshDisplay();
    }


    //================ CLEAR SCREEN =================

    // If "#" pressed
    else if(key == '#') {

      // Clear expression
      expression = "";

      // Refresh OLED
      refreshDisplay();
    }


    //================ BACKSPACE =================

    // If "B" pressed
    else if(key == 'B') {

      // Remove last character
      if(expression.length() > 0) {

        expression.remove(expression.length()-1);
      }

      // Refresh OLED
      refreshDisplay();
    }


    //================ HISTORY =================

    // If "H" pressed
    else if(key == 'H') {

      // Clear OLED
      display.clearDisplay();

      // OLED settings
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);

      // Title
      display.setCursor(0,0);
      display.print("HISTORY");

      int y = 12;

      // Show saved history
      for(int i=0; i<5; i++) {

        if(history[i] != "") {

          display.setCursor(0,y);

          display.print(history[i]);

          y += 10;
        }
      }

      // Show on OLED
      display.display();

      // Wait 4 seconds
      delay(4000);

      // Return to calculator screen
      refreshDisplay();
    }


    //================ NORMAL INPUT =================

    else {

      // Convert keypad symbols

      if(key == 'A')
        expression += '+';

      else if(key == 'S')
        expression += '-';

      else if(key == 'M')
        expression += '*';

      else if(key == 'D')
        expression += '/';

      // Add normal characters
      else
        expression += key;

      // Refresh OLED
      refreshDisplay();
    }
  }
}
```

---

## WORKING PRINCIPLE

1. User enters expression using keypad.
2. Arduino receives keypad input.
3. OLED displays the expression.
4. On pressing `=`, Arduino evaluates the expression using BODMAS rule.
5. Result is displayed on OLED.
6. Previous calculations are stored in history.

---

## OUTPUT / RESULT

### Example

Input:

```text
(8+2)*5-20/4
```

Output:

```text
45
```

The calculator successfully performed calculations according to the BODMAS rule.

---

## ADVANTAGES

- Supports BODMAS calculations
- Compact and portable
- Real-time OLED display
- History and backspace support

---

## APPLICATIONS

- Educational projects
- Scientific calculator systems
- Arduino learning

---

## CONCLUSION

The BODMAS calculator using Arduino UNO was successfully implemented. The calculator correctly evaluated mathematical expressions and displayed the results on the OLED display.

---

## REFERENCES

1. https://www.arduino.cc/

2. https://docs.arduino.cc/hardware/uno-rev3/

3. https://github.com/adafruit/Adafruit_SSD1306

4. https://playground.arduino.cc/Code/Keypad/

5. https://www.circuitbasics.com/how-to-set-up-a-keypad-on-an-arduino/