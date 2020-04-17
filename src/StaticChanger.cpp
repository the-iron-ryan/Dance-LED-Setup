#include "StaticChanger.h"

StaticChanger::StaticChanger(CRGBSet _leds, CRGB _color) :
    Changer(_leds)
{
    bUsesPalette = false;
    curSolidColor = _color;
}

StaticChanger::StaticChanger(CRGBSet _leds, CRGBPalette16 _palette, int _curPaletteIndex) : 
    Changer(_leds, _palette)
{
    bUsesPalette = true;
    curPaletteIndex = _curPaletteIndex;
}

void StaticChanger::init() 
{
    if(bUsesPalette)
    {
        leds.fill_solid(palette[curPaletteIndex]);
    }
    else
    {
        leds.fill_solid(curSolidColor);
    }
}

void StaticChanger::step() 
{
    if(bUsesPalette)
    {
        leds.fill_solid(palette[curPaletteIndex]);
    }
    else
    {
        leds.fill_solid(curSolidColor);
    }
}

void StaticChanger::stop() 
{

}
