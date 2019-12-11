#ifndef BASECHANGER_H
#define BASECHANGER_H

#include <FastLED.h>
#include "MusicData.h"

class Changer
{
    public:

        [[deprecated("Channels is no longer supported, now that MusicData contains all music data.")]]
        Changer(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) :
            leds(_leds),
            channels(_channels),
            minLED(_minLED),
            maxLED(_maxLED)
            {}

        [[deprecated("Channels is no longer supported, now that MusicData contains all music data.")]]
        Changer(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : 
            leds(_leds),
            channels(_channels),
            minLED(_minLED),
            maxLED(_maxLED),
            palette(_palette)
            {}

        Changer(CRGBSet _leds) : 
            leds(_leds)
            {}

        Changer(CRGBSet _leds, CRGBPalette16 _palette) : 
            leds(_leds),
            palette(_palette)
            {}

        virtual void step() {}

        void setPalette(CRGBPalette16);

    protected:

        CRGBSet leds;
        
        MusicData* music = MusicData::instance();
        
        [[deprecated("Channels is no longer supported, now that MusicData contains all music data.")]]
        int* channels;

        [[deprecated("Min/MaxLED no longer supported. Pass CRGBSet LED slice into constructor, instead.")]]
        int minLED;

        [[deprecated("Min/MaxLED no longer supported. Pass CRGBSet LED slice into constructor, instead.")]]
        int maxLED;

        [[deprecated("setAllPixInRange no longer supported. Use 'CRGBSet =' instead.")]]
        void setAllPixInRange(int minPix, int maxPix, CRGB color);

        CRGBPalette16 palette;
        
        CRGB getColorFromPalette(int colorIndex);

};

#endif