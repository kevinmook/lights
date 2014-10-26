#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN             2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS       150

#define MAX_BRIGHTNESS  100
#define DELAY_VAL       50

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int brightness, red, green, blue, loop_count, color_mode;
bool intensifying;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  loop_count = 0;
  intensifying = false;
  color_mode = 0;
}

void loop() {
  loop_count = loop_count % MAX_BRIGHTNESS;
  if(loop_count == 0) {
    intensifying = !intensifying;
    
    if(intensifying) {
      color_mode += 1;
      color_mode = color_mode % 8;
      if(color_mode == 0) {
        color_mode += 1;
      }
    }
  }
  
  for(int led=0; led<NUMPIXELS; led++){
    brightness = intensifying ? loop_count : MAX_BRIGHTNESS - loop_count;
    red = 0;
    green = 0;
    blue = 0;
    if(color_mode & 1) red = brightness;
    if(color_mode & 2) green = brightness;
    if(color_mode & 4) blue = brightness;
    
    pixels.setPixelColor(led, pixels.Color(red, green, blue));
  }
  
  pixels.show();
  delay(DELAY_VAL); // Delay for a period of time (in milliseconds).
  loop_count += 1;
}
