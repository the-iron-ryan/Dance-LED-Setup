#ifndef BASSPUSH_H
#define BASSPUSH_H

#include "Changer.h"

class BassPush: public Changer
{
    public:

        BassPush(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) {}
        BassPush(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:

        void shiftPixDown(int speed);

        int squeezeChannelsToInt(int bass, int mid, int treb);
        void adjustPulseBrightness();

        long ticks = 0;

};

#endif