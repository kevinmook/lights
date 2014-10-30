
#define ADAFRUIT

#include <DigiFi.h>

#ifndef ADAFRUIT
  #include <WS2812.h>
#else
  #include <Adafruit_NeoPixel.h>
#endif

#define PIN             2
#define NUMPIXELS       50

#ifndef ADAFRUIT
  WS2812 LED(NUMPIXELS);
  cRGB value;
#else
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#endif

int low_led, high_led;
int red, green, blue;
DigiFi wifi;

void setup() {
  bool toggle = false;
  initializeLEDs();
  
  for(int led=0; led<NUMPIXELS; led++) {
    setColor(led, led, 0, NUMPIXELS - led);
  }
  applyColor();
  
  wifi.begin(9600);
  
  while (wifi.ready() != 1) {
    delay(1000);
    for(int led=0; led<NUMPIXELS; led++) {
      if(toggle) {
        setColor(led, 0, 0, 50);
      } else {
        setColor(led, 50, 0, 0);
      }
    }
    applyColor();
    toggle = !toggle;
  }
  String address = wifi.server(8080);//sets up server and returns IP
  
  for(int led=0; led<NUMPIXELS; led++) {
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

void initializeLEDs() {
  #ifndef ADAFRUIT
    LED.setOutput(PIN);
  #else
    pixels.begin();
  #endif
}

void setColor(int led, int r, int g, int b) {
    r &= 0xFF;
    g &= 0xFF;
    b &= 0xFF;
    if(low_led < 0) low_led = 0;
    if(low_led >= NUMPIXELS) low_led = NUMPIXELS - 1;
    if(high_led < 0) high_led = 0;
    if(high_led > NUMPIXELS) high_led = NUMPIXELS;
    if(low_led > high_led) low_led = high_led;
  #ifndef ADAFRUIT
    value.r = r;
    value.g = g;
    value.b = b;
    LED.set_crgb_at(led, value);
  #else
    pixels.setPixelColor(led, pixels.Color(r, g, b));
  #endif
}

void applyColor() {
  #ifndef ADAFRUIT
    LED.sync(); // Sends the value to the LED
  #else
    pixels.show();
  #endif
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