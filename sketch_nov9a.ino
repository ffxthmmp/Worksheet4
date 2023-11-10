#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

// ใช้รหัส wifi ของเพื่อนเนื่องจากของหนูไม่สามารถเชื่อมได้ค่ะ 
const char* ssid = "nntdy";
const char* password = "123456789";
const char* serverName = "http://api.thingspeak.com/update?api_key=JLN0ASKM5US4SSA1";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;


DHT dht(D4, DHT11);     // Define the DHT sensor pin and type

void setup() {
  Serial.begin(115200);
  dht.begin();
 // Set the LED pin as an output

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status() == WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      // Read temperature and humidity from DHT sensor
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      if (!isnan(temperature) && !isnan(humidity)) {
        String serverPath = String(serverName) + "&field1=" + String(temperature) + "&field2=" + String(humidity);
        http.begin(client, serverPath.c_str());
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);

       
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
      }
      else {
        Serial.println("Failed to read data from DHT sensor");
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
