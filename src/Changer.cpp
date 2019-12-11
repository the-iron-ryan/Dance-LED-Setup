#include <FastLED.h>
#include "Changer.h"
        
void Changer::setAllPixInRange(int minPix, int maxPix, CRGB color)
{

    for (int i = minPix; i < maxPix; i++)
    {
        this->leds[i] = color;
    }
    return;

}

void Changer::setPalette(CRGBPalette16 _palette)
{
    this->palette = _palette;
}

CRGB Changer::getColorFromPalette(int colorIndex)
{
    return ColorFromPalette(this->palette, colorIndex);
}