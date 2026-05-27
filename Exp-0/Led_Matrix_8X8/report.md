# **EXPERIMENT – 0 GO FURTHER – 3**

## PROJECT NAME

**LED Matrix Emoji Display System**

---

## OBJECTIVE / PROBLEM STATEMENT

The objective of this project is to interface an 8×8 LED matrix with Arduino UNO and display different emoji patterns.

This experiment helps in understanding:
- LED matrix interfacing
- Row and column scanning
- Pattern generation
- Multiplexing technique

---

## COMPONENTS USED

| Component | Quantity |
|---|---|
| Arduino UNO | 1 |
| 8×8 LED Matrix | 1 |
| 125Ω Resistors | 8 |
| Jumper Wires | Required |
| Breadboard | 1 |

---

## PIN CONNECTIONS

| LED Matrix | Arduino Pin |
|---|---|
| Row 1 | D1 |
| Row 2 | D2 |
| Row 3 | D3 |
| Row 4 | D4 |
| Row 5 | D5 |
| Row 6 | D6 |
| Row 7 | D7 |
| Row 8 | D8 |
| Column 1 | A5 |
| Column 2 | A4 |
| Column 3 | A3 |
| Column 4 | A2 |
| Column 5 | D9 |
| Column 6 | D10 |
| Column 7 | D11 |
| Column 8 | D12 |

---

## SOFTWARE USED

- Arduino IDE
- Proteus / SimulIDE

---

## CIRCUIT DIAGRAM

<img src="./Led_ Matrix_ Circuit.png" width="300">

---

## MAIN ARDUINO CODE

```cpp
byte rows[8] = {1,2,3,4,5,6,7,8};
byte cols[8] = {A5,A4,A3,A2,9,10,11,12};

// Emoji Patterns
byte emoji[3][8] = {

  // Smiley Face
  {
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100
  },

  // Sad Face
  {
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100
  }
};

void setup() {

  for(int i=0; i<8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }
}

void displayEmoji(int num) {

  for(int t=0; t<250; t++) {

    for(int r=0; r<8; r++) {

      // OFF all rows
      for(int i=0; i<8; i++) {
        digitalWrite(rows[i], LOW);
      }

      // OFF all columns
      for(int i=0; i<8; i++) {
        digitalWrite(cols[i], HIGH);
      }

      // Activate current row
      digitalWrite(rows[r], HIGH);

      // Display pattern
      for(int c=0; c<8; c++) {

        if(bitRead(emoji[num][r], 7-c)) {
          digitalWrite(cols[c], LOW);
        }
      }

      delay(2);
    }
  }
}

void loop() {

  displayEmoji(0); // Smiley
  //displayEmoji(1); // Heart
  displayEmoji(2); // Sad
}
```

---

## WORKING PRINCIPLE

1. The 8×8 LED matrix is connected to Arduino UNO using row and column connections.

2. Arduino scans rows and columns rapidly using multiplexing.

3. Binary patterns are stored inside the `emoji` array.

4. The `displayEmoji()` function activates rows one by one and controls columns according to the stored pattern.

5. Different emoji patterns are displayed continuously on the LED matrix.

6. Due to fast scanning, the complete image appears continuously visible.

---

## WHAT I LEARN

- Interfacing an LED matrix with Arduino UNO
- Multiplexing technique
- Row and column scanning
- Pattern generation using binary values
- Arduino digital output voltage is approximately 5V
- Recommended current per pin is 20mA
- LED matrix LEDs require current limiting resistors

---

## ADVANTAGES

- Can display multiple patterns
- Low-cost display system
- Useful for learning multiplexing
- Simple animation implementation

---

## APPLICATIONS

- LED display boards
- Emoji displays
- Scrolling text systems
- Digital signboards
- Embedded display projects

---

## CONCLUSION

The LED matrix was successfully interfaced with Arduino UNO.

Different emoji patterns were displayed by controlling rows and columns using multiplexing techniques. This project helped in understanding matrix displays and pattern generation in embedded systems.

---

## REFERENCES

1. Arduino Official Website  
https://www.arduino.cc/

2. Arduino UNO Documentation  
https://docs.arduino.cc/hardware/uno-rev3/

3. LED Matrix Basics  
https://www.electronicsforu.com/resources/learn-electronics/8x8-led-matrix-module

4. Arduino Tutorials  
https://www.arduino.cc/en/Tutorial/HomePage