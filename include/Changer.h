#ifndef BASECHANGER_H
#define BASECHANGER_H

#include <FastLED.h>
#include "Palettes.h"

class Changer
{
    public:

        Changer(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) :
            leds(_leds),
            channels(_channels),
            minLED(_minLED),
            maxLED(_maxLED)
            {}

        Changer(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : 
            leds(_leds),
            channels(_channels),
            minLED(_minLED),
            maxLED(_maxLED),
            palette(_palette)
            {}

        virtual void step() {}

        void setPalette(CRGBPalette16);

    protected:

        CRGBSet leds;
        int* channels;
        int minLED;
        int maxLED;

        CRGBPalette16 palette;
        
        void setAllPixInRange(int minPix, int maxPix, CRGB color);
        void interpBetweenPixels(int minPix, int maxPix);
        void interpBetweenPixels(int minPix, int maxPix, CRGB firstColor, CRGB secondColor);
        int* squeezeToThreeChannels();
        CRGB getColorFromPalette(int colorIndex);

};

#endif