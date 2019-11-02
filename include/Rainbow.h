#ifndef RAINBOW_H
#define RAINBOW_H

#include "Changer.h"

class Rainbow: public Changer
{
    public:

        Rainbow(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) {}
        Rainbow(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:

        long tick = 0L;

};

#endif