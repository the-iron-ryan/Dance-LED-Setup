#include "StaticChanger.h"

StaticChanger::StaticChanger(CRGBSet _leds, CRGBPalette16 _palette, int _curPaletteIndex) : 
    Changer(_leds, _palette)
{
    curPaletteIndex = _curPaletteIndex;
}

void StaticChanger::init() 
{
    leds.fill_solid(palette[curPaletteIndex]);
}

void StaticChanger::step() 
{
    leds.fill_solid(palette[curPaletteIndex]);
}

void StaticChanger::stop() 
{

}
