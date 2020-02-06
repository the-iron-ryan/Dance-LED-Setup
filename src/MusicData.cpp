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
    // Initialize pins
    pinMode     (MusicFrame::AUDIO1_PIN, INPUT);
    pinMode     (MusicFrame::AUDIO2_PIN, INPUT);
    pinMode     (MusicFrame::STROBE_PIN, OUTPUT);
    pinMode     (MusicFrame::RESET_PIN,  OUTPUT);
    digitalWrite(MusicFrame::RESET_PIN,  LOW);
    digitalWrite(MusicFrame::STROBE_PIN, HIGH);

    // Get the current mean energy of all the frame in the frame buffer
    m_energyMean = 0;
    for (int i = 0; i < m_frameBuffer.size; i++)
    {
        m_energyMean += m_frameBuffer[i].energyLevel;
    }
    m_energyMean /= m_frameBuffer.size;

    // Get the current variance of the energy of all frames in the frame buffer
    m_energyDeviation = 0;
    for (int i = 0; i < m_frameBuffer.size; i++)
    {
        m_energyMean += (m_frameBuffer[i].energyLevel - m_energyMean)
                      * (m_frameBuffer[i].energyLevel - m_energyMean);
    }
    m_energyDeviation /= m_frameBuffer.size - 1;
    
    
}

void MusicData::update()
{
    m_ticks++;

    m_frameBuffer.push(ticks);

    MusicFrame first = m_frameBuffer.first();
    MusicFrame last = m_frameBuffer.last();

    float originalMean = m_energyMean;

    m_energyMean += (first.energyLevel - last.energyLevel) / m_frameBuffer.size;
    m_energyVariance += ((first.energyLevel - originalMean) * (first.energyLevel - m_energyMean)
                      - (last.energyLevel - originalMean) * (last.energyLevel - m_energyMean))
                      / m_frameBuffer.size;
    m_energyDeviation = sqrt(m_energyVariance);

    first.deviation = (first.energyLevel - m_energyMean) / m_energyDeviation;

//    Serial.print('\t');
//    Serial.print(m_energyMean);
//    Serial.print('\t');
//    Serial.print(m_energyVariance);
//    Serial.print('\t');
//    Serial.print(m_energyDeviation);
//    Serial.print('\t');
//    Serial.print(first.deviation);
//    Serial.println();

    m_strongBeat = false;
    m_weakBeat   = false;

    if (first.deviation > STRONG_DEVIATION
      && (m_ticks - m_lastStrong) > STRONG_COOLDOWN)
    {
        m_strongBeat = true;
        m_lastStrong = m_ticks;
    }
    
    if (first.deviation > WEAK_DEVIATION
      && (m_ticks - m_lastWeak) > WEAK_COOLDOWN)
    {
        m_weakBeat = true;
        m_lastWeak = m_ticks;
    }

//    Serial.print(m_strongBeat);
//    Serial.print('\t');
//    Serial.print(m_weakBeat);
//    Serial.print('\t');
//    Serial.println();

}