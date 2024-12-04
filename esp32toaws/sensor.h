#include "DHT.h"

#define DHTPIN 4      // Digital pin 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Initialize DHT sensor
void initSensor() {
  dht.begin();
}

//Function to get temperature
float getTemperature() {
  return dht.readTemperature();
}

//Function to get humidity
float getHumidity() {
  return dht.readHumidity();
}
