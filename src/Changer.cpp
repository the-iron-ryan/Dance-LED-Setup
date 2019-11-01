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

// VERY SLOW
void Changer::interpBetweenPixels(int minPix, int maxPix)
{

    CRGB firstColor  = this->leds[minPix];
    CRGB secondColor = this->leds[maxPix];

    this->interpBetweenPixels(minPix, maxPix, firstColor, secondColor);

}

// VERY SLOW
void Changer::interpBetweenPixels(int minPix, int maxPix, CRGB firstColor, CRGB secondColor)
{

    int dist = maxPix - minPix;
    float redDiff = (firstColor.red - secondColor.red) / dist;
    float greenDiff = (firstColor.green - secondColor.green) / dist;
    float blueDiff = (firstColor.blue - secondColor.blue) / dist;

    for (int i = 1; i < dist; i++)
    {
        this->leds[minPix + i] = CRGB(
            firstColor.red + (i * redDiff),
            firstColor.green + (i * greenDiff),
            firstColor.blue + (i * blueDiff)
        );
    }

}

int* Changer::squeezeToThreeChannels()
{

    // Avg 8-track into 3 channels
    int bass = ( this->channels[0] + this->channels[1] + this->channels[2] ) / 3;
    int mid  = ( this->channels[3] + this->channels[4]                     ) / 2;
    int treb = ( this->channels[5] + this->channels[6]                     ) / 2;

    // Crunch numbers into range from [0, 255]
    static int result[3];
    result[0] = (bass * 256) / 1023;
    result[1] = (mid  * 256) / 1023;
    result[2] = (treb * 256) / 1023;

    return result;

}

void Changer::setPalette(CRGBPalette16 _palette)
{
    this->palette = _palette;
}

CRGB Changer::getColorFromPalette(int colorIndex)
{
    return ColorFromPalette(this->palette, colorIndex);
}