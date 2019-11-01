#ifndef MIXBAR_H
#define MIXBAR_H

#include "Changer.h"

class MixBar: public Changer
{
    public:

        MixBar(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) :
            Changer(_channels, _leds, _minLED, _maxLED) {}

        MixBar(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) :
            Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:


};

#endif