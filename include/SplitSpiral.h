#ifndef SPLIT_SPIRAL_H_ 
#define SPLIT_SPIRAL_H_

#include "Changer.h"
#include <FastLED.h>

#define MAX_LAYERS 16

#define MIN_INTENSITY 100
#define DIMINISH_RATE 25
#define PARTIAL_LAYER_LIGHT_PROPORTION 10

#define BUFFER_DEPTH 4


template<int N>
class SplitSpiral : public Changer
{
public:
    SplitSpiral(CRGBSet _leds) : Changer(_leds) { initArcLengths(); } 

    virtual void step()
    {

        counter++;

        leds(0, leds.len) = CRGB::Black;

        // store channel data into next frame in buffer
        int bufferSlot = counter % BUFFER_DEPTH;
        for (int i = 0; i < N; i++)
        {
            channelBuffer[bufferSlot][i] = music->current().channels[i];
        }

        // compute channel averages and save them into the scratch buffer
        for (int chan = 0; chan < N; chan++)
        {

            int avg = 0;
            for (int bufferFrame = 0; bufferFrame < BUFFER_DEPTH; bufferFrame++)
            {
                avg += channelBuffer[bufferFrame][chan];                
            }

            avg /= BUFFER_DEPTH;

            // for (int i = 0; i < N; i++)
            // {
            //     Serial.print(scratchChannel[chan]);
            //     Serial.print('\t');
            // }
            // Serial.println();

            scratchChannel[chan] = avg;

        } 

        for (int wedge = 0; wedge < N; wedge++)
        {

            if (thresholds[wedge] < scratchChannel[wedge])
            {
                thresholds[wedge] = scratchChannel[wedge];
            }
            else if (thresholds[wedge] > 1023)
            { 
                // Clamp to 1023
                thresholds[wedge] = 1023;
            }
            else if (thresholds[wedge] < MIN_INTENSITY)
            {
                // Clamp to min intensity
                thresholds[wedge] = MIN_INTENSITY;
            }
            else if (thresholds[wedge] > MIN_INTENSITY)
            {
                thresholds[wedge] -= DIMINISH_RATE;
            }
            
            CRGB color = getColorFromPalette((256 / N) * wedge);
            float intensity = (thresholds[wedge] - MIN_INTENSITY)
                            / (1023.00         - MIN_INTENSITY);

            // Amplify intensity by amplifier variable, for balancing
            intensity *= totalAmp;
            intensity += intensity * wedge * slopeAmp;

            applyColorToWedge(wedge, color, intensity);

        }

        if (counter % 500 == 1)
        {
            Serial.println();
            for (int i = 0; i < N; i++)
            {
                Serial.print(thresholds[i]);
                Serial.print('\t');
            }
            Serial.println();
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

    float totalAmp = 1.5;
    float slopeAmp = 0.2;

    int channelBuffer[BUFFER_DEPTH][N];
    int scratchChannel[N];

    long counter = 0;

};
#endif