#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define LED  33
#define BUTTON 25

const char* ssid = "Telefon MI";
const char* password = "Barcelona1";

const char* SERVER_ADDRESS = "http://192.168.43.212:5000/hooks";

const char* BOARD_ID = "1234";
const char* MIEJSCE = "warsztat 027";
void setup()
{
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
        
    }
    pinMode(LED,OUTPUT);
    pinMode(BUTTON,INPUT_PULLUP);
}
int send_status_request(int buttonState) 
{
    
    DynamicJsonDocument jsonDoc(200);
    jsonDoc["place"] =  MIEJSCE;
    if(buttonState==LOW)
    {
        jsonDoc["state"] = "hanged";
    }
    else
    {
        jsonDoc["state"]="empty";
    }
    jsonDoc["board_id"] = BOARD_ID;
    String payload;
    serializeJson(jsonDoc, payload);
    // Wyślij żądanie POST na serwer Flask
    HTTPClient http;
    http.begin(SERVER_ADDRESS);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.sendRequest("POST", payload);
    http.end();
    delay(1000);
    return httpResponseCode;
}


void loop() 
{   
    int buttonState = digitalRead(BUTTON);
    if(buttonState==LOW)   
    {
        digitalWrite(LED,HIGH);
    }
    else
    {
        digitalWrite(LED,LOW);
    }
    int httpResponseCode = send_status_request(buttonState);
    if(httpResponseCode !=200)
    {
        int buttonState = digitalRead(BUTTON);
        if(buttonState==LOW) 
        { 
            while (1) 
            {
                digitalWrite(LED, HIGH);
                delay(500);
                digitalWrite(LED, LOW);
                delay(500);
                buttonState = digitalRead(BUTTON);
                httpResponseCode = send_status_request(buttonState);
                if(httpResponseCode==200 )
                {
                    break;
                }
            }
            if (httpResponseCode==200)
            {
                digitalWrite(LED,HIGH);
            }
        }            
       
    }
}

