#include <WS2812.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN             0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS       140

#define MAX_BRIGHTNESS  255
#define DELAY_VAL       50

WS2812 LED(NUMPIXELS);
cRGB value;

int brightness, red, green, blue;
int loop_count = 0;
bool intensifying = false;
int color_mode = 0;

void setup() {
  LED.setOutput(PIN); // Digital Pin 9
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
    value.r = 0;
    value.g = 0;
    value.b = 0;
    if(color_mode & 1) value.r = brightness;
    if(color_mode & 2) value.g = brightness;
    if(color_mode & 4) value.b = brightness;
    
    LED.set_crgb_at(i, value); // Set value at LED found at index 0
  }
  
  LED.sync(); // Sends the value to the LED
  delay(DELAY_VAL); // Delay for a period of time (in milliseconds).
  loop_count += 1;
}
