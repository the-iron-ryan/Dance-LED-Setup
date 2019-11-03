#include "Quadrant.h"
#include <math.h>


void Quadrant::step() 
{
    calcQuadFreqs();

    int curQuadIndex = 0;
    for (int i = maxLED - 1; i > 0; i--)
    {
        int LEDIndex = (maxLED - 1) - i;
        double size = ledLen * (double)LEDIndex;

        // Serial.println("Size: ");
        // Serial.println(size);
        // Serial.println("Max: ");
        // Serial.println(quadIndexes[curQuadIndex]);
        // Serial.println("");

        // Update quad index if greater than curent arc size
        if (size > quadIndexes[curQuadIndex] && curQuadIndex < 15)
        {
            curQuadIndex++;
        }

        if (i < minLED)
        {
            leds[i] = CRGB::Black;
        }
        else
        {
            int index = curQuadIndex % 4;
            double freqPercent = quadFreqPercentage[index];

            // Are we greater than last value? If so, set it
            if (freqPercent > (quadFreqMaxes[index] + greaterThresh))
            {
                quadFreqMaxes[index] = freqPercent;
            }

            // Get percent value represented at this given turn of the spiral
            double turnMaxPercent = (curQuadIndex / 4 + 1) * percentInc;
            double val = turnMaxPercent - quadFreqMaxes[index];

            uint8_t palIndex = ((index + 1) * 63) - 1;
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

void Quadrant::initArcLengths()
{
  // Setup pi indexes using math science shit
  for(int i = 0; i < 16; i++)
  {
    quadIndexes[i] = calcArcLen(0, i * (PI / 2.0f));
  }
}

double Quadrant::arsinh(double x)
{
  return log(x + sqrt(pow(x, 2) + 1));
}

// Get arc len from start to end theta along spiral
double Quadrant::calcArcLen(double thetaStrt, double thetaStop) 
{
  // Arc length of spiral, by taking End Length - Start Length
  return (C * ( (arsinh(thetaStop) / 2) + ( (thetaStop * sqrt( pow(thetaStop, 2) + 1 )) / 2 ) ) )
       - (C * ( (arsinh(thetaStrt) / 2) + ( (thetaStrt * sqrt( pow(thetaStrt, 2) + 1 )) / 2 ) ) );
}

void Quadrant::calcQuadFreqs()
{
  quadFreqPercentage[0] = channels[0];
  quadFreqPercentage[0] /= (1.0 * 1023.0);

  quadFreqPercentage[1] = channels[1] + channels[2];
  quadFreqPercentage[1] /= (2.0 * 1023.0);

  quadFreqPercentage[2] = channels[3] + channels[4];
  quadFreqPercentage[2] /= (2.0 * 1023.0);

  quadFreqPercentage[3] = channels[5] + channels[6];
  quadFreqPercentage[3] /= (2.0 * 1023.0);
}

void Quadrant::reduce()
{
  for(int i = 0; i < 4; i++)
  {
    if(quadFreqMaxes[i] > 0)
      quadFreqMaxes[i] -= decayRate;
  }
}
