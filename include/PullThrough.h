#ifndef PULLTHROUGH_H
#define PULLTHROUGH_H

#include "Changer.h"

class PullThrough: public Changer
{
    public:

        PullThrough(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) :
            Changer(_channels, _leds, _minLED, _maxLED) {}

        PullThrough(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) :
            Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:

        long tick = 0;

        void pullColorThrough(CRGB color);

        int threshold = 0;

        CRGB pickColor();

        int getMaxChannel();


};

#endif