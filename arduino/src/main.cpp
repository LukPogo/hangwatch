#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "Telefon MI";
const char* password = "Barcelona1";

const char* serverAddress = "http://192.168.43.212:5000/get_gpio_state";

void setup(){
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    
}

void loop() {
    // Odczytaj stan GPIO
   int gpioState = 10 ;
    DynamicJsonDocument jsonDoc(200);
    jsonDoc["gpioState"] = gpioState;
    String payload;
    serializeJson(jsonDoc,payload);
    // Wyślij żądanie POST na serwer Flask
    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "application/json");
     int httpResponseCode = http.sendRequest("GET", payload);
    if(httpResponseCode > 0)
    {
        Serial.printf("HTTP Response code: %d\n",httpResponseCode );
        String response = http.getString();
        DynamicJsonDocument jsonDoc(200);
        deserializeJson(jsonDoc, response);
        String success = jsonDoc["success"].as<String>();
        Serial.println(response);

    }
    else
    {
        Serial.printf("HTTP Request failed %s\n", http.errorToString(httpResponseCode).c_str());

    }
  //Serial.print(code);
    // Poczekaj przez określony czas
    http.end();
    delay(1000);
}
