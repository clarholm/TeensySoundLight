#include "FastLED.h"
#define NUM_LEDS 400
const int dataline = 6;

CRGB leds[NUM_LEDS];
void setup() {
        Serial.begin(115200); 
   	LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
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
