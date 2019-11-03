#include <FastLED.h>
#include "Changer.h"
#include "SimplePulse.h"

#define BASS_PROPORTION 0.25
#define MID_PROPORTION 0.25
#define TREB_PROPORTION 0.50


void SimplePulse::step() {

    this->leds(0, this->minLED).fill_solid(CRGB::Black);

    int ogBrightness = this->maxBrightness;
    this->adjustPulseBrightness();
    
    if (ogBrightness < this->maxBrightness)
    {
        int* threeChannel = this->squeezeToThreeChannels();
        int colorIndex = this->squeezeChannelsToInt(threeChannel[0], threeChannel[1], threeChannel[2]);

        this->leds(this->minLED, this->maxLED).fill_solid(this->getColorFromPalette(176));
    }

    this->scaleLEDs(this->maxBrightness);

    return;

}

int SimplePulse::squeezeChannelsToInt(int bass, int mid, int treb)
{

    int bassProp = (int)(bass * BASS_PROPORTION);
    int midProp  = (int)(mid  * MID_PROPORTION);
    int trebProp = (int)(treb * TREB_PROPORTION);

    return bassProp + midProp + trebProp;

}

void SimplePulse::adjustPulseBrightness()
{

    int avg = 0;
    for(int i = 0; i < 2; i++)
    {
        avg += this->channels[i];
    }
    avg = avg / 2;

    float newBrightness = avg / 1024.0;

    this->maxBrightness *= 0.60;

    if (newBrightness > this->maxBrightness) {
        this->maxBrightness = newBrightness;
    }

    if (this->maxBrightness < 0.2)
    {
        this->maxBrightness = 0.2;
    }

    // Serial.println(this->maxBrightness);

}
