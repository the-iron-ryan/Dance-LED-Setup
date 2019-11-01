#include <FastLED.h>
#include "Changer.h"
#include "PushThrough.h"


void PushThrough::step() {

    int* threeChannel = this->squeezeToThreeChannels();

    CRGB newColor(threeChannel[0], threeChannel[1], threeChannel[2]);

    this->shiftPixDown(8);

    leds[this->maxLED] = newColor;

    return;

}

void PushThrough::shiftPixDown(int speed)
{

    int next;
    CRGB nextColor;
    int offset = this->ticks % speed;

    for (int i = this->minLED + offset; i < this->maxLED; i += speed)
    {

        next = i + speed;
        if (next >= this->maxLED)
        {
            next = maxLED - 1;
        }
        nextColor = leds[next + 1];

        this->setAllPixInRange(i, next, nextColor);

    }


}
