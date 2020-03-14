#ifndef STATIC_CHANGER_H
#define STATIC_CHANGER_H

#include "Changer.h"

class StaticChanger : public Changer
{
public:
    StaticChanger(CRGBSet _leds, CRGBPalette16 _palette, int _curPaletteIndex = 0);

    virtual void step() override;

    virtual void init() override;

    virtual void stop() override;

private:
    // Index to read the palette from
    int curPaletteIndex = 0;
};


#endif