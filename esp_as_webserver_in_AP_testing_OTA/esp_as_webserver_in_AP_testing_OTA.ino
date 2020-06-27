#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
//used python 2.7.18
const byte led = 2;

const char *ssid = "baman-baman";
const char *password = "dobaarbaman";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);
  Serial.print("Access point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address :\t");
  Serial.println(WiFi.softAPIP());

  ArduinoOTA.setHostname("ESP8266");
  ArduinoOTA.setPassword("0502003019");

  ArduinoOTA.onStart([](){
    Serial.println("Start");
    });

  ArduinoOTA.onEnd([](){
    Serial.println("\n End");
    });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress: %u%%\r", (progress/(total/100)));
    });

  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    
    ArduinoOTA.begin();
    Serial.println("OTA ready");

    pinMode(led, OUTPUT);
    digitalWrite(led, 1);
}

unsigned long previousTime = millis();

const unsigned long interval = 100;

void loop() {
  ArduinoOTA.handle();
  unsigned long diff = millis() - previousTime;
  if(diff > interval) {
    digitalWrite(led, !digitalRead(led)); // Change the state of the LED
    previousTime += diff;
  }
}
