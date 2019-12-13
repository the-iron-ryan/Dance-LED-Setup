#include "MusicFrame.h"

MusicFrame::MusicFrame(long tick):
    creationTick(tick),
    creationTime(millis())
{
    // Cycle reset pin
    digitalWrite(MusicFrame::RESET_PIN, HIGH);
    digitalWrite(MusicFrame::RESET_PIN, LOW);

    for(int band = 0; band < MusicFrame::NUM_CHANNELS; band++)
    {
        digitalWrite(MusicFrame::STROBE_PIN, LOW); // strobe pin on the shield - kicks the IC up to the next band 
        delayMicroseconds(50); // Delay for allow voltage to adjust

        // Update left, right, and main channel entries
        left[band]    = analogRead(MusicFrame::AUDIO1_PIN);
        right[band]   = analogRead(MusicFrame::AUDIO2_PIN);
        channels[band] = max(left[band], right[band]); 

        // Reset the strobe pin to high volt
        digitalWrite(MusicFrame::STROBE_PIN, HIGH); 
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