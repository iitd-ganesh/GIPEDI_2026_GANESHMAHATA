#define led 12    // LED connected to digital pin 12

void setup() {
    // Configure LED pin as an output
    pinMode(led, OUTPUT);
}

void loop() {
    // Continuously toggle the LED ON and OFF
    toggle_led();
}

// Function to blink the LED
void toggle_led()
{
    // Turn LED ON
    digitalWrite(led, HIGH);
    delay(1000);          // Keep LED ON for 1 second

    // Turn LED OFF
    digitalWrite(led, LOW);
    delay(1000);          // Keep LED OFF for 1 second
}