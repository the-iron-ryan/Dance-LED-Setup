#include "MusicData.h"
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
    pinMode     (MusicData::AUDIO1_PIN, INPUT);
    pinMode     (MusicData::AUDIO2_PIN, INPUT);
    pinMode     (MusicData::STROBE_PIN, OUTPUT);
    pinMode     (MusicData::RESET_PIN,  OUTPUT);
    digitalWrite(MusicData::RESET_PIN,  LOW);
    digitalWrite(MusicData::STROBE_PIN, HIGH);
}

MusicFrame::MusicFrame(long tick):
    creationTick(tick),
    creationTime(millis())
{
    // Cycle reset pin
    digitalWrite(MusicData::RESET_PIN, HIGH);
    digitalWrite(MusicData::RESET_PIN, LOW);

    for(int band = 0; band < MusicData::NUM_CHANNELS; band++)
    {
        digitalWrite(MusicData::STROBE_PIN, LOW); // strobe pin on the shield - kicks the IC up to the next band 
        delayMicroseconds(50); // Delay for allow voltage to adjust

        // Update left, right, and main channel entries
        left[band]    = analogRead(MusicData::AUDIO1_PIN);
        right[band]   = analogRead(MusicData::AUDIO2_PIN);
        channels[band] = max(left[band], right[band]); 

        // Reset the strobe pin to high volt
        digitalWrite(MusicData::STROBE_PIN, HIGH); 
    }
}

void MusicFrame::log() const
{
    Serial.print(creationTick);
    Serial.print(',');
    Serial.print(creationTime);

    for (int i = 0; i < G_NUM_CHANNELS; i++)
    {
        Serial.print(',');
        Serial.print(channels[i]);
    }

    Serial.println();
}

void MusicData::update()
{
    ticks++;
    m_frameBuffer.push(MusicFrame(ticks));
}