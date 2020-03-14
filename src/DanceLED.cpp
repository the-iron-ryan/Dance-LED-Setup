#include <FastLED.h>
#include <IRremote.h>
#include <Thread.h>
#include <ThreadController.h>

#include "Palettes.h"
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

#define NUM_CHANGERS 4
Changer* changers[NUM_CHANGERS];
Changer* CUR_CHANGER;

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

// Create paletee array
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
  changers[0] = new StaticChanger(leds(0, NUM_LEDS), CRGB::Red);
  changers[1] = new StaticChanger(leds(0, NUM_LEDS), CRGB::Blue);
  changers[2] = new StaticChanger(leds(0, NUM_LEDS), CRGB::Green);
  changers[3] = new SplitLine<1, 6>(leds(0, NUM_LEDS));

  // Default to changer 0
  CUR_CHANGER = changers[2];

  // Init IR remote
  remote = Remote(IR_SIGNAL_PIN, NUM_CHANGERS);
  remote.addRemotePair(ERemoteButton::ZERO, changers[0]);
  remote.addRemotePair(ERemoteButton::ONE, changers[1]);
  remote.addRemotePair(ERemoteButton::TWO, changers[2]);
  remote.addRemotePair(ERemoteButton::THREE, changers[3]);

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
  CUR_CHANGER->stop();

  currentChangerIndex = random(NUM_CHANGERS);
  currentPaletteIndex = random(NUM_PALETTES);

  CUR_CHANGER = changers[currentChangerIndex];
  CUR_CHANGER->setPalette(palettes[currentPaletteIndex]);

  CUR_CHANGER->init();
}

void setLeds()
{
    data->update();
    if (data->strongBeat)
    {
      // startNewEpoch();
    }
    else if (data->weakBeat)
    {
      // CUR_CHANGER->setPalette(palettes[random(NUM_PALETTES)]);
    }

    // Serial.print("Changer Ptr: ");
    // Serial.println((unsigned long) CUR_CHANGER, HEX);
    CUR_CHANGER->step();

    FastLED.show();
  }

void loop()
{
  // Get IR results, if any
  remote.pingResutls();

  setLeds();
}
