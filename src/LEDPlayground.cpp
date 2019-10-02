#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    300
#define LED_TYPE    NEOPIXEL

// LED data structur
CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
}

void loop() {
    // leds[0] = CRGB::Red;
    // FastLED.show();
    // delay(1000);

    // leds[0] = CRGB::Black;
    // FastLED.show();
    // delay(1000);

    for(int i = 0; i < NUM_LEDS; i++)
    {
        // Show Gray
        leds[i] = CRGB::Gray;
        FastLED.show();

        // clear at current position
        leds[i] = CRGB::Black;
        delay(30 - (30 * ((float)i/NUM_LEDS)));
    }
}