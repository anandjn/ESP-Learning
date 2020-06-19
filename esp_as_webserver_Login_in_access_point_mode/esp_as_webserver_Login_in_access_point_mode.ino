#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>     //webserver library

ESP8266WebServer server(80);     //server object listening to port 80

const char *ssid = "baman-baman";
const char *password = "dobaarbaman";


/***FUNCTION-PROTOTYPES*********/
void handleRoot();          //function to run when request "/"
void handleLogin();           //function to run when received post request /login with arguments
void handleNotFound();      //function to run when invalid request
/******************************/



void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);
  Serial.print("Access point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  /****SERVER-CONFIG****/
  server.on("/", HTTP_GET ,handleRoot);      //function call to handleRoot when requested "/" (root page: 192.168.4.1/) from server
  server.on("/login", HTTP_POST, handleLogin);    //function to run when received post request /login with some arguments
  server.onNotFound(handleNotFound);         //function call to handleNotFound when invalid request  
 
  /******START-SERVER*****/
  server.begin();
  Serial.print("Web-server started");

}

void loop() {
  server.handleClient();  //Keep listening to request from clients      
}

/******FUNCTION-DESCRIPTIONS*********/
//serves status 200 (ok) and a login form
void handleRoot(){
  server.send(200, "text/html", "<form action = \"/login\" method=\"post\"><input type=\"text\" name=\"username\" placeholder=\"Your Username\"><br><input type=\"text\" name=\"password\" placeholder=\"Your Password\"><br><input type=\"submit\" value = \"Login\"></form><p>Try \"admin\" and \"visualiser\"</p>");
}

//check for arguments and permit or deny login
void handleLogin(){
  //if credential are not available or NULL send bad request (400)
  if( ! server.hasArg("username") || ! server.hasArg("password") || server.arg("username") == NULL || server.arg("password") == NULL) {
    server.send(400, "text/plain", "404: Invalid Request");
    return;    
  }

  //if credential are right send 200(ok) and allow login
  if (server.arg("username") == "admin" && server.arg("password") == "visualiser"){
    server.send(200, "text/html", "<h1> Welcome," + server.arg("username") + "!</h1> <p>login successful</p> ");
  }
  //else send 401 unauthorized
  else {
    server.send(401, "text/plain", "401: Unauthorized ");
  }
}

//send 404 when there is no handler for requested URI
void handleNotFound(){
  server.send(404, "text/plain", "404: Not Found");
}
