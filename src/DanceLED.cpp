#include <FastLED.h>

#include "PushThrough.h"
#include "Changer.h"
#include "Rainbow.h"
#include "SimplePulse.h"
#include "MixBar.h"

// Arduino Music Visualizer 0.3

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// This code uses the Sparkfun Spectrum Shield

// LED LIGHTING SETUP
#define LED_PIN     7
#define NUM_LEDS    1097
#define BRIGHTNESS  10
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
CRGB realleds[NUM_LEDS];
CRGBSet leds(realleds, NUM_LEDS);


// Palette definition
DEFINE_GRADIENT_PALETTE( PAL_HEATMAP_TEST ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_GEN ) {
  0,    0,    0,    0,
  32,    0,    21,   140, // midnight blue
  128,   0,    196,  84,  // emerald green
  170,  232,  104,  0,   // pumpkin orange
  230,  232,  104,  0,   // pumpkin orange
  255,  255,  255,  255, // full white
};

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_PUMPKIN ) {
  0,    0,    0,    0,  // black
  16,   100,  25,   25, // dark red
  96,   230,  10,   10, // blood red
  120,  255,  150,  40, // pumpkin orange
  245,  255,  255,  40, // candle yellow
  255,  255,  255,  255, // pure white
};

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_GHOUL ) {
  0,    0,    0,    0,    // black
  16,   90,   0,    120,  // dark purple
  64,   160,  30,   160,  // light purple
  200,  0,    150,  40,   // green
  255,  255,  255,  255   // pure white
};

DEFINE_GRADIENT_PALETTE( PAL_POINTY_PARTY ) {
  0,    0,    0,    0, // black
  16,   150,  44,   44, // Dark Rose
  180,  208,  100,  100, // Old Rose
  210,  220,  220,  220, // Grey
  255,  255,  255,  255, // full while
};

DEFINE_GRADIENT_PALETTE( PAL_RAINBOW ) {
  0,    0,    0,    0,    // black
  10,   209,  0,    0,    // r
  50,   255,  102,  32,   // o
  100,   255,  213,  33,   // y
  150,   51,   221,  0,    // g
  200,   17,   51,   204,  // b
  225,   34,   0,    102,  // i
  255,   51,   0,    68,   // v
};

#define NUM_PALETTES 5
CRGBPalette16 palettes[NUM_PALETTES];

// AUDIO INPUT SETUP
int strobe = 4;
int reset = 5;
int audio1 = A0;
int audio2 = A1;
int left[7];
int right[7];
int channels[7];
int band;
int audio_input = 0;
int freq = 0;

// STANDARD VISUALIZER VARIABLES
int midway = NUM_LEDS / 2; // CENTER MARK FROM DOUBLE LEVEL VISUALIZER
int loop_max = 0;
int k = 255; // COLOR WHEEL POSITION
int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION

// Current tick
long tick = 0;

// Time passed tracking
#define TICKS_PER_EPOCH 1000

// Track epochs and preallocate random selectors.
int currentChanger;
int currentPalette;

// Preinit changer array
#define NUM_CHANGERS 3
Changer* changers[NUM_CHANGERS];

void setup()
{
  // SPECTRUM SETUP
  pinMode(audio1, INPUT);
  pinMode(audio2, INPUT);
  pinMode(strobe, OUTPUT);
  pinMode(reset, OUTPUT);
  digitalWrite(reset, LOW);
  digitalWrite(strobe, HIGH);
  
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();

  // CREATE CHANGER COLLECTION
  changers[0] = new MixBar     (channels, leds, 230, NUM_LEDS, PAL_HALLOWEEN_GHOUL);
  changers[1] = new PushThrough(channels, leds, 0,   NUM_LEDS, PAL_HALLOWEEN_GHOUL);
  changers[2] = new SimplePulse(channels, leds, 230, NUM_LEDS, PAL_HALLOWEEN_GHOUL);

  // CREATE PALETTE COLLECTION
  palettes[0] = PAL_HALLOWEEN_GEN;
  palettes[1] = PAL_HALLOWEEN_GHOUL;
  palettes[2] = PAL_HALLOWEEN_PUMPKIN;
  palettes[3] = PAL_POINTY_PARTY;
  palettes[4] = PAL_RAINBOW;

  // SERIAL AND INPUT SETUP
  Serial.begin(9600);
  Serial.println("\nListening...");
}

void readMSGEQ7()
// Function to read 7 band equalizers
{
  digitalWrite(reset, HIGH);
  digitalWrite(reset, LOW);
  for(band=0; band <7; band++)
  {
    digitalWrite(strobe, LOW); // strobe pin on the shield - kicks the IC up to the next band 
    delayMicroseconds(30); // 
    left[band] = analogRead(audio1); // store left band reading
    right[band] = analogRead(audio2); // ... and the right
    channels[band] = max(left[band], right[band]); // Add the maximum between the two as that channel output
    digitalWrite(strobe, HIGH); 
  }
}


// Gets the current epoch, according to the number of ticks
int getCurrentEpoch()
{
  tick++;
  return tick / TICKS_PER_EPOCH;
}

void printLED(CRGBSet set, int index)
{
  Serial.print(set[index].r);
  Serial.print(",\t");
  Serial.print(set[index].g);
  Serial.print(",\t");
  Serial.print(set[index].b);
  Serial.println();
}

void printCLED(int index)
{
  Serial.print(realleds[index].r);
  Serial.print(",\t");
  Serial.print(realleds[index].g);
  Serial.print(",\t");
  Serial.print(realleds[index].b);
  Serial.println();
}

void printChannels()
{
  Serial.print(left[0]);
  Serial.print('\t');
  Serial.print(left[1]);
  Serial.print('\t');
  Serial.print(left[2]);
  Serial.print('\t');
  Serial.print(left[3]);
  Serial.print('\t');
  Serial.print(left[4]);
  Serial.print('\t');
  Serial.print(left[5]);
  Serial.print('\t');
  Serial.print(left[6]);
  Serial.println();
}

void startNewEpoch()
{
  currentChanger = random(NUM_CHANGERS);
  currentPalette = random(NUM_PALETTES);

  changers[currentChanger]->setPalette(palettes[currentPalette]);
}

void loop()
{  

  // Update channel info
  readMSGEQ7();

  if (tick % TICKS_PER_EPOCH == 0)
  {
    startNewEpoch();
  }

  // Iterate whichever changer corresponds to the current epoch
  changers[currentChanger]->step();

  //printLED(leds, NUM_LEDS);
  //printCLED(NUM_LEDS);
  //printChannels();

  FastLED.show();

  tick++;

}