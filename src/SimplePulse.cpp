#include <FastLED.h>
#include "Changer.h"
#include "SimplePulse.h"

#define BASS_PROPORTION 0.75
#define MID_PROPORTION 0.125
#define TREB_PROPORTION 0.125

void SimplePulse::step() {

    int* threeChannel = this->squeezeToThreeChannels();

    int colorIndex = this->squeezeChannelsToInt(threeChannel[0], threeChannel[1], threeChannel[2]);

    this->setAllPixInRange(this->minLED, this->maxLED, this->getColorFromPalette(colorIndex));

    return;

}

int SimplePulse::squeezeChannelsToInt(int bass, int mid, int treb)
{

    int bassProp = (int)(bass * BASS_PROPORTION);
    int midProp  = (int)(mid  / MID_PROPORTION);
    int trebProp = (int)(treb / TREB_PROPORTION);

    return bassProp + midProp + trebProp;

}
