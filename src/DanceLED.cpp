#include <FastLED.h>
#include <IRremote.h>
#include <Thread.h>
#include <ThreadController.h>

#include "ChangerData.h"

#include "Remote.h"
#include "MusicData.h"
#include "PushThrough.h"
#include "StaticChanger.h"
#include "Changer.h"
#include "SimplePulse.h"
#include "MixBar.h"
#include "SplitSpiral.h"
#include "SplitLine.h"
#include "BassPush.h"

// Arduino Music Visualizer 0.3

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// This code uses the Sparkfun Spectrum Shield

// Settings for IR remote signal
const int IR_SIGNAL_PIN = 7;

// Pins for reading raw analog audio
#define LEFT_RAW_AUDIO_PIN A3
#define RIGHT_RAW_AUDIO_PIN A4

// LED LIGHTING SETUP
#define LED_PIN     54
#define NUM_LEDS    150
#define BRIGHTNESS  200
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
CRGB realleds[NUM_LEDS];
CRGBSet leds(realleds, NUM_LEDS);


DEFINE_GRADIENT_PALETTE( PAL_HEATMAP_TEST ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( PAL_QUAD_TEST ) {
  0,   255,  0,  0,   // red
 62,   255,  0,  0,   // red
125,     0,255,  0,   // green
188,     0,  0,255,   // blue
251,   255,255,  0,   // yellow
255,   255,255,  0, };// yellow

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

DEFINE_GRADIENT_PALETTE( PAL_REVRAINBOW ) {
  0,     148,   0,    211,   // v
  42,    75,    0,    130,  // i
  84,    0,     0,    255,  // b
  126,   0,     255,  0,    // g
  168,   255,   255,  0,   // y
  210,   255,   127,  0,   // o
  255,   255,   0,    0,    // r
};

DEFINE_GRADIENT_PALETTE( PAL_MAGMA ) {
	0,		48,		11,		102,		// Deep Violet
	50,		132,	32,		130,		// Maximum Purple
	125,	199,	57,		116,		// Fuchsia Rose
	175,	254,	138,	92,			// Coral
	255,	252,	252,	186,		// Very Pale Yellow
};

DEFINE_GRADIENT_PALETTE( PAL_FOREST ) {
	0,		11,		102,	15,		// Deep Green
	50,		9,		155,	106,	// Green (NCS)
	125,	33,		198,	190,	// Maxmimum Blue-Green
	175,	59,		134,	191,	// Cyan-Blue Azure
	255,	164,	165,	191,	// Blue Bell
};

DEFINE_GRADIENT_PALETTE( PAL_MONOCHROME )
{
  0,    1,    1,    1,    // Black
  230,  127,  127,  127,  // Grey
  255,  255,  255,  255,    // White
};

DEFINE_GRADIENT_PALETTE ( PAL_BEACHY )
{
  0,    0,    12,   188,    // Blue
  100,  210,  184,  0,      // Yellow
  180,  0,    175,  0,      // Green
  255,  0,    90,   255,    // Blue, but Different!
};

#define NUM_PALETTES 5
CRGBPalette16 palettes[NUM_PALETTES];

// AUDIO INPUT SETUP
MusicData* data = MusicData::instance();
MusicFrame testFrame = MusicFrame(0);

// Init remote
Remote remote = Remote(IR_SIGNAL_PIN, NUM_CHANGERS);

// Current tick
long tick = 0;

// Time passed tracking
#define TICKS_PER_EPOCH 500

// Track epochs and preallocate random selectors.
int currentChangerIndex;
int currentPaletteIndex;

void setup()
{
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();

  // CREATE CHANGER COLLECTION
  //changers[0] = new SplitSpiral<1, 6>  (leds(0, NUM_LEDS));
  //changers[1] = new SplitSpiral<1, 6>  (leds(0, NUM_LEDS));
  //changers[2] = new SplitSpiral<1, 6>  (leds(0, NUM_LEDS));
  changers[0] = new StaticChanger(leds, PAL_MAGMA, 100);
  changers[1] = new StaticChanger(leds, PAL_FOREST, 100);

  // Init IR remote
  remote = Remote(IR_SIGNAL_PIN, NUM_CHANGERS);
  remote.addRemotePair(ERemoteButton::ZERO, changers[0]);
  remote.addRemotePair(ERemoteButton::ONE, changers[1]);

  // CREATE PALETTE COLLECTION
  palettes[0] = PAL_BEACHY;
  palettes[1] = PAL_REVRAINBOW;
  palettes[2] = PAL_FOREST;
  palettes[3] = PAL_MAGMA;
  palettes[4] = PAL_HALLOWEEN_GHOUL;

  // SERIAL AND INPUT SETUP
  Serial.begin(9600);

  // RANDOM SEED
  randomSeed(analogRead(LED_PIN));

  // BEGIN LOG
  Serial.println("\nListening...");
}



// Gets the current epoch, according to the number of ticks
int getCurrentEpoch()
{
  tick++;
  return tick / TICKS_PER_EPOCH;
}

void startNewEpoch()
{
  remote.getCurrentChanger()->stop();

  currentChangerIndex = random(NUM_CHANGERS);
  currentPaletteIndex = random(NUM_PALETTES);

  // CUR_CHANGER = changers[currentChangerIndex];
  // CUR_CHANGER->setPalette(palettes[currentPaletteIndex]);

  // CUR_CHANGER->init();
}

void setLeds()
{
  if(remote.getCurrentChanger())
  {
    data->update();
    // if (data->strongBeat)
    // {
    //   startNewEpoch();
    // }
    // else if (data->weakBeat)
    // {
    //   CUR_CHANGER->setPalette(palettes[random(NUM_PALETTES)]);
    // }

    Serial.print("Changer Ptr: ");
    Serial.println((unsigned long)remote.getCurrentChanger(), HEX);
    remote.getCurrentChanger()->step();

    FastLED.show();
  }
}

void loop()
{
  // Get IR results, if any
  remote.pingResutls();

  setLeds();
}
