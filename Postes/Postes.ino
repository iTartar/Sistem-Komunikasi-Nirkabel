#include "DHT.h"
#include <WiFi.h>
#include "ThingSpeak.h"

#define DHTPIN 4
#define DHTTYPE DHT11

// Ganti dengan API Keys anda di Thingspeak
#define CHANNEL_ID 3184524
#define CHANNEL_API_KEY "84D7K53XXDMHW50K"

const char* ssid = "Infinix HOT 10S";
const char* password = "aqilmbohh";
const char* server = "api.thingspeak.com";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Deteksi Suhu");
  dht.begin();
  
  // Initial WiFi
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  ThingSpeak.begin(client);
}

void loop() {
  float t = dht.readTemperature();
  
  if (isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }
  
  if (client.connect(server, 80)) {
    ThingSpeak.setField(1, t);
    
    int x = ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
    
    if (x == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" °C send to Thingspeak");
  }
  
  client.stop();
  Serial.println("Waiting…");
  delay(20000);
}