#ifndef BASECHANGER_H
#define BASECHANGER_H

#include <FastLED.h>

class BaseChanger
{
    public:
        
        BaseChanger(int* _channels, CRGB* _leds) : leds(_leds), channels(_channels) {}

        BaseChanger& operator++(int n)
        {
            this->step();
            return *this;
        }

        virtual void step() = 0;

    protected:

        CRGB* leds;
        int*  channels;
        
};

#endif