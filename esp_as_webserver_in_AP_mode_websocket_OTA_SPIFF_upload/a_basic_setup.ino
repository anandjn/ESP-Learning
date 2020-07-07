
void startWiFi(){
  WiFi.softAP(ssid, password);
  Serial.print("Access point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
}
/**************************************/
void startOTA(){

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
}
/******************************************************/

void startSPIFFS(){
  SPIFFS.begin();         //Begins the SPI Flash file system and display its contents
  Serial.println("SPIFFS started. Contents:");
  /*----listing----*/
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize =  dir.fileSize();

    Serial.printf("\tFS File: %s, size: %s \r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
  /*------------*/
  Serial.print("\n");
  
}

/*****************************************************/

void startWebSocket(){
  webSocket.begin();                       //start websocket server
  webSocket.onEvent(webSocketEvent);       //on incoming message on websocket run function - webSocketEvent
  Serial.println("Websocket server started"); 
}

/*****************************************************/

void startServer(){
  server.on("/edit.html", HTTP_POST, [](){
      server.send(200, "text/plain", "");
    },handleFileUpload);
  server.onNotFound(handleNotFound);        //send appropitate file if exists or error if does'nt exist
  server.begin();                           //start server
  Serial.println("HTTP server started");
}

/*****************************************************/
