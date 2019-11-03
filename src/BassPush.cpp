#include <FastLED.h>
#include "Changer.h"
#include "BassPush.h"

#define BASS_PROPORTION 1.0
#define MID_PROPORTION 0.0
#define TREB_PROPORTION 0.0

#define PUSH_SPEED 8

void BassPush::step() {

    CRGB newColor = this->leds[this->maxLED];

    int ogBrightness = this->maxBrightness;
    this->adjustPulseBrightness();
    
    if (ogBrightness < this->maxBrightness)
    {
        int* threeChannel = this->squeezeToThreeChannels();
        int colorIndex = this->squeezeChannelsToInt(threeChannel[0], threeChannel[1], threeChannel[2]);

        newColor = this->getColorFromPalette(colorIndex);
    }

    newColor.r *= maxBrightness;
    newColor.g *= maxBrightness;
    newColor.b *= maxBrightness;
    

    this->shiftPixDown(PUSH_SPEED);

    leds[this->maxLED] = newColor;

    return;

}

void BassPush::shiftPixDown(int speed)
{

    int next;
    CRGB nextColor;
    int offset = this->ticks % speed;
    this->ticks++;

    for (int i = this->minLED + offset; i < this->maxLED; i += speed)
    {

        next = i + speed;
        if (next >= this->maxLED)
        {
            next = maxLED - 1;
        }
        nextColor = leds[next + 1];

        this->leds(i, next).fill_solid(nextColor);

    }

}

int BassPush::squeezeChannelsToInt(int bass, int mid, int treb)
{

    int bassProp = (int)(bass * BASS_PROPORTION);
    int midProp  = (int)(mid  * MID_PROPORTION);
    int trebProp = (int)(treb * TREB_PROPORTION);

    return bassProp + midProp + trebProp;

}

void BassPush::adjustPulseBrightness()
{

    int avg = 0;
    for(int i = 0; i < 2; i++)
    {
        avg += this->channels[i];
    }
    avg = avg / 2;
    
    if (avg > 800)
    {
        this->maxBrightness = 1.0;
    }
    else
    {
        this->maxBrightness = 0;
    }

}
