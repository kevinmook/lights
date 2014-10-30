
// this is an attempt at getting WS2812.h to work on the digix

#include <DigiFi.h>
#include <WS2812.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN             2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS       250

#define MAX_BRIGHTNESS  100
#define DELAY_VAL       50

WS2812 LED(NUMPIXELS);

cRGB value;
int low_led, high_led;
int red, green, blue;
DigiFi wifi;

void setup() {
  bool toggle = false;
  LED.setOutput(PIN);
  
  for(int led=0; led<=NUMPIXELS; led++) {
    setColor(led, 0, 0, 50);
  }
  applyColor();
  
  wifi.begin(9600);
  
  while (wifi.ready() != 1) {
    delay(1000);
    for(int led=0; led<=NUMPIXELS; led++) {
      if(toggle) {
        setColor(led, 50, 0, 0);
      } else {
        setColor(led, 0, 0, 50);
      }
    }
    applyColor();
    toggle = !toggle;
  }
  String address = wifi.server(8080);//sets up server and returns IP
  
  for(int led=0; led<=NUMPIXELS; led++) {
    setColor(led, 50, 50, 50);
  }
  applyColor();
}

void loop() {
  if(wifi.serverRequest()) {
    char request_path[50];
    wifi.serverRequestPath().toCharArray(request_path, 50);
    low_led = -1;
    high_led = -1;
    red = -1;
    green = -1;
    blue = -1;
    sscanf(request_path, "/?command=%d,%d,%d,%d,%d", &low_led, &high_led, &red, &green, &blue);
    if (low_led >= 0 && high_led >= 0 && red >= 0 && green >= 0 && blue >= 0) {
      red &= 0xFF;
      green &= 0xFF;
      blue &= 0xFF;
      if(low_led < 0) low_led = 0;
      if(low_led >= NUMPIXELS) low_led = NUMPIXELS - 1;
      if(high_led < 0) high_led = 0;
      if(high_led > NUMPIXELS) high_led = NUMPIXELS;
      if(low_led > high_led) low_led = high_led;
      for(int led=low_led; led<high_led; led++) {
        setColor(led, red, green, blue);
      }
      applyColor();
    }
    
    // pixels.getPixels();   // returns current state
    serverResponse("<html><body><h1>Doin' Stuff!</h1></body></html>", 200);
  }

  delay(10);
}

void setColor(int led, int r, int g, int b) {
  // pixels.setPixelColor(led, pixels.Color(r, g, b));
  value.r = r;
  value.g = g;
  value.b = b;
  LED.set_crgb_at(led, value); // Set value at LED found at index 0
}

void applyColor() {
  // pixels.show();
  LED.sync(); // Sends the value to the LED
}

void serverResponse(String response, int code) { //defaults to code = 200
    Serial1.print("HTTP/1.1 ");
    Serial1.print(code);
    if(code==200)
        Serial1.print(" OK");
    else if(code==404)
        Serial1.print(" Not Found");
    else
        Serial1.print(" OK"); //left as OK to not mess anything up
    Serial1.print(" \r\n");
    Serial1.print("Content-Type: text/html;\r\n");
    Serial1.print("Content-Length: ");
    Serial1.print(response.length());
    Serial1.print("\r\n");
    Serial1.print("Access-Control-Allow-Origin: *\r\n");
    Serial1.print("Connection: close\r\n\r\n");
    Serial1.print(response);
    Serial1.print("\r\n\r\n");

    return;
}