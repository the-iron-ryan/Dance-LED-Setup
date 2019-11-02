#ifndef SAMPLECHANGER_H
#define SAMPLECHANGER_H

#include "Changer.h"

class SampleChanger: public Changer
{
    public:

        SampleChanger(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) :
            Changer(_channels, _leds, _minLED, _maxLED) {}

        SampleChanger(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) :
            Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:


};

#endif