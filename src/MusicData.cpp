#include "MusicData.h"
#include "MusicFrame.h"
#include "Arduino.h"
#include "pins_arduino.h"

MusicData* MusicData::m_instance = NULL;

MusicData* MusicData::instance()
{
    if (!m_instance)
    {
        m_instance = new MusicData;
        Serial.println("MusicData instance created.");
    }

    return m_instance;
}

MusicData::MusicData()
{
    pinMode     (MusicFrame::AUDIO1_PIN, INPUT);
    pinMode     (MusicFrame::AUDIO2_PIN, INPUT);
    pinMode     (MusicFrame::STROBE_PIN, OUTPUT);
    pinMode     (MusicFrame::RESET_PIN,  OUTPUT);
    digitalWrite(MusicFrame::RESET_PIN,  LOW);
    digitalWrite(MusicFrame::STROBE_PIN, HIGH);
}

void MusicData::update()
{
    m_ticks++;

    m_frameBuffer.push(ticks);
}

void MusicData::analyze()
{
    m_refreshRate = -2;
}