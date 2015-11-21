#include <FastLED.h>
#define NUM_LEDS 144
const int dataline = 2;
const int clockline = 10;

CRGB leds[NUM_LEDS];
void setup() {
Serial.begin(115200); 
LEDS.addLeds<WS2801, dataline, clockline>(leds, NUM_LEDS);
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
delay(10);
FastSPI_LED.show();
}
