#include <FastLED.h>
#include "Changer.h"
        
void Changer::setAllPixInRange(int minPix, int maxPix, CRGB color)
{

    for (int i = minPix; i <= maxPix; i++)
    {
        this->leds[i] = CRGB(color);
    }
    return;

}