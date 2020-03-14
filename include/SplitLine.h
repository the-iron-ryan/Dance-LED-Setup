#ifndef SPLIT_LINE_H_
#define SPLIT_LINE_H_

#include "Changer.h"
#include <FastLED.h>

#define BUFFER_DEPTH 3
#define MIN_INTENSITY 100
#define MAX_INTENSITY 1023
#define DIMINISH_RATE 18

#define TOTAL_AMP 2

#define CHANNEL_AMP 1.5, \
                    1.5, \
                    1.33, \
                    1.0, \
                    1.33, \
                    1.66, \
                    2.5 
#define N_FUNC() (B - A + 1)
#define N N_FUNC()

/*
 * SplitLine - Same thing as the split spiral, but you know, in a straight line (the fuck you think this was?)
 * 
 *             Why you ask? Cuz spirals are for nerds
*/
template<int A, int B>
class SplitLine : public Changer
{
public:
    SplitLine(CRGBSet _leds) : Changer(_leds) { }

    /**
     * Completes one step of the changer's iteration
     */
    virtual void step() 
    {
        leds(0, leds.len) = CRGB::Black;

        // Grabbin current music averages and shit
        for (int channel = A; channel < B; channel++)
        {
            int avg = 0;
            for (int bufferFrameIndex = 0; bufferFrameIndex < BUFFER_DEPTH; bufferFrameIndex++)
            {
                avg += music->at(bufferFrameIndex).channels[channel];                
            }

            avg /= BUFFER_DEPTH;

            scratchFrame[channel - A] = avg;
        } 

        for (int lineIndex = 0; lineIndex < N; lineIndex++)
        {
            // Set our current threshold if we're greater. If not, begin reducin'
            if(scratchFrame[lineIndex] > thresholds[lineIndex])
            {
                thresholds[lineIndex] = scratchFrame[lineIndex];
            }
            else
            {
                thresholds[lineIndex] -= DIMINISH_RATE;
            }

            // If statements are for scrubs
            thresholds[lineIndex] = constrain(thresholds[lineIndex], MIN_INTENSITY, MAX_INTENSITY);


            // Calc respective color and intensity for this freq channel
            CRGB CurLineColor = getColorFromPalette((256 / N) * lineIndex);
            colors[lineIndex] = CurLineColor;


            float intensity = (thresholds[lineIndex] - MIN_INTENSITY)
                            / (1023.00               - MIN_INTENSITY);
            intensity *= TOTAL_AMP;
            intensity *= channelAmps[lineIndex + A];
            intensisties[lineIndex] = intensity;
        }


        // Thats where I draw the line
        int LEDLineSplit = round(leds.len / N);
        int nxtSplitIndex = LEDLineSplit;
        int curLineIndex = 0;

        // Paint dem LEDs 
        for(int LEDIndex = 0; LEDIndex < leds.len; LEDIndex++)
        {
            if(LEDIndex > nxtSplitIndex && curLineIndex < N - 1)
            {
                nxtSplitIndex += LEDLineSplit;
                curLineIndex++;
            }

            float curIntensity = constrain(intensisties[curLineIndex], 0.f, 0.99f);
            CRGB curCol = CRGB(
                (uint8_t)colors[curLineIndex].r * curIntensity,
                (uint8_t)colors[curLineIndex].g * curIntensity,
                (uint8_t)colors[curLineIndex].b * curIntensity);

            leds[LEDIndex] = curCol;
        }
    }

    /**
     * Called before the changer is activated.
     */
    virtual void init() {}

    virtual void stop() {}

private:

    CRGB colors[N];

    float intensisties[N];

    int scratchFrame[N];

    int thresholds[N];

    double channelAmps[7] = { CHANNEL_AMP };
};


#endif