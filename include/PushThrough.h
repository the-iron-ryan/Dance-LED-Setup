#ifndef PUSHTHROUGH_H
#define PUSHTHROUGH_H

#include "Changer.h"

class PushThrough: public Changer
{
    public:

        PushThrough(int* _channels, CRGB* _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) {}

        virtual void step();

};

#endif