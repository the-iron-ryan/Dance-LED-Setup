#include <FastLED.h>

#include "PushThrough.h"
#include "Changer.h"
#include "Rainbow.h"

// Arduino Music Visualizer 0.3

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// This code uses the Sparkfun Spectrum Shield

// LED LIGHTING SETUP
#define LED_PIN     6
#define NUM_LEDS    150
#define BRIGHTNESS  10
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Time passed tracking
#define TICKS_PER_EPOCH 10000

// AUDIO INPUT SETUP
int strobe = 4;
int reset = 5;
int audio1 = A0;
int audio2 = A1;
int left[7];
int right[7];
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


// Preinit changer array
#define NUM_CHANGERS 2
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
  changers[0] = new PushThrough(left, leds, 230, 1097);
  changers[1] = new Rainbow    (left, leds, 230, 1097);

  // SERIAL AND INPUT SETUP
  Serial.begin(115200);
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
    digitalWrite(strobe, HIGH); 
  }
}


// Gets the current epoch, according to the number of ticks
int getCurrentEpoch()
{
  tick++;
  return tick / TICKS_PER_EPOCH;
}

void loop()
{  
  
  // Update channel info
  readMSGEQ7();

  // Get current epoch
  int epoch = getCurrentEpoch();

  // Iterate whichever changer corresponds to the current epoch
  changers[epoch % NUM_CHANGERS]++;

}