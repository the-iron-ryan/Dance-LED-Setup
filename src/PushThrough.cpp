#include <FastLED.h>
#include "Changer.h"
#include "PushThrough.h"

#define BASS_PROPORTION 0.33
#define MID_PROPORTION 0.33
#define TREB_PROPORTION 0.33

#define PUSH_SPEED 9

void PushThrough::step() {

    int* threeChannel = this->squeezeToThreeChannels();

    int colorIndex = this->squeezeChannelsToInt(threeChannel[0], threeChannel[1], threeChannel[2]);
    CRGB newColor = this->getColorFromPalette(colorIndex);

    this->shiftPixDown(PUSH_SPEED);

    leds[this->maxLED] = newColor;

    return;

}

void PushThrough::shiftPixDown(int speed)
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

int PushThrough::squeezeChannelsToInt(int bass, int mid, int treb)
{

    int bassProp = (int)(bass * BASS_PROPORTION);
    int midProp  = (int)(mid  * MID_PROPORTION);
    int trebProp = (int)(treb * TREB_PROPORTION);

    return bassProp + midProp + trebProp;

}
