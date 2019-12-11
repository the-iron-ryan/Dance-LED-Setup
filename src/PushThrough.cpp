#include <FastLED.h>
#include "Changer.h"
#include "PushThrough.h"

#define MIN_THRESHOLD 300
#define LIGHT_DIM_INTERVAL 200
#define THRESH_DIM_INTERVAL 50

#define PUSH_SPEED 2
#define CORE_LENGTH 150

void PushThrough::step() {

    int maxChannel = getMaxChannel();

    setPulseColor(maxChannel);

    CRGB scaled = scalePulseBrightness(brightnessScalar / 1024.0);

//    Serial.print(scaled.r);
//    Serial.print('\t');
//    Serial.print(scaled.g);
//    Serial.print('\t');
//    Serial.print(scaled.b);
//    Serial.println();

//    leds(minLED, maxLED) = scaled;

    this->shiftPixDown(PUSH_SPEED);

    leds(leds.len - CORE_LENGTH, leds.len) = scaled;

    return;

}

void PushThrough::shiftPixDown(int speed)
{

    int next;
    CRGB nextColor;
    int offset = this->ticks % speed;
    this->ticks++;

    for (int i = offset; i < leds.len; i += speed)
    {

        next = i + speed;
        if (next >= leds.len)
        {
            next = leds.len - 1;
        }
        nextColor = leds[next + 1];

        this->leds(i, next).fill_solid(nextColor);

    }

}

int PushThrough::getMaxChannel()
{
    int max = 0;
    int maxChannel = 0;

    for (int i = 0; i < 7; i++)
    {
        if (music->current().channels[i] > max)
        {
            max = music->current().channels[i];
            maxChannel = i;
        }
    }

    return maxChannel;

}

CRGB PushThrough::getColorFromPaletteWeight()
{
    int weightedSum = 0;

    for (int i = 0; i < 7; i++)
    {
        weightedSum += music->current().channels[i] * (i + 1);
    }

    float weightedAvgChannel = weightedSum / 8;

    int paletteIndex = int(weightedAvgChannel * 255);

    return getColorFromPalette(paletteIndex);

}

void PushThrough::setPulseColor(int maxChannel)
{
    int maxAmplitude = music->current().channels[maxChannel];

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

CRGB PushThrough::scalePulseBrightness(float scale)
{
    CRGB scaled = CRGB(
        uint16_t(pulseColor.r * scale),
        uint16_t(pulseColor.g * scale),
        uint16_t(pulseColor.b * scale)
    );

    return scaled;
}