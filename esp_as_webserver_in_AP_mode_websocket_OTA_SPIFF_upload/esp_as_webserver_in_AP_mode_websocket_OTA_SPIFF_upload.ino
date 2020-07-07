#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   //webserver library
#include <ArduinoOTA.h>         //over the air
#include <FS.h>                 //SPIFF library
#include <WebSocketsServer.h>

ESP8266WebServer server(80);    //server object listening to port 80 for HTTP request
WebSocketsServer webSocket(81);  //websocket server at port 81

File fsUploadFile;              //a file object to temporary store received file

//access-point credentials
const char *ssid = "baman-baman";
const char *password = "dobaarbaman";

//message for client

String message = "Hello World!";
//long long previousMillis= millis();

//OTA-credentials


//connections
#define LED_RED     D1
#define LED_YELLOW  D2

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  Serial.begin(115200);
  delay(10);
  Serial.print("\r\n");

  //basic_setup
  startWiFi();      //start AP
  startOTA();       //start OTA
  startSPIFFS();    //start SPIFFS
  startWebSocket(); //start WebSocket server
  startServer();    //start SERVER
}

void loop() {
  webSocket.loop();             //constantly check for websocket events
  server.handleClient();        //run the server
  ArduinoOTA.handle();          //listen to OTA updates

  //if (millis() - previousMillis > 1000){
    webSocket.broadcastTXT(message.c_str(), message.length());
    //previousMillis = millis();
  //}

}
