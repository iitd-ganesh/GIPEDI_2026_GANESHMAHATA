#include <DHT.h>

#define DHTPIN 1   //Pin 1 for DHT11 Sensor
#define DHTTYPE DHT11
#define BUZZER 8    //Pin 8 for Buzzer
#define THRESHOLD 50    

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    pinMode(BUZZER, OUTPUT);
    dht.begin();
}

void loop()
{
    float temp = dht.readTemperature();

    if (temp >= THRESHOLD)  //Condition statement for Problem stat. 
        digitalWrite(BUZZER, HIGH);
    else
        digitalWrite(BUZZER, LOW);

    delay(1000);
}