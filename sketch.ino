#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define suhu 14
#define DHTTYPE DHT22
DHT dht(suhu, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Your server URL
const char* serverUrl = "http://192.168.1.6:5000/data";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  pinMode(suhu, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.... ");
}

void loop() {
  float defsuhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  if (isnan(defsuhu) || isnan(kelembapan)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Suhu : ");
  Serial.print(defsuhu);
  Serial.println(" C");

  Serial.print("kelembapan : ");
  Serial.print(kelembapan);
  Serial.println(" %");


  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"Suhu\":" + String(defsuhu) + ",\"kelembapan\":" + String(kelembapan) + ",}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  Serial.println("----------------------------------------------");
  // Send a request every 10 seconds
  delay(10000);


}
