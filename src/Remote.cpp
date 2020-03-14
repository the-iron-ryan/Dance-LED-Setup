#include "Remote.h"

#include "ChangerData.h"
#include "Changer.h"
#include "IRremote.h"

Remote::Remote(int IRPin, int numRemotePairs, unsigned long censorTimeMilis)
{
    // Enable IR input
    IRReceiver = new IRrecv(IRPin);
    IRReceiver->enableIRIn();

    IRResults = new decode_results();
    IRResults->decode_type = decode_type_t::LEGO_PF;

    curPairIndex = 0;
    totNumRemotePairs = numRemotePairs;
    totTime = censorTimeMilis;
    pairDict = new RemotePair[totNumRemotePairs];
}
Remote::~Remote()
{
    delete IRReceiver;
    delete IRResults;
    delete[] pairDict;
}

void Remote::addRemotePair(ERemoteButton button, Changer* changer)
{
    if(curPairIndex < totNumRemotePairs)
        pairDict[curPairIndex++] = RemotePair(button, changer);
}

Changer* Remote::getChanger(ERemoteButton button)
{
    Changer* changerPtr = nullptr;
    for(int i = 0; i < curPairIndex; i++)
    {
        if(pairDict[i].button == button)
        {
            changerPtr = pairDict[i].changer;
            break;
        }
    }

    return changerPtr;
}

void Remote::pingResutls()
{
    Changer *curChanger = nullptr;
    if (IRReceiver->decode(IRResults))
    {
        curTime = millis();
        IRReceiver->resume();

        // Begin delay for IR input
        while (millis() - curTime < totTime)
        {
            if (IRReceiver->decode(IRResults))
            {
                Serial.println(IRResults->value, HEX);
                if (IRResults->value != 0xFFFFFFFF)
                {
                    Serial.println("Getting results...");
                    curTime = millis();

                    ERemoteButton button = getResultButton();
                    if (getChanger(button))
                    {
                        curChanger = getChanger(button);
                        CUR_CHANGER = curChanger;
                        Serial.println("Changer Ptr");
                        Serial.println((unsigned long) curChanger, HEX);
                    }
                }
                IRReceiver->resume();
            }
        }
        IRReceiver->resume();
    }
}

ERemoteButton Remote::getResultButton()
{
    switch(IRResults->value){
    case 0xFFA25D: Serial.println("POWER"); break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK");    break;
    case 0xFF02FD: Serial.println("PAUSE");    break;
    case 0xFFC23D: Serial.println("FAST FORWARD");   break;
    case 0xFFE01F: Serial.println("DOWN");    break;
    case 0xFFA857: Serial.println("VOL-");    break;
    case 0xFF906F: Serial.println("UP");    break;
    case 0xFF9867: Serial.println("EQ");    break;
    case 0xFFB04F: Serial.println("ST/REPT");    break;
    case 0xFF6897: Serial.println("0");    break;
    case 0xFF30CF: Serial.println("1");    break;
    case 0xFF18E7: Serial.println("2");    break;
    case 0xFF7A85: Serial.println("3");    break;
    case 0xFF10EF: Serial.println("4");    break;
    case 0xFF38C7: Serial.println("5");    break;
    case 0xFF5AA5: Serial.println("6");    break;
    case 0xFF42BD: Serial.println("7");    break;
    case 0xFF4AB5: Serial.println("8");    break;
    case 0xFF52AD: Serial.println("9");    break;
    case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.print(" other button   ");
    Serial.println(IRResults->value);
    }

    return static_cast<ERemoteButton>(IRResults->value);
}