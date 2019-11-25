#include <FastLED.h>
#include "Changer.h"
#include "PullThrough.h"

#define DIM_LIGHT_AMOUNT 1
#define DIM_THRESH_AMOUNT 20
#define MINIMUM_DIM 5
#define MINIMUM_THRESH 250
#define STARTING_PIXEL 200

#define STEP 4

void PullThrough::step() {

    CRGB color = pickColor();
    pullColorThrough(color);

    return;

}

void PullThrough::pullColorThrough(CRGB color)
{

//    tick++;
//    int offset = tick % STEP;
//
//    for (int i = minLED + offset; i < maxLED; i += STEP)
//    {
//
//        int next = i + STEP;
//        if (next >= maxLED)
//        {
//            next = maxLED - 1;
//        }
//        CRGB nextColor = leds[next + 1];
//
//        leds(i, next).fill_solid(nextColor);
//
//    }

    leds(maxLED - STARTING_PIXEL , maxLED).fill_solid(color);

}

CRGB PullThrough::pickColor()
{

    CRGB pulseColor;
    
    int maxChannel = this->getMaxChannel();     
    int channelVal = channels[maxChannel];

    if (channelVal > threshold)
    {
        threshold = channelVal;

        float strength = channelVal / 1024.0;

        pulseColor = getColorFromPalette( (maxChannel * 32) + 1 );

        pulseColor = CRGB(
            int(pulseColor.r * strength),
            int(pulseColor.b * strength),
            int(pulseColor.g * strength)
        );
    }
    else if (threshold > MINIMUM_THRESH)
    {
        threshold -= DIM_THRESH_AMOUNT;

        int rPulse = pulseColor.r - DIM_LIGHT_AMOUNT;
        if (rPulse < MINIMUM_DIM)
        {
            rPulse = MINIMUM_DIM;
        }

        int gPulse = pulseColor.g - DIM_LIGHT_AMOUNT;
        if (gPulse < MINIMUM_DIM)
        {
            gPulse = MINIMUM_DIM;
        }

        int bPulse = pulseColor.b - DIM_LIGHT_AMOUNT;
        if (bPulse < MINIMUM_DIM)
        {
            bPulse = MINIMUM_DIM;
        }

        pulseColor = CRGB(
            rPulse,
            gPulse,
            bPulse
        );
    }
    else
    {
        pulseColor = CRGB::Black;
    }

    return pulseColor;

}

int PullThrough::getMaxChannel()
{
    int result = 0;
    int largest = 0;

    for (int i = 0; i < 7; i++)
    {
        if (channels[i] > largest)
        {
            result = i;
            largest = channels[i];
        }
    }

    return result;
}