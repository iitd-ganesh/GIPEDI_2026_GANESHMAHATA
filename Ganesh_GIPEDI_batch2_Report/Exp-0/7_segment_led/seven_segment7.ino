// 7-Segment Display with Arduino UNO
// Display Type: Common Cathode

// Segment pin connections
int a = 2;   // Segment A
int b = 3;   // Segment B
int c = 4;   // Segment C
int d = 5;   // Segment D
int e = 6;   // Segment E
int f = 7;   // Segment F
int g = 8;   // Segment G

void setup() {

  // Configure all segment pins as outputs
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

}

void loop() {

  // Display digit 0
  // Segments ON: A, B, C, D, E, F
  // Segment OFF: G
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);

  delay(1000);   // Hold digit for 1 second

  // Display digit 1
  // Segments ON: B, C
  // Segments OFF: A, D, E, F, G
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);

  delay(1000);

  // Display digit 2
  // Segments ON: A, B, D, E, G
  // Segments OFF: C, F
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);

  delay(1000);

  // Display digit 3
  // Segments ON: A, B, C, D, G
  // Segments OFF: E, F
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);

  delay(1000);

}