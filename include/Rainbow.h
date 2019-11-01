#ifndef RAINBOW_H
#define RAINBOW_H

#include "Changer.h"

class Rainbow: public Changer
{
    public:

        Rainbow(int* _channels, CRGB* _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) {}

        virtual void step();

    private:

        long tick = 0L;

};

#endif