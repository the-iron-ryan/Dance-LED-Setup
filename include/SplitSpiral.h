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

        // Represents the part of the spiral we're in (0 - N)
        int curSectorIndex = 0;

        for (int i = maxLED - 1; i > 0; i--)
        {
            int LEDIndex = (maxLED - 1) - i;
            float size = ledLen * (float)LEDIndex; 

            // Update spiral index if greater than curent arc size
            if (size > spiralSectorIndexes[curSectorIndex] && curSectorIndex < N*4)
            {
                curSectorIndex++;
            }

            if (i < minLED)
            {
                leds[i] = CRGB::Black;
            }
            else
            {
                // Represents which section we're currently inside of (0 - N)
                int spiralSectionIndex = curSectorIndex % N;
                float freqPercent = sectionFreqPercentage[spiralSectionIndex];

                // Are we greater than last value? If so, set it
                if (freqPercent > (sectionFreqMaxes[spiralSectionIndex] + greaterThresh))
                {
                    sectionFreqMaxes[spiralSectionIndex] = freqPercent;
                }

                // Get percent value represented at this given turn of the spiralA
                int scaleMult = (curSectorIndex / N) + 1;
                float turnMaxPercent = scaleMult * percentInc;
                turnMaxPercent = min(1.0, turnMaxPercent);
                float val = turnMaxPercent - sectionFreqMaxes[spiralSectionIndex];

                int increment = (255/N);
                uint8_t palIndex = ((spiralSectionIndex + 1) * increment) - 1;
                leds[i] = ColorFromPalette(palette, palIndex);

                if (val > 0.0)
                {
                    // Cap value at percent min, done so that we zero out channels
                    val = min(percentInc, val);
                    float relAmt = percentInc - val;
                    float percentFilled = relAmt / percentInc;

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
            spiralSectorIndexes[i] = calcArcLen(0, i * (2.0 * PI) / (float)N);
        }
    }
    float arsinh(float x)
    {
        return log(x + sqrt(pow(x, 2) + 1));
    }
    float calcArcLen(float thetaStrt, float thetaStop)
    {
        // Arc length of spiral, by taking End Length - Start Length
        return (C * ( (arsinh(thetaStop) / 2) + ( (thetaStop * sqrt( pow(thetaStop, 2) + 1 )) / 2 ) ) )
             - (C * ( (arsinh(thetaStrt) / 2) + ( (thetaStrt * sqrt( pow(thetaStrt, 2) + 1 )) / 2 ) ) );
    } 
    void calcQuadFreqs()
    {
        for (int i = 0; i < N; i++)
        {
            channels[i] = constrain(channels[i], 0, 1023);
            sectionFreqPercentage[i] = (float) channels[i] / 1023.0;
            sectionFreqPercentage[i] = constrain(sectionFreqPercentage[i], 0.0, 1.0);
        }
    }
    void reduce()
    {
        for (int i = 0; i < N; i++)
        {
            if (sectionFreqMaxes[i] > 0.0)
                sectionFreqMaxes[i] -= decayRate;
        }
    }

    // r = C * theta (equation for spiral)
    float C = 4.5;

    // Pi indexes
    float spiralSectorIndexes[(N * 4) + 1];

    float sectionFreqPercentage[N];
    float sectionFreqMaxes[N];

    float ledLen = 1.65;
    float percentInc = 0.24;

    float decayRate = 0.15;
    float greaterThresh = 0.0;
};
#endif