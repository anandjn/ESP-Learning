/*************************SPIFFS*************************/
String formatBytes(size_t bytes){
  if(bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < 1024*1024){
     return String(bytes/1024.0) + "KB";
  } else if (bytes < 1024*1024*1024) {
    return String(bytes/1024.0/1024.0) + "MB";
  }
}

/*************************SERVER*************************/
void handleNotFound(){      //if requested file dose'nt exist return error 404
  if(!handleFileRead(server.uri())){      //look for file in SPIFFS (handelFileRead returns False if file dosen't exists)
    server.send(404, "text/plain", "404: File not Found");
    }
  }

//sends the requested file back to browser if it exists and return True else False
bool handleFileRead(String path) {
  Serial.print("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html" ;      //If root or URL is requested we send index.html
  String contentType = getContentType(path);          //get the contentType of file
  String pathWithGz = path + ".gz";                  //extension with gzip 
  if ((SPIFFS.exists(pathWithGz)) || (SPIFFS.exists(path))){    //if file exists as compressed or normal then send and return True
    //if compressed version exists; modify path to compressed file
    if (SPIFFS.exists(pathWithGz)) path += ".gz";             
      
      File file = SPIFFS.open(path, "r");              //open the file with reading mode and store in File object
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

/*************************WEB-SOCKET*************************/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  switch(type){ 
    case WStype_DISCONNECTED:         //if the websocket is disconnected
      Serial.printf("[%u] Disconnected! \n", num);
      break;
    case WStype_CONNECTED:{           //if new connection is established print its ip
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      break;
      }
    case WStype_TEXT:{                 //if text data is received
      Serial.printf("[%u] get Text: %s \n", num, payload);
      /**********************************/
      /*                                */
      /*        PROCESS-TEXT HERE       */
      /*                                */
      
      if (payload[0] == '#') {
        //data is for our LED
        String r = "";
        String y = "";
        
        //extract data
        int i = 1;
        bool flag = true;
        while(payload[i]){
          if(payload[i] == ' ') {
            flag = false;
            i++;
          }
          if (flag == true){
            Serial.print("r1=");
            Serial.println(r);
            Serial.print("payload");
            Serial.println(payload[i]-48);
            r = r + (payload[i]-48);
          }
          else {
            y = y + (payload[i]-48);
          }
          i++;
        }
        Serial.print("r=");
        Serial.println(r);
        Serial.print("y=");
        Serial.println(y);
        //map
        analogWrite(LED_RED, r.toInt());
        analogWrite(LED_YELLOW, y.toInt());
        
        /**********************************/
      }
      break;
    }
  }   
}
