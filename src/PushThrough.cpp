#include <FastLED.h>
#include "Changer.h"
#include "PushThrough.h"

void movePixDown(CRGB* pix, int min, int max)
{
    for (int i = max; i > min; i--)
    {
        pix[i - 1] = pix[i];
    }
}


void PushThrough::step() {

    // Avg 8-track into 3 channels
    int bass = ( this->channels[0] + this->channels[1]                     ) / 2;
    int mid  = ( this->channels[2] + this->channels[3] + this->channels[4] ) / 3;
    int treb = ( this->channels[5] + this->channels[6] + this->channels[7] ) / 3;

    // Crunch numbers into range from [0, 255]
    bass = (bass * 256) / 1023;
    mid  = (mid  * 256) / 1023;
    treb = (bass * 256) / 1023;

    CRGB newColor(mid, treb, bass);

    movePixDown(leds, this->minLED, this->maxLED);

    leds[this->maxLED] = newColor;

    return;

}


