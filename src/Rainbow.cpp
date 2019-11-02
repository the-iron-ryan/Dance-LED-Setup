#include <FastLED.h>
#include "Changer.h"
#include "Rainbow.h"

CRGB Scroll(int pos) {
  pos = abs(pos) % 256;
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  /*
  Serial.print(pos);
  Serial.print(" -> ");
  Serial.print("r: ");
  Serial.print(color.r);
  Serial.print("    g: ");
  Serial.print(color.g);
  Serial.print("    b: ");
  Serial.println(color.b);
  */
  return color;
}

void Rainbow::step() {

    this->tick++;

    CRGB rainbowColor = Scroll(tick);

    this->setAllPixInRange(this->minLED, this->maxLED, rainbowColor);

    return;

}


