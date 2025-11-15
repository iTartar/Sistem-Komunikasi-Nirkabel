#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Deteksi Suhu dan Kelembaban");
  dht.begin();
}

void loop() {
  delay(2000);
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
}