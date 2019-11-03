#include <FastLED.h>
#include "Changer.h"
#include "SimplePulse.h"

#define BASS_PROPORTION 0.25
#define MID_PROPORTION 0.25
#define TREB_PROPORTION 0.50

void scaleLEDs(CRGBSet pix, float scale)
{

//    Serial.print(pix[500].r);
//    Serial.print('\t');
//    Serial.print(pix[500].g);
//    Serial.print('\t');
//    Serial.print(pix[500].b);
//    Serial.println();

    for(CRGB & pixel : pix)
    {
        pixel.r = (uint8_t)(pixel.r * scale);
        pixel.g = (uint8_t)(pixel.g * scale);
        pixel.b = (uint8_t)(pixel.b * scale);
    }

//    Serial.print(pix[500].r);
//    Serial.print('\t');
//    Serial.print(pix[500].g);
//    Serial.print('\t');
//    Serial.print(pix[500].b);
//    Serial.println();

}

void SimplePulse::step() {

    this->leds(0, this->minLED).fill_solid(CRGB::Black);

    int ogBrightness = this->maxBrightness;
    calcDampenFactor();
    
    if (ogBrightness < this->maxBrightness)
    {
        int* threeChannel = this->squeezeToThreeChannels();
        int colorIndex = this->squeezeChannelsToInt(threeChannel[0], threeChannel[1], threeChannel[2]);

        this->leds(this->minLED, this->maxLED).fill_solid(this->getColorFromPalette(colorIndex));
    }

    scaleLEDs(leds, this->maxBrightness);

    return;

}

int SimplePulse::squeezeChannelsToInt(int bass, int mid, int treb)
{

    int bassProp = (int)(bass * BASS_PROPORTION);
    int midProp  = (int)(mid  * MID_PROPORTION);
    int trebProp = (int)(treb * TREB_PROPORTION);

    return bassProp + midProp + trebProp;

}

void SimplePulse::calcDampenFactor()
{

    int avg = 0;
    for(int i = 0; i < 7; i++)
    {
        avg += this->channels[i];
    }
    avg = avg / 7;

    float newBrightness = avg / 1024.0;

    this->maxBrightness *= 0.75;

    if (newBrightness > this->maxBrightness) {
        this->maxBrightness = newBrightness;
    }

    if (this->maxBrightness < 0.1)
    {
        this->maxBrightness = 0.1;
    }

    // Serial.println(this->maxBrightness);

}
