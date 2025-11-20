#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"
 
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Initialize Wifi connection to the router
char ssid[] = "SSID WIFI";
char password[] = "PASSWORD WIFI";
 
// Initialize Telegram BOT
#define BOTtoken "BOT API TELEGRAM"
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
 
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
 
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 
    //Cek Pembacaan Sensor DHT11
    if (text == "/statussuhu") {
      int t = dht.readTemperature();
      String temp = "Suhu saat ini : ";
      temp += int(t);
      temp +=" °C\n";
      bot.sendMessage(chat_id,temp, "");
    }
    
    if (text == "/statuskelembapan") {
      int h = dht.readHumidity();
      String temp = "Kelembaban: ";
      temp += int(h);
      temp += " %";
      bot.sendMessage(chat_id,temp, "");
    }
 
    //Cek Command untuk setiap aksi
    if (text == "/start") {
      String welcome = "Welcome  " + from_name + ".\n";
      welcome += "/statussuhu : Status Suhu\n";
      welcome += "/statuskelembapan : Status Kelembapan\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
 
void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // This is the simplest way of getting this working
  // if you are passing sensitive information, or controlling
  // something important, please either use certStore or at
  // least client.setFingerPrint
  client.setInsecure();
 
  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(500);
 
  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int t = dht.readTemperature();
  int h = dht.readHumidity();
  
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
 
    lastTimeBotRan = millis();
  }
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" Celcius Humidity: ");
  Serial.println(h);
  delay(3000);
}