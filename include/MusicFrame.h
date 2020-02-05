#ifndef MUSIC_FRAME_H
#define MUSIC_FRAME_H

#include "Arduino.h"

#define G_NUM_CHANNELS 7

struct MusicFrame
{

    static const int NUM_CHANNELS = G_NUM_CHANNELS;

    static const uint8_t STROBE_PIN = 4;
    static const uint8_t RESET_PIN = 5;
    static const uint8_t AUDIO1_PIN = (uint8_t)'6';
    static const uint8_t AUDIO2_PIN = (uint8_t)'7';

    int channels[G_NUM_CHANNELS];

    int creationTime = 0;
    int creationTick = 0;

    MusicFrame(): MusicFrame(0) {}

    MusicFrame(long tick);

    int& operator[](int i)
    {
        return channels[i];
    }

    void log() const;

};

#endif