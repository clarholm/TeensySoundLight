// LED Audio Spectrum Analyzer Display
//
// Creates an impressive LED light show to music input
//   using Teensy 3.1 with the OctoWS2811 adaptor board
//   http://www.pjrc.com/store/teensy31.html
//   http://www.pjrc.com/store/octo28_adaptor.html
//
// Line Level Audio Input connects to analog pin A3
//   Recommended input circuit:
//   http://www.pjrc.com/teensy/gui/?info=AudioInputAnalog
//
// This example code is in the public domain.

#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#define DATA_PIN    2
#define CLK_PIN   10
#define LED_TYPE    WS2801
#define COLOR_ORDER GRB
//#define NUM_LEDS    50

// The display size and color to use
const unsigned int matrix_width = 15;
const unsigned int matrix_height = 10;
const unsigned int myColor = 0x400020;

// These parameters adjust the vertical thresholds
const float maxLevel = 1;      // 1.0 = max, lower is more "sensitive"
const float dynamicRange = 40.0; // total range to display, in decibels
const float linearBlend = 0.3;   // useful range is 0 to 0.7

CRGB leds[matrix_width * matrix_height];
/*
// Audio library objects
//AudioInputAnalog         adc1(A3);       //xy=99,55
AudioInputI2S            i2s1;           //xy=139,91
AudioAnalyzeFFT1024      fft;            //xy=265,75
AudioConnection          patchCord1(i2s1, fft);
AudioControlSGTL5000     audioShield;    //xy=366,225
const int myInput = AUDIO_INPUT_MIC;
*/

AudioInputI2S            i2s1;           //xy=139,91
AudioMixer4              mixer1;         //xy=312,134
AudioOutputI2S           i2s2;           //xy=392,32
AudioAnalyzeFFT1024      fft;        //xy=467,147
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 0, i2s2, 0);
AudioConnection          patchCord3(i2s1, 1, mixer1, 1);
AudioConnection          patchCord4(i2s1, 1, i2s2, 1);
AudioConnection          patchCord5(mixer1, fft);
AudioControlSGTL5000     audioShield;    //xy=366,225

const int myInput = AUDIO_INPUT_MIC;
//const int myInput = AUDIO_INPUT_LINEIN;

// This array holds the volume level (0 to 1.0) for each
// vertical pixel to turn on.  Computed in setup() using
// the 3 parameters above.
float thresholdVertical[matrix_height];

// This array specifies how many of the FFT frequency bin
// to use for each horizontal pixel.  Because humans hear
// in octaves and FFT bins are linear, the low frequencies
// use a small number of bins, higher frequencies use more.
int frequencyBinsHorizontal[matrix_width] = {
   1,  1,  1,  2,  3,  6,  15,  25,  30,  35, 44, 60, 100, 150
};



// Run setup once
void setup() {
  Serial.println("Setup started");
  // the audio library needs to be given memory to start working
  AudioMemory(12);

  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.4);
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);

  // compute the vertical thresholds before starting
  computeVerticalLevels();

  // turn on the display
  //  FastLED.addLeds<OCTOWS2811>(leds,(matrix_width * matrix_height) / 8);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN>(leds,(matrix_width * matrix_height));
}

// A simple xy() function to turn display matrix coordinates
// into the index numbers OctoWS2811 requires.  If your LEDs
// are arranged differently, edit this code...
unsigned int xy(unsigned int x, unsigned int y) {
  if ((y & 1) == 0) {
    // even numbered rows (0, 2, 4...) are left to right
    return y * matrix_width + x;
  } else {
    // odd numbered rows (1, 3, 5...) are right to left
    return y * matrix_width + matrix_width - 1 - x;
  }
}

// Run repetitively
void loop() {
  unsigned int x, y, freqBin;
  float level;

  if (fft.available()) {
    

    // freqBin counts which FFT frequency data has been used,
    // starting at low frequency
    freqBin = 0;

    for (x=0; x < matrix_width; x++) {
      // get the volume for each horizontal pixel position
      level = fft.read(freqBin, freqBin + frequencyBinsHorizontal[x] - 1);

      // uncomment to see the spectrum in Arduino's Serial Monitor
      Serial.print(level);
      Serial.print("  ");

      for (y=0; y < matrix_height; y++) {
        // for each vertical pixel, check if above the threshold
        // and turn the LED on or off
        if (level >= thresholdVertical[y]) {
          leds[xy(x,y)] = CRGB(myColor);
        } else {
          leds[xy(x,y)] = CRGB::Black;
        }
      }
      // increment the frequency bin count, so we display
      // low to higher frequency from left to right
      freqBin = freqBin + frequencyBinsHorizontal[x];
    }
    // after all pixels set, show them all at the same instant
    FastLED.show();
    Serial.println();
  }
}


// Run once from setup, the compute the vertical levels
void computeVerticalLevels() {
  unsigned int y;
  float n, logLevel, linearLevel;

  for (y=0; y < matrix_height; y++) {
    n = (float)y / (float)(matrix_height - 1);
    logLevel = pow10f(n * -1.0 * (dynamicRange / 20.0));
    linearLevel = 1.0 - n;
    linearLevel = linearLevel * linearBlend;
    logLevel = logLevel * (1.0 - linearBlend);
    thresholdVertical[y] = (logLevel + linearLevel) * maxLevel;
  }
}
