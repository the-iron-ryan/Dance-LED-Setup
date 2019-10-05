#include <FastLED.h>

// Arduino Music Visualizer 0.3

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// This code uses the Sparkfun Spectrum Shield

// LED LIGHTING SETUP
#define LED_PIN             6
#define NUM_LEDS            300
#define NUM_LEDS_PER_CHNL   NUM_LEDS/3
#define BRIGHTNESS          64
#define LED_TYPE            NEOPIXEL
#define MIN_FREQ            80          // Min value to ignore 
#define AMP_AMT             30
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// AUDIO INPUT SETUP
int strobe = 4;
int reset = 5;
int audio1 = A0;
int audio2 = A1;
int left[7];
int right[7];
long max[7]; // max of left and right channels
int band;
int audio_input = 0;
int freq = 0;

// Channel indexes
int bass_start = 0;
int bass_stop = 1;

int mid_start = 2;
int mid_stop = 4;

int treb_start = 5;
int treb_stop = 6;

// Channel Reactions
long bass_react = 0;
long mid_react = 0;
long treb_react = 0;

// Channel LEDs start/stop points
int bass_led_start = 0;
int bass_led_stop = bass_led_start + NUM_LEDS_PER_CHNL - 1;

int mid_led_start = bass_led_stop + 1;
int mid_led_stop = mid_led_start + NUM_LEDS_PER_CHNL - 1;

int treb_led_start = mid_led_stop + 1;
int treb_led_stop = treb_led_start + NUM_LEDS_PER_CHNL - 1;

// Channel Starting Colors
CRGB bass_color = CRGB::Red;
CRGB mid_color = CRGB::Green;
CRGB treb_color = CRGB::Blue;

// STANDARD VISUALIZER VARIABLES
int midway = NUM_LEDS / 2; // CENTER MARK FROM DOUBLE LEVEL VISUALIZER
int loop_max = 0;
int k = 255; // COLOR WHEEL POSITION
int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION

// RAINBOW WAVE SETTINGS
int wheel_speed = 2;

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

  // SERIAL AND INPUT SETUP
  Serial.begin(115200);
  Serial.println("\nListening...");
}

// FUNCTION TO GENERATE COLOR bassD ON VIRTUAL WHEEL
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino
CRGB Scroll(int pos) {
  pos = abs(pos);
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;
}

// FUNCTION TO GET AND SET COLOR
// THE ORIGINAL FUNCTION WENT BACKWARDS
// THE MODIFIED FUNCTION SENDS WAVES OUT FROM FIRST LED
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino
void singleRainbow()
{
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    if (i < react)
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
    else
      leds[i] = CRGB(0, 0, 0);      
  }
  FastLED.show(); 
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

    max[band] = max(left[band], right[band]); // grab max value between left and right audio bands

    digitalWrite(strobe, HIGH); 
  }
}

void convertSingle()
{
  if (left[freq] > right[freq])
    audio_input = left[freq];
  else
    audio_input = right[freq];

  if (audio_input > 80)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 1023L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.println(pre_react);
  }
}

// Calc bass react from current audio input
void calcBassReact()
{
  long bass_input = 0;

  // Do low pass filter:
  // Add to input each of the bass frequencies
  for(int i = bass_start; i <= bass_stop; i++)
  {
    bass_input += max[i];
  }
  Serial.print("Bass input: ");
  Serial.println(bass_input);

  if(bass_input > MIN_FREQ)
  {
    // Get num of leds for bass
    // NOTE: max freq per channel is 1023. We can get the total max by multiplying this value by total frequencies in this channel
    long bass_pre_react = ((long)NUM_LEDS_PER_CHNL * (long)bass_input)/  1023L;
    bass_pre_react /= (bass_stop - bass_start + 1);


    // Only react if we're greater than previous. 
    // NOTE: will decay over time if not greater - handled later after LEDs are written
    if(bass_pre_react + AMP_AMT > bass_react)
      bass_react = bass_pre_react + AMP_AMT;

    Serial.print("Bass: ");
    Serial.println(bass_pre_react);
  }
}


// Calc mid react from current audio input
void calcMidReact()
{
  long mid_input = 0;

  // Do low pass filter:
  // Add to input each of the mid frequencies
  for(int i = mid_start; i <= mid_stop; i++)
  {
    mid_input += max[i];
  }

  if(mid_input > MIN_FREQ)
  {
    // Get num of leds for mid
    // NOTE: max freq per channel is 1023. We can get the total max by multiplying this value by total frequencies in this channel
    long mid_pre_react = ((long)NUM_LEDS_PER_CHNL * (long)mid_input)/ 1023L;
    mid_pre_react /= (mid_stop - mid_start + 1);


    // Only react if we're greater than previous. 
    // NOTE: will decay over time if not greater - handled later after LEDs are written
    if(mid_pre_react + AMP_AMT > mid_react)
      mid_react = mid_pre_react + AMP_AMT;

    Serial.print("mid: ");
    Serial.println(mid_pre_react);
  }
}

// Calc treb react from current audio input
void calcTrebReact()
{
  long treb_input = 0;

  // Do low pass filter:
  // Add to input each of the treb frequencies
  for(int i = treb_start; i <= treb_stop; i++)
  {
    treb_input += max[i];
  }

  if(treb_input > MIN_FREQ)
  {
    // Get num of leds for treb
    // NOTE: max freq per channel is 1023. We can get the total max by multiplying this value by total frequencies in this channel
    long treb_pre_react = ((long)NUM_LEDS_PER_CHNL * (long)treb_input)/ 1023L;
    treb_pre_react /= (treb_stop - treb_start + 1);


    // Only react if we're greater than previous. 
    // NOTE: will decay over time if not greater - handled later after LEDs are written
    if(treb_pre_react + AMP_AMT > treb_react)
      treb_react = treb_pre_react + AMP_AMT;

    Serial.print("treb: ");
    Serial.println(treb_pre_react);
  }
}

void decayChannels()
{
  if(bass_react > 0)
    bass_react--;

  if(mid_react > 0)
    mid_react--;

  if(treb_react > 0)
    treb_react--;
}

// Lights up the LEDs.
// NOTE: sideffect - makes the party lit
void fireBurninOnTheDanceFloor()
{
  for(int i = NUM_LEDS - 1; i >= 0; i--) 
  {
    int led_val = (NUM_LEDS - 1) - i;
    
    if (led_val < bass_react)
      leds[i] = bass_color;
    else if (led_val >= bass_react && 
             led_val <  bass_react + mid_react)
      leds[i] = mid_color;
    else if(led_val >= mid_react && 
            led_val <  mid_react + treb_react)
      leds[i] = treb_color;
    else
      leds[i] = CRGB(0, 0, 0);      
  }
  FastLED.show(); 
}




// FUNCTION TO VISUALIZE WITH A SINGLE LEVEL
void singleLevel()
{
  readMSGEQ7();

  convertSingle();

  singleRainbow(); // APPLY COLOR

  k = k - wheel_speed; // SPEED OF COLOR WHEEL
  if (k < 0) // RESET COLOR WHEEL
    k = 255;

  // REMOVE LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
}


void danceFloor()
{
  // Read our audio input
  readMSGEQ7();

  // Calc our channel values
  calcBassReact();
  calcMidReact();
  calcTrebReact();

  // Light it up
  fireBurninOnTheDanceFloor();

  // Decay everything over time
  decayChannels();
}

void loop()
{  
  danceFloor();
}