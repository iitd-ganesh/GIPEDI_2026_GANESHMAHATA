
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
  displayEmoji(1); // Heart
  displayEmoji(2); // Sad
}
