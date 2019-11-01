#ifndef BASECHANGER_H
#define BASECHANGER_H

#include <FastLED.h>

class Changer
{
    public:
        
        Changer(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : leds(_leds), channels(_channels), minLED(_minLED), maxLED(_maxLED) {}

        Changer& operator++(int n)
        {
            this->step();
            return *this;
        }

        virtual void step() {}

    protected:

        CRGBSet leds;
        int minLED;
        int maxLED;

        int* channels;
        
        void setAllPixInRange(int minPix, int maxPix, CRGB color);

        void interpBetweenPixels(int minPix, int maxPix);

        void interpBetweenPixels(int minPix, int maxPix, CRGB firstColor, CRGB secondColor);

        int* squeezeToThreeChannels();

};

#endif