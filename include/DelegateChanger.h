#ifndef DELEGATE_CHANGER_H
#define DELEGATE_CHANGER_H

#include "Changer.h"

class DelegateChanger : public Changer
{
public:
    DelegateChanger(CRGBSet _leds, void (*_del)(void)) : Changer(_leds)
    {
        changerDel = _del;
    }

    virtual void init() override { if(changerDel != NULL) changerDel(); }

private:

    // Delegate that gets called on init
    void (*changerDel)(void);
};


#endif