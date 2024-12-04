// sensor.h
#include "DHT.h"

#define DHTPIN 4      // Digital pin 4
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Initialisera DHT-sensorn
void initSensor() {
  dht.begin();
}

// Hämta temperatur
float getTemperature() {
  return dht.readTemperature();
}

// Hämta luftfuktighet
float getHumidity() {
  return dht.readHumidity();
}
