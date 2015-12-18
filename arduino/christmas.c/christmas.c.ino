// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <DigiFi.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

#define SERVER_PORT    8080

#define BUFSIZE 255

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
DigiFi server;
bool pixelsChanged = true;
bool currentLineIsBlank = false;
int red = 0;
int green = 150;
int blue = 0;

void setup() {
  setupPixels();
  startServer();
}

void setupPixels() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void startServer() {
  // start the server:
  server.begin();
  server.server(SERVER_PORT); //start server on port 8080
  while (server.ready() != 1) {
    delay(100);
  }
}

void loop() {
  if (server.serverRequest()) {
    handleServerLoop();
  } else if(pixelsChanged) {
    setColors();
    sendColors();
  }

  delay(1);
}

void handleServerLoop() {
  if(server.serverRequestPath() == "/") {
    // send a standard http response header
    server.println("HTTP/1.1 200 OK");
    server.println("Content-Type: text/html");
    server.println("Connection: close");  // the connection will be closed after completion of the response
    //server.println("Refresh: 5");  // refresh the page automatically every 5 sec
    server.println("Transfer-Encoding: chunked"); 
    server.println();
    server.printChunk("<!DOCTYPE HTML>");
    server.printChunk("<html>");
    server.printChunk("<pre>");
    // server.printChunk(clientLine);
    server.printChunk("</pre>");
    server.printChunk("</html>");
    server.closeChunk();

    updateColors();
  } else {
    server.serverResponse("404 Not Found", 404);
    delay(10);
  }
}

void updateColors() {
  if(green == 150) {
    green = 0;
    red = 150;
  } else {
    green = 150;
    red = 0;
  }

  pixelsChanged = true;
}

void setColors() {
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Moderately bright green color.
  }
}

void sendColors() {
  pixels.show(); // This sends the updated pixel color to the hardware.
  pixelsChanged = false;
}
