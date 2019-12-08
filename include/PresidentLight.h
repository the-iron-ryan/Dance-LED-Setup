#ifndef PRESIDE8T_LIGHT_H_ 
#define PRESIDE8T_LIGHT_H_ 

#include "Changer.h"
#include <FastLED.h>


class PresidentLight : public Changer
{
public:
    PresidentLight(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) { initArcLengths(); } 
    PresidentLight(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) { initArcLengths(); } 

    virtual void step()
    {
        int curQuadIndex = 0;
        for (int i = maxLED - 1; i > 0; i--)
        {
            int LEDIndex = (maxLED - 1) - i;
            double size = ledLen * (double)LEDIndex; 

            // Update quad index if greater than curent arc size
            if (size > quadIndexes[curQuadIndex] && curQuadIndex < 8*4)
            {
                curQuadIndex++;
            }

            if (i < minLED)
            {
                leds[i] = CRGB::Black;
            }
            else
            {
                int index = curQuadIndex % 8;

                if(index == 3 || index == 4)
                {
                    leds[i] = CRGB::Orange;
                }
                else
                {
                    leds[i] = CRGB::Black;
                }
            }
        }
    }
private:
    void initArcLengths()
    {
        // Setup pi indexes using math science shit
        for (int i = 0; i < (8*4) + 1; i++)
        {
            quadIndexes[i] = calcArcLen(0, i * (2.0 * PI) / (double)8);
        }
    }
    double arsinh(double x)
    {
        return log(x + sqrt(pow(x, 2) + 1));
    }
    double calcArcLen(double thetaStrt, double thetaStop)
    {
        // Arc length of spiral, by taking End Length - Start Length
        return (C * ( (arsinh(thetaStop) / 2) + ( (thetaStop * sqrt( pow(thetaStop, 2) + 1 )) / 2 ) ) )
             - (C * ( (arsinh(thetaStrt) / 2) + ( (thetaStrt * sqrt( pow(thetaStrt, 2) + 1 )) / 2 ) ) );
    } 
    void calcQuadFreqs()
    {
        for (int i = 0; i < 8; i++)
        {
            quadFreqPercentage[i] = channels[i];
            quadFreqPercentage[i] /= (1023.0);

            quadFreqPercentage[i] = constrain(quadFreqPercentage[i], 0.0, 100.0);
        }
    }
    void reduce()
    {
        for (int i = 0; i < 8; i++)
        {
            if (quadFreqMaxes[i] > 0)
                quadFreqMaxes[i] -= decayRate;
        }
    }

    // r = C * theta (equation for spiral)
    double C = 4.5;

    // Pi indexes
    double quadIndexes[(8 * 4) + 1];

    double quadFreqPercentage[8];
    double quadFreqMaxes[8];

    double ledLen = 1.65;
    double percentInc = 0.24;

    double decayRate = 0.15;
    double greaterThresh = 0.0;
};
#endif