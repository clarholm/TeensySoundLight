#include "FastLED.h"
#define NUM_LEDS 1664
const int dataline = 11;
const int clockline = 13;

CRGB leds[NUM_LEDS];
void setup() {
        Serial.begin(115200); 
   //	LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
   //FastLED.addLeds<APA102,7,11,RGB,DATA_RATE_MHZ(10)>(leds,NUM_LEDS);
   //DATA_RATE_MHZ(12)
   LEDS.addLeds<WS2801, dataline, clockline, RGB, DATA_RATE_KHZ(800)>(leds, NUM_LEDS);
}
int serialGlediator() {
  while (!Serial.available()) {}
  return Serial.read();
}
void loop() {
   while (serialGlediator() != 1) {} 

   for (int i=0; i < NUM_LEDS; i++) {
     leds[i].r = serialGlediator();
     leds[i].g = serialGlediator();
     leds[i].b = serialGlediator();
   }
     FastSPI_LED.show();
}
