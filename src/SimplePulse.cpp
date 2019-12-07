#include <FastLED.h>
#include "Changer.h"
#include "SimplePulse.h"

#define MIN_THRESHOLD 300
#define LIGHT_DIM_INTERVAL 100
#define THRESH_DIM_INTERVAL 10

#define NUM_CHANNELS 1

void SimplePulse::step() {

    int maxChannel = getMaxChannel();

    setPulseColor(maxChannel);

    CRGB scaled = scalePulseBrightness(brightnessScalar / 1024.0);

//    Serial.print(scaled.r);
//    Serial.print('\t');
//    Serial.print(scaled.g);
//    Serial.print('\t');
//    Serial.print(scaled.b);
//    Serial.println();

    leds(0, minLED) = CRGB::Black;
    leds(minLED, maxLED) = scaled;

    return;

}

int SimplePulse::getMaxChannel()
{
    int max = 0;
    int maxChannel = 0;

    for (int i = 0; i < 7; i++)
    {
        if (channels[i] > max)
        {
            max = channels[i];
            maxChannel = i;
        }
    }

    return maxChannel;

}

CRGB SimplePulse::getColorFromPaletteWeight()
{
    int weightedSum = 0;

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        weightedSum += channels[i] * (i + 1);
    }

    float weightedAvgChannel = weightedSum / (NUM_CHANNELS + 1);

    int paletteIndex = int(weightedAvgChannel * 255);

    return getColorFromPalette(paletteIndex);

}

void SimplePulse::setPulseColor(int maxChannel)
{
    int maxAmplitude = channels[maxChannel];

    if (maxAmplitude > threshold)
    {
        threshold = maxAmplitude;
        brightnessScalar = maxAmplitude;
        pulseColor = getColorFromPaletteWeight();
    }
    else if (maxAmplitude > MIN_THRESHOLD)
    {
        threshold -= THRESH_DIM_INTERVAL;
        brightnessScalar -= LIGHT_DIM_INTERVAL;

        if (threshold < 0)
        {
            threshold = 0;
        }

        if (brightnessScalar < 0)
        {
            brightnessScalar = 0;
        }

    }
    else
    {
        pulseColor = CRGB::Black;
    }
}

CRGB SimplePulse::scalePulseBrightness(float scale)
{
    CRGB scaled = CRGB(
        uint16_t(pulseColor.r * scale),
        uint16_t(pulseColor.g * scale),
        uint16_t(pulseColor.b * scale)
    );

    return scaled;
}