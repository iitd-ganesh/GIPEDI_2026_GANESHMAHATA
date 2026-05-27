#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= PINS =================
#define BUZZER    11
#define BLUE_LED  12
#define RED_LED   13

// ================= KEYPAD =================
const byte ROWS = 5;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'R', '0', 'X', 'D'},
  {'*', '@', '$', 'E'}
};

byte rowPins[ROWS] = {2, 3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 9, 10};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ================= VARIABLES =================
String adminPassword = "";
int attemptsLeft = 3;

// =====================================================

void showMessage(String line1, String line2 = "") {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 10);
  display.println(line1);

  display.setCursor(0, 30);
  display.println(line2);

  display.display();
}

// =====================================================

bool isValidPassword(String pass) {

  // Only 4 character password allowed

  if (pass.length() != 4)
    return false;

  return true;
}

// =====================================================

String readPassword() {

  String pass = "";

  while (pass.length() < 4) {

    char key = keypad.getKey();

    if (key) {

      tone(BUZZER, 1500, 80);

      pass += key;

      display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(WHITE);

      display.setCursor(0, 10);
      display.print("ENTER PASSWORD");

      display.setCursor(0, 30);

      for (int i = 0; i < pass.length(); i++) {
        display.print("*");
      }

      display.display();
    }
  }

  delay(500);

  return pass;
}

// =====================================================

void accessGranted() {

  digitalWrite(BLUE_LED, HIGH);

  tone(BUZZER, 2000, 300);

  showMessage("ACCESS GRANTED");

  delay(3000);

  digitalWrite(BLUE_LED, LOW);
}

// =====================================================

void accessDenied() {

  digitalWrite(RED_LED, HIGH);

  tone(BUZZER, 500, 500);

  showMessage(
    "WRONG PASSWORD",
    "Attempts Left: " + String(attemptsLeft)
  );

  delay(2000);

  digitalWrite(RED_LED, LOW);
}

// =====================================================

void alarmMode() {

  showMessage("SYSTEM LOCKED", "ALARM ACTIVE");

  while (1) {

    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 700);

    delay(300);

    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);

    delay(300);
  }
}

// =====================================================

void createPassword() {

  while (1) {

    showMessage(
      "CREATE PASSWORD",
      "Enter 4 Characters"
    );

    String newPass = readPassword();

    if (isValidPassword(newPass)) {

      adminPassword = newPass;

      tone(BUZZER, 2000, 300);

      showMessage("PASSWORD SAVED");

      delay(2000);

      break;
    }

    else {

      tone(BUZZER, 400, 1000);

      showMessage("INVALID PASSWORD");

      delay(2000);
    }
  }
}

// =====================================================

void resetPassword() {

  showMessage("RESET MODE", "Enter Old Pass");

  String oldPass = readPassword();

  // Correct old password
  if (oldPass == adminPassword) {

    tone(BUZZER, 2000, 300);

    showMessage("CORRECT", "Set New Password");

    delay(2000);

    while (1) {

      String newPass = readPassword();

      if (isValidPassword(newPass)) {

        adminPassword = newPass;

        tone(BUZZER, 2000, 500);

        showMessage("PASSWORD RESET");

        delay(2000);

        return;
      }

      else {

        tone(BUZZER, 400, 1000);

        showMessage("INVALID PASSWORD");

        delay(2000);
      }
    }
  }

  // Wrong old password
  else {

    tone(BUZZER, 300, 1000);

    showMessage("WRONG OLD PASS");

    delay(2000);
  }
}

// =====================================================

void setup() {

  pinMode(BUZZER, OUTPUT);

  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.display();

  // Create admin password
  createPassword();
}

// =====================================================

void loop() {

  showMessage(
    "ENTER PASSWORD",
    "Attempts Left: " + String(attemptsLeft)
  );

  String enteredPassword = "";

  while (enteredPassword.length() < 4) {

    char key = keypad.getKey();

    if (key) {

      // Reset password mode
      if (key == 'R') {

        resetPassword();

        return;
      }

      tone(BUZZER, 1500, 80);

      enteredPassword += key;

      display.clearDisplay();

      display.setTextSize(1);
      display.setTextColor(WHITE);

      display.setCursor(0, 10);
      display.print("ENTER PASSWORD");

      display.setCursor(0, 30);

      for (int i = 0; i < enteredPassword.length(); i++) {
        display.print("*");
      }

      display.display();
    }
  }

  // ================= CHECK PASSWORD =================

  if (enteredPassword == adminPassword) {

    attemptsLeft = 3;

    accessGranted();
  }

  else {

    attemptsLeft--;

    accessDenied();

    if (attemptsLeft <= 0) {

      alarmMode();
    }
  }
}       