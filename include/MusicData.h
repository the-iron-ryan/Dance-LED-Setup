#ifndef MUSICDATA_H
#define MUSICDATA_H

#define G_NUM_CHANNELS 7
#include <CircularBuffer.h>

struct MusicFrame
{
    int left[G_NUM_CHANNELS];
    int right[G_NUM_CHANNELS];
    int channels[G_NUM_CHANNELS];

    long creationTime = 0;
    long creationTick = 0;

    MusicFrame(): MusicFrame(0) {}

    MusicFrame(long tick);

    int& operator[](int i)
    {
        return channels[i];
    }

    void log() const;

};

class MusicData
{

public:

    static MusicData* instance();

    static const int FRAME_QUEUE_SIZE = 100;

    static const uint8_t STROBE_PIN = 4;
    static const uint8_t RESET_PIN = 5;
    static const uint8_t AUDIO1_PIN = (uint8_t)'6';
    static const uint8_t AUDIO2_PIN = (uint8_t)'7';

    static const int NUM_CHANNELS = G_NUM_CHANNELS;

    long ticks = 0;

    void update();
    MusicData& operator++(int n)
    {
        this->update();
        return *this;
    }

    const MusicFrame& at(int i)
    {
        return m_frameBuffer[i];
    }

    const MusicFrame& operator[](int i)
    {
        return this->at(i);
    }

    const MusicFrame& current()
    {
        return this->at(0);
    }

private:


    static MusicData* m_instance;

    MusicData();

    CircularBuffer<MusicFrame, G_NUM_CHANNELS> m_frameBuffer; 

};

#endif