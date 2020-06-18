#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>     //webserver library

ESP8266WebServer server(80);    //server object  to listen at port 80

const int led = 2;

const char *ssid = "baman-baman";
const char *password = "dobaarbaman";

/***FUNCTION-PROTOTYPES*********/
void handleRoot();          //function to run when request "/"
void handleLED();           //function to run when received post request /LED
void handleNotFound();      //function to run when invalid request
/******************************/

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  pinMode(led, OUTPUT);     //set pin mode to output

  WiFi.softAP(ssid, password);
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  /****SERVER-CONFIG****/
  server.on("/", HTTP_GET ,handleRoot);      //function call to handleRoot when requested "/" (root page: 192.168.4.1/) from server
  server.on("/LED", HTTP_POST, handleLED);    //function to run when received post request /LED
  server.onNotFound(handleNotFound);         //function call to handleNotFound when invalid request  
 
  /******START-SERVER*****/
  server.begin();
  Serial.print("Web-server started");
} 

void loop() {
  server.handleClient();        //Keep listening to request from clients
}


/******FUNCTION-DESCRIPTIONS*********/
//function to run when request "/"
void handleRoot(){
  //toggles LED when button is clicked
  server.send(200, "text/html", "<form method=\"post\" action=\"/LED\"><input type=\"submit\" value=\"Toggle LED\"></form>");         //send HTTP status=200 (ok) and send html form
}

//function to run at "/LED" post request
void handleLED(){
  digitalWrite(led, !digitalRead(led));       //change state of led
  server.sendHeader("Location", "/");         //header to go to new location, in this case, homepage
  server.send(303);                           //Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

//function to run when invalid request
void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");      //send HTTP status=404 (Not found) there is no handler for request URI        
}
