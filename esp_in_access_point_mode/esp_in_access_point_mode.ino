#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>      //include mDNS library

const char *ssid = "password_is_hello123";
const char *password = "hello123";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  if(!MDNS.begin("lightify")){
    Serial.println("Error setting up mDNS");
  }

  Serial.println("mDNS responder started");

}

void loop() {
  // put your main code here, to run repeatedly:

}
