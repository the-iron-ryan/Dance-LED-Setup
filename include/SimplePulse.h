#ifndef SIMPLEPULSE_H
#define SIMPLEPULSE_H

#include "Changer.h"

class SimplePulse: public Changer
{
    public:

        SimplePulse(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) {}
        SimplePulse(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:

        int squeezeChannelsToInt(int bass, int mid, int treb);
        void adjustPulseBrightness();

};

#endif