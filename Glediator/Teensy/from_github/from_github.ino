/*
  This is a simple arduino sketch to use with your teensy2ledcontroller [0].
  You need to install the FastSPI_Led2 library [1] to compile this code.
  
  It is as straight forward as it gets.
  Open a Serial interface with 115200 Baud.
  Implement the Glediator [2] Protocol (0x01 startbyte followed by NUM_LEDS * 3 bytes of data).
  Display the data.
  Profit.
  
  
  This code should work with every Arduino, Teensy or other platform that implements the Arduino wiring.
  It was so far only tested with Teensy 2.0 [3] tough.
[0] http://www.hackerspaceshop.com/teensy/teensy2ledcontroller.html
[1] http://code.google.com/p/fastspi/
[2] http://glediator.de / http://solderlab.de/index.php/software/glediator
[3] http://www.hackerspaceshop.com/teensy/teensy-2-0.html
*/


#include "FastSPI_LED2.h"


//  how many pixels will connect in total?

#define NUM_LEDS 144



const int dataline = 2;
const int clockline = 10;

//const int startbutton = PIN_D3;


CRGB leds[NUM_LEDS];

void setup() {

        Serial.begin(115200); // this is currently at 9600 .. but should be faster in the future .. just dont wonder if your current setup doesn't work out of the box with this baudrate
        Serial.println("Serial initalized!\nFeed me some data. njam njam.");
  
   	// For safety (to prevent too high of a power draw), the test case defaults to
   	// setting brightness to 25% brightness
        // LEDS.setBrightness(64);

   	 //LEDS.addLeds<WS2812, dataline>(leds, NUM_LEDS);
   	// LEDS.addLeds<TM1809, dataline>(leds, NUM_LEDS);
   	// LEDS.addLeds<UCS1903, dataline>(leds, NUM_LEDS);
   	// LEDS.addLeds<TM1803, dataline>(leds, NUM_LEDS);

   	// LEDS.addLeds<P9813>(leds, NUM_LEDS);
   	
   	//LEDS.addLeds<LPD8806>(leds, NUM_LEDS);
	// LEDS.addLeds<WS2801>(leds, NUM_LEDS);
   	// LEDS.addLeds<SM16716>(leds, NUM_LEDS);

   	// LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);

	// Put ws2801 strip on the hardware SPI pins with a BGR ordering of rgb and limited to a 1Mhz data rate
	LEDS.addLeds<WS2801, dataline, clickline, BGR, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);

   	// LEDS.addLeds<LPD8806, dataline, clockline>(leds, NUM_LEDS);
   	// LEDS.addLeds<WS2811, dataline, BRG>(leds, NUM_LEDS);
   	// LEDS.addLeds<LPD8806, BGR>(leds, NUM_LEDS);
   
   /*  
   //there is a button on the teensy2 ledcontroller.. but we dont use it for now..
   // initialize the pushbutton pin as an input:
   pinMode(startbutton, INPUT_PULLUP); 
   
   while (digitalRead(startbutton))
   {
     // show some animation till the button is pressed ..
   }
   
   */
   
}





// hacky hack implements blocking read() from serial.
int serialReadBlocking() {
  while (!Serial.available()) {}
  return Serial.read(); // one byte
}










void loop() {
   while (serialReadBlocking() != 1) {} // head of frame ...
   
//Serial.println("FRAMESTART");
   for (int i=0; i < NUM_LEDS; i++) {
     leds[i].r = serialReadBlocking();
     leds[i].g = serialReadBlocking();
     leds[i].b = serialReadBlocking();
   }
//Serial.println("FRAMEEND");   
    
   // display the received data on the matrix 
   FastSPI_LED.show();
}

