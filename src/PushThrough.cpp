#include <FastLED.h>
#include "BaseChanger.h"
#include "PushThrough.h"

void PushThrough::step() {

    int bass =  ( this->channels[0] + this->channels[1]                     ) / 2;
    int mid  =  ( this->channels[2] + this->channels[3] + this->channels[4] ) / 3;
    int treb =  ( this->channels[5] + this->channels[6] + this->channels[7] ) / 3;

    return;
}


