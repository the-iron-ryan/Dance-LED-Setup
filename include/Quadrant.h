#ifndef QUADRANT_H_
#define QUADRANT_H_

#include "Changer.h"
#include <FastLED.h>



class Quadrant : public Changer
{
public:
    Quadrant(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) { initArcLengths(); } 
    Quadrant(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) { initArcLengths(); } 

    virtual void step();

private:
    void initArcLengths();
    double arsinh(double x);
    double calcArcLen(double thetaStrt, double thetaStop);
    void calcQuadFreqs();
    void reduce();

    // r = C * theta (equation for spiral)
    double C = 4.5;

    // Pi indexes
    double quadIndexes[17];

    double quadFreqPercentage[4];
    double quadFreqMaxes[4];

    double ledLen = 1.65;
    double percentInc = 0.24;

    double decayRate = 0.35;
    double greaterThresh = 0.1;
};
#endif