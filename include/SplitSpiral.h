#ifndef SPLIT_SPIRAL_H_ 
#define SPLIT_SPIRAL_H_

#include "Changer.h"
#include <FastLED.h>

#define MAX_LAYERS 16

#define MIN_INTENSITY 100
#define DIMINISH_RATE 23
#define PARTIAL_LAYER_LIGHT_PROPORTION 10

#define BUFFER_DEPTH 3

#define TOTAL_AMP 2

#define CHANNEL_0_AMP 1.5
#define CHANNEL_1_AMP 1.5
#define CHANNEL_2_AMP 1.33
#define CHANNEL_3_AMP 1.0
#define CHANNEL_4_AMP 1.33
#define CHANNEL_5_AMP 1.66
#define CHANNEL_6_AMP 2.5

#define N_FUNC() (B - A + 1)
#define N N_FUNC()

template<int A, int B>
class SplitSpiral : public Changer
{
public:
    SplitSpiral(CRGBSet _leds) : Changer(_leds) { initArcLengths(); } 

    virtual void init()
    {
        startWedge++;
    }

    virtual void step()
    {

        leds(0, leds.len) = CRGB::Black;

        // compute channel averages and save them into the scratch buffer
        for (int channel = A; channel <= B; channel++)
        {

            int avg = 0;
            for (int bufferFrameIndex = 0; bufferFrameIndex < BUFFER_DEPTH; bufferFrameIndex++)
            {
                avg += music->at(bufferFrameIndex).channels[channel];                
            }

            avg /= BUFFER_DEPTH;

            // for (int i = 0; i < N; i++)
            // {
            //     Serial.print(scratchFrame[frame]);
            //     Serial.print('\t');
            // }
            // Serial.println();

            scratchFrame[channel - A] = avg;

        } 

//        for (int i = 0; i < N; i++)
//        {
//            Serial.print(scratchFrame[i]);
//            Serial.print('-');
//            Serial.print(thresholds[i]);
//            Serial.print('\t');
//        }
//        Serial.println();

        for (int wedge = 0; wedge < N; wedge++)
        {

            if (scratchFrame[wedge] >= thresholds[wedge])
            {
                thresholds[wedge] = scratchFrame[wedge];
            }
            else
            {
                thresholds[wedge] -= DIMINISH_RATE;
            }

            // Clamp values
            if (thresholds[wedge] > 1023)
            { 
                // Clamp to 1023
                thresholds[wedge] = 1023;
            }
            else if (thresholds[wedge] < MIN_INTENSITY)
            {
                // Clamp to min intensity
                thresholds[wedge] = MIN_INTENSITY;
            }
            
            CRGB color = getColorFromPalette((256 / N) * wedge);
            float intensity = (thresholds[wedge] - MIN_INTENSITY)
                            / (1023.00         - MIN_INTENSITY);

            // Amplify intensity by amplifier variable, for balancing
            intensity *= TOTAL_AMP;
            intensity *= channelAmps[wedge + A];

            applyColorToWedge((wedge + startWedge) % N, color, intensity);

        }

    }
private:

    void initArcLengths()
    {

        // Setup pi indexes using math science shit
        // - Ryan Dougherty, #1 software partner

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < MAX_LAYERS; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    channelArcs[i][j][k] = 0;
                }
            }
        }

        int currentLED = 0;
        int nextLED    = 0;

        float currentAngle = 0;

        int currentSection = 0;
        int currentLayer = 0;

        while (currentLED <= leds.len)
        {

            // Determine the run length for this section
            int runLength = calcArcLen(currentAngle, currentAngle + WEDGE_ANGLE);
            int runLengthInLEDs = runLength / LED_SPACING;

            // Next LED is the current one plus the run length
            nextLED = int(currentLED + runLengthInLEDs);

            // TEST: See if an OOB index is about to be added to the arc array
            if    (currentLED >= leds.len
                || currentLED < 0
                || nextLED    >= leds.len
                || nextLED    < 0)
            {
                Serial.println("Bad data added to arcs!");
            }

            // Beginning and end LEDs are now set - add the interval to the channel's layers
            channelArcs[currentSection][currentLayer][0] = leds.len - nextLED;
            channelArcs[currentSection][currentLayer][1] = leds.len - currentLED;

            // Set state for next iteration.
            currentAngle = currentAngle + WEDGE_ANGLE;

            // If the section is on its' final slice, increment to the next layer.
            if (currentSection == N - 1)
            {
                currentLayer++;
            }

            // Increment the section
            currentSection = (currentSection + 1) % N;

            // Set the current LED to the start of the next length
            currentLED = nextLED;

        }

        maxLayer = currentLayer - 1;
        // Serial.print("Max layer: ");
        // Serial.println(maxLayer);
        
        Serial.println("Spiral calculated!");

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

    void applyColorToWedge(int wedge, CRGB& color, float intensity)
    {

        // Clamp intensity from 0% - 100%
        if (intensity < 0.00)
            intensity = 0.00;

        else if (intensity > 0.99)
            intensity = 0.99;

        float litLayers = intensity * maxLayer;
        int numFullyLit = int(litLayers);
        float partialLayerIntensity = litLayers - numFullyLit;

        // Smooth the partial layer's intensity, so that it isn't so jittery in the <0.5 range.
        partialLayerIntensity = roundToNearestProportion(partialLayerIntensity, PARTIAL_LAYER_LIGHT_PROPORTION);

        // Serial.println(intensity);
        // Serial.println(litLayers);
        // Serial.println(numFullyLit);
        // Serial.println(partialLayerIntensity);
        // Serial.println();

        for (int i = 0; i <= numFullyLit; i++)
        {
            setWedgeLayerToColor(wedge, i, color);
        }

        if (numFullyLit < maxLayer)
        {
            CRGB dimmedColor = CRGB(
                uint8_t(color.r * partialLayerIntensity),
                uint8_t(color.g * partialLayerIntensity),
                uint8_t(color.b * partialLayerIntensity)
            );

            setWedgeLayerToColor(wedge, numFullyLit + 1, dimmedColor);
        }

    }

    void setWedgeLayerToColor(int wedge, int layer, CRGB& color)
    {
        leds(channelArcs[wedge][layer][0], channelArcs[wedge][layer][1]) = color;
    }

    float roundToNearestProportion(float num, int proportion)
    {
        return float(int(num * proportion)) / proportion;
    }

    bool initialized = false;

    // r = C * theta (equation for spiral)
    float C = 4.5;
    float WEDGE_ANGLE = 2*M_PI / N;
    float LED_SPACING = 1.65;

    // Pi indexes
    float spiralSectorIndexes[(N * 4) + 1];

    // Int for the max used layer within the channel arcs. 
    int maxLayer = 0;

    // Stored channel portions
    int channelArcs[N][MAX_LAYERS][2];

    int thresholds[N];

    int scratchFrame[N];

    int startWedge = 0;

    double channelAmps[7] = {
        CHANNEL_0_AMP,
        CHANNEL_1_AMP,
        CHANNEL_2_AMP,
        CHANNEL_3_AMP,
        CHANNEL_4_AMP,
        CHANNEL_5_AMP,
        CHANNEL_6_AMP
    };

};
#endif