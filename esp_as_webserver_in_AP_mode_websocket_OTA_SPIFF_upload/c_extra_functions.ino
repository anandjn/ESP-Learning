/***************SERVER (helping function extras)**************************/
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
