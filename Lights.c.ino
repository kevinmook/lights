// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN             6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS       5

#define MAX_BRIGHTNESS  100
#define DELAY_VAL       50

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int brightness, red, green, blue;
int loop_count = 0;
bool intensifying = false;
int color_mode = 0;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
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
  
  for(int i=0;i<NUMPIXELS;i++){
    brightness = intensifying ? loop_count : MAX_BRIGHTNESS - loop_count;
    red = 0;
    green = 0;
    blue = 0;
    if(color_mode & 1) red    = brightness;
    if(color_mode & 2) green  = brightness;
    if(color_mode & 4) blue   = brightness;
    
    pixels.setPixelColor(i, pixels.Color(red,green,blue));
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(DELAY_VAL); // Delay for a period of time (in milliseconds).
  loop_count += 1;
}
