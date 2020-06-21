#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>     //webserver library
#include <FS.h>                   //SPIFF library

ESP8266WebServer server(80);     //server object listening to port 80

File fsUploadFile;              //a file object to temporary store received file

const char *ssid = "baman-baman";
const char *password = "dobaarbaman";

/***FUNCTION-PROTOTYPES*********/
//return the proper content type for file given, eg (input -> .html output -> text/html)
//which will later be sent to browser to comprehend
//converts file extension to MIME type
String getContentType(String filename);
//sends the requested file back to browser if it exists
bool handleFileRead(String path);
//uploads a new file to the SPIFFS
void handleFileUpload();

/*****************************/

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

  SPIFFS.begin();         //Begins the SPI Flash file system

  /*****SERVER-CONFIG*******/
  //handling file uploads
  server.on("/upload", HTTP_GET, []() {
      if (!handleFileRead("/upload.html")){
      server.send(404, "text/plain", "404: Not Found");
      }
    
    });
  /******err*********/
  server.on("/upload", HTTP_POST, 
    [](){}, 
    handleFileUpload
    );
 /**********************/
 
  //handling all other requests of server
  server.onNotFound([](){
    if (!handleFileRead(server.uri())){
      server.send(404, "text/plain", "404: Not Found");
    }
    });
  
  /***********************/ 

  /******START-SERVER*****/
  server.begin();       //starting the server
  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();  //Keep listening to request from clients  

}



/*****FUNCTION-DESCRIPTION**********/

//return the proper content type for file given, eg (input -> .html output -> text/html)
//which will later be sent to browser to comprehend
//converts file extension to MIME type

String getContentType(String filename){
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

//sends the requested file back to browser if it exists
bool handleFileRead(String path) {
  Serial.print("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html" ;      //If root or URL is requested we send index.html
  String contentType = getContentType(path);          //the the contentType of file
  String pathWithGz = path + ".gz";                  //extension with gzip 
  if ((SPIFFS.exists(pathWithGz)) || (SPIFFS.exists(path))){    //if file exists as compressed or normal then send and return True
    //if compressed version exists; modify path to compressed file
    if (SPIFFS.exists(pathWithGz)) path += ".gz";             
      
      File file = SPIFFS.open(path, "r");             //open the file with reading mode and store in File object
      size_t sent = server.streamFile(file, contentType);   //send the opened file to client
      Serial.println(String("\tSent file: ") + path);
      file.close();                                   //close file object
      return true;  
  }
  //if does not exists then return false
  Serial.println("\t File Not Found");
  return false;
}

//receive file from browser and write it to SPIFFS on webserver

void handleFileUpload(){
  HTTPUpload& upload = server.upload();                //local object referncing
  if (upload.status == UPLOAD_FILE_START) {         //phase 1
    String filename = upload.filename;                //getting file name
    if(!filename.startsWith("/")) filename = "/" + filename;       //puting the file in folder
    Serial.print("handleFileUpload name: "); Serial.println(filename);        
    fsUploadFile = SPIFFS.open(filename, "w");            //this creates file in write mode if doesnt exist or overwrite if exists
    //filename = String();            //clear memory    
  } else if (upload.status == UPLOAD_FILE_WRITE) {     //phase 2
      if(fsUploadFile) {                                    //if ready for upload
        fsUploadFile.write(upload.buf, upload.currentSize);   //write received bytes to file
      }
  } else if (upload.status == UPLOAD_FILE_END) {    //phase 3
      if(fsUploadFile) {                                  //if file is created
        fsUploadFile.close();                           //close and free the file object
        Serial.print("handleFileUpload size: "); Serial.println(upload.totalSize);
        //after uploading or receiving the complete file respond the clint to new location (success page)
        server.sendHeader("Location", "/success.html");
        server.send(303);
      }else {                                         //if file creation failed
      server.send(500, "text/plain", "500: couldn't create file");
      }
    }
}
