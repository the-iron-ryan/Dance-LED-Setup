#ifndef SPLIT_SPIRAL_H_ 
#define SPLIT_SPIRAL_H_

#include "Changer.h"
#include <FastLED.h>


template<int N>
class SplitSpiral : public Changer
{
public:
    SplitSpiral(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) { initArcLengths(); } 
    SplitSpiral(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) { initArcLengths(); } 

    virtual void step()
    {
        calcQuadFreqs();

        int curQuadIndex = 0;
        for (int i = maxLED - 1; i > 0; i--)
        {
            int LEDIndex = (maxLED - 1) - i;
            double size = ledLen * (double)LEDIndex; 

            // Update quad index if greater than curent arc size
            if (size > quadIndexes[curQuadIndex] && curQuadIndex < N*4)
            {
                curQuadIndex++;
            }

            if (i < minLED)
            {
                leds[i] = CRGB::Black;
            }
            else
            {
                int index = curQuadIndex % N;
                double freqPercent = quadFreqPercentage[index];

                // Are we greater than last value? If so, set it
                if (freqPercent > (quadFreqMaxes[index] + greaterThresh))
                {
                    quadFreqMaxes[index] = freqPercent;
                }

                // Get percent value represented at this given turn of the spiralA
                int scaleMult = (curQuadIndex / N) + 1;
                double turnMaxPercent = scaleMult * percentInc;
                turnMaxPercent = min(1.0, turnMaxPercent);
                double val = turnMaxPercent - quadFreqMaxes[index];

                int increment = (255/N);
                uint8_t palIndex = ((index + 1) * increment) - 1;
                leds[i] = ColorFromPalette(palette, palIndex);

                if (val > 0.0)
                {
                    // Cap value at percent min, done so that we zero out channels
                    val = min(percentInc, val);
                    double relAmt = percentInc - val;
                    double percentFilled = relAmt / percentInc;

                    uint8_t fadeAmt = (1.0 - percentFilled) * 255;
                    leds[i].fadeToBlackBy(fadeAmt);
                }
            }
        }

        // reduce max values
        reduce();
    }
private:
    void initArcLengths()
    {
        // Setup pi indexes using math science shit
        for (int i = 0; i < (N*4) + 1; i++)
        {
            quadIndexes[i] = calcArcLen(0, i * (2.0 * PI) / (double)N);
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
        for (int i = 0; i < N; i++)
        {
            quadFreqPercentage[i] = channels[i];
            quadFreqPercentage[i] /= (1023.0);

            quadFreqPercentage[i] = constrain(quadFreqPercentage[i], 0.0, 100.0);
        }
    }
    void reduce()
    {
        for (int i = 0; i < N; i++)
        {
            if (quadFreqMaxes[i] > 0)
                quadFreqMaxes[i] -= decayRate;
        }
    }

    // r = C * theta (equation for spiral)
    double C = 4.5;

    // Pi indexes
    double quadIndexes[(N * 4) + 1];

    double quadFreqPercentage[N];
    double quadFreqMaxes[N];

    double ledLen = 1.65;
    double percentInc = 0.24;

    double decayRate = 0.50;
    double greaterThresh = 0.0;
};
#endif