#define led 12

void setup() {
    pinMode(led, OUTPUT); // put your setup code here, to run once:
}

void loop() {
    toggle_led(); // put your main code here, to run repeatedly:
}

void toggle_led()
{
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
}