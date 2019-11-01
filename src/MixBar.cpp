#include <FastLED.h>
#include "Changer.h"
#include "MixBar.h"

#define SAMPLE_INDEX 32
#define SAMPLE_2 64
#define SAMPLE_3 96
#define SAMPLE_4 128
#define SAMPLE_5 160
#define SAMPLE_6 192
#define SAMPLE_7 224
#define SAMPLE_8 255

const int paletteIndex[] = { 0, 32, 64, 96, 128, 160, 192, 224, 255 };

void MixBar::step() {

    int numPixels = this->maxLED - this->minLED;
    int maxPixPerSection = numPixels / 7;
    int sectionBound = this->maxLED;

    float percentFilled;
    int sectionFilled;

    CRGB sectionColor;

    for (int i = 6; i >= 0; i--)
    {
        percentFilled = this->channels[i] / 1024.0;
        sectionFilled = (int)(maxPixPerSection * percentFilled);

        sectionColor = getColorFromPalette(paletteIndex[i]);

        this->setAllPixInRange(sectionBound - sectionFilled, sectionBound, sectionColor);
        sectionBound = sectionBound - sectionFilled;
    }

    this->setAllPixInRange(this->minLED, sectionBound, CRGB::Black);

    return;

}