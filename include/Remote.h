#ifndef REMOTE_H
#define REMOTE_H

#include <IRremote.h>
#include <Thread.h>

class Changer;

enum ERemoteButton
{
    POWER     =  0xFFA25D,  
    FUNC      =  0xFFE21D, 
    VOL_PLUS  =  0xFF629D, 
    BACK      =  0xFF22DD, 
    PAUSE     =  0xFF02FD, 
    FF        =  0xFFC23D, 
    DOWN      =  0xFFE01F, 
    VOL_MINUS =  0xFFA857, 
    UP        =  0xFF906F,
    EQ        =  0xFF9867, 
    ST        =  0xFFB04F, 
    ZERO      =  0xFF6897, 
    ONE       =  0xFF30CF, 
    TWO       =  0xFF18E7, 
    THREE     =  0xFF7A85, 
    FOUR      =  0xFF10EF, 
    FIVE      =  0xFF38C7, 
    SIX       =  0xFF5AAd, 
    SEVEN     =  0xFF42BD, 
    EIGHT     =  0xFF4AB5, 
    NINE      =  0xFF52AD,
    REP       =  0xFFFFFFFF 
};

struct RemotePair
{
    RemotePair() : button(ERemoteButton::REP), changer(nullptr) {}


    RemotePair(ERemoteButton button, Changer* changer) : button(button), changer(changer) {}

    ERemoteButton button;
    Changer *changer;
};

/*
 * Remote - Binds actions to a IR remote
*/
class Remote
{
public:

    Remote(int IRPin, int numRemotePairs, unsigned long censorTimeMilis = 2000);
    ~Remote();

    void addRemotePair(ERemoteButton button, Changer* changer);

    Changer* getChanger(ERemoteButton button);

    void pingResutls();

    ERemoteButton getResultButton();



private:
    IRrecv *IRReceiver;
    decode_results *IRResults;

    RemotePair* pairDict;

    // Time used to delay IR signal 
    unsigned long curTime;

    unsigned long totTime = 2000;

    int totNumRemotePairs;
    int curPairIndex;

};

#endif

